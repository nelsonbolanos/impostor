// Impostor - Social deduction party game for Flipper Zero
#include "impostor.h"
#include "words.h"
#include <furi_hal_random.h>

// ============================================================================
// SCENE HANDLER ARRAYS (forward declarations are in impostor.h)
// ============================================================================

void (*const impostor_scene_on_enter_handlers[])(void*) = {
    impostor_scene_main_menu_on_enter,
    impostor_scene_language_on_enter,
    impostor_scene_player_count_on_enter,
    impostor_scene_impostor_count_on_enter,
    impostor_scene_category_on_enter,
    impostor_scene_role_reveal_on_enter,
    impostor_scene_discussion_on_enter,
    impostor_scene_about_on_enter,
};

bool (*const impostor_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    impostor_scene_main_menu_on_event,
    impostor_scene_language_on_event,
    impostor_scene_player_count_on_event,
    impostor_scene_impostor_count_on_event,
    impostor_scene_category_on_event,
    impostor_scene_role_reveal_on_event,
    impostor_scene_discussion_on_event,
    impostor_scene_about_on_event,
};

void (*const impostor_scene_on_exit_handlers[])(void*) = {
    impostor_scene_main_menu_on_exit,
    impostor_scene_language_on_exit,
    impostor_scene_player_count_on_exit,
    impostor_scene_impostor_count_on_exit,
    impostor_scene_category_on_exit,
    impostor_scene_role_reveal_on_exit,
    impostor_scene_discussion_on_exit,
    impostor_scene_about_on_exit,
};

static const SceneManagerHandlers impostor_scene_handlers = {
    .on_enter_handlers = impostor_scene_on_enter_handlers,
    .on_event_handlers = impostor_scene_on_event_handlers,
    .on_exit_handlers = impostor_scene_on_exit_handlers,
    .scene_num = ImpostorSceneCount,
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void impostor_select_impostors(GameState* game) {
    // Reset all roles
    for(uint8_t i = 0; i < MAX_PLAYERS; i++) {
        game->is_impostor[i] = false;
    }

    // Randomly select impostors using hardware RNG
    uint8_t impostors_selected = 0;
    while(impostors_selected < game->impostor_count) {
        uint8_t candidate = furi_hal_random_get() % game->player_count;
        if(!game->is_impostor[candidate]) {
            game->is_impostor[candidate] = true;
            impostors_selected++;
        }
    }
}

const char* impostor_get_random_word(Category category, Language language) {
    uint8_t index = furi_hal_random_get() % WORDS_PER_CATEGORY;
    if(language == LanguageSpanish) {
        return words_es[category][index];
    }
    return words_en[category][index];
}

const char* impostor_get_category_name(Category category, Language language) {
    if(language == LanguageSpanish) {
        return category_names_es[category];
    }
    return category_names_en[category];
}

// ============================================================================
// APP LIFECYCLE
// ============================================================================

static bool impostor_navigation_callback(void* context) {
    ImpostorApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void impostor_timer_callback(void* context) {
    ImpostorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, ImpostorEventTimerTick);
}

static bool impostor_custom_callback(void* context, uint32_t event) {
    ImpostorApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

ImpostorApp* impostor_app_alloc(void) {
    ImpostorApp* app = malloc(sizeof(ImpostorApp));
    if(!app) return NULL;

    // Initialize game state with defaults
    app->game.language = LanguageEnglish;
    app->game.player_count = 4;
    app->game.impostor_count = 1;
    app->game.current_player = 0;
    app->game.category = CategoryAnimals;
    app->game.secret_word = NULL;
    app->game.reveal_state = RoleRevealStateWaiting;
    app->game.timer_start_tick = 0;

    // Open records
    app->gui = furi_record_open(RECORD_GUI);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Create view dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, impostor_navigation_callback);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, impostor_custom_callback);

    // Create scene manager
    app->scene_manager = scene_manager_alloc(&impostor_scene_handlers, app);

    // Create views
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, ImpostorViewSubmenu, submenu_get_view(app->submenu));

    app->variable_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(app->view_dispatcher, ImpostorViewVariableItemList,
                             variable_item_list_get_view(app->variable_item_list));

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, ImpostorViewWidget, widget_get_view(app->widget));

    // Create timer for discussion countdown
    app->timer = furi_timer_alloc(impostor_timer_callback, FuriTimerTypePeriodic, app);

    return app;
}

void impostor_app_free(ImpostorApp* app) {
    // Stop timer
    furi_timer_stop(app->timer);
    furi_timer_free(app->timer);

    // Remove and free views
    view_dispatcher_remove_view(app->view_dispatcher, ImpostorViewSubmenu);
    submenu_free(app->submenu);

    view_dispatcher_remove_view(app->view_dispatcher, ImpostorViewVariableItemList);
    variable_item_list_free(app->variable_item_list);

    view_dispatcher_remove_view(app->view_dispatcher, ImpostorViewWidget);
    widget_free(app->widget);

    // Free scene manager and view dispatcher
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    // Close records
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);

    free(app);
}

// ============================================================================
// SCENE: MAIN MENU
// ============================================================================

typedef enum {
    MainMenuIndexPlay,
    MainMenuIndexAbout,
} MainMenuIndex;

static void impostor_scene_main_menu_callback(void* context, uint32_t index) {
    ImpostorApp* app = context;
    if(index == MainMenuIndexPlay) {
        scene_manager_next_scene(app->scene_manager, ImpostorSceneLanguage);
    } else if(index == MainMenuIndexAbout) {
        scene_manager_next_scene(app->scene_manager, ImpostorSceneAbout);
    }
}

void impostor_scene_main_menu_on_enter(void* context) {
    ImpostorApp* app = context;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Impostor");
    submenu_add_item(app->submenu, "Play", MainMenuIndexPlay, impostor_scene_main_menu_callback, app);
    submenu_add_item(app->submenu, "About", MainMenuIndexAbout, impostor_scene_main_menu_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewSubmenu);
}

bool impostor_scene_main_menu_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void impostor_scene_main_menu_on_exit(void* context) {
    ImpostorApp* app = context;
    submenu_reset(app->submenu);
}

// ============================================================================
// SCENE: LANGUAGE
// ============================================================================

static void impostor_scene_language_callback(void* context, uint32_t index) {
    ImpostorApp* app = context;
    app->game.language = (Language)index;
    scene_manager_next_scene(app->scene_manager, ImpostorScenePlayerCount);
}

void impostor_scene_language_on_enter(void* context) {
    ImpostorApp* app = context;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Language / Idioma");
    submenu_add_item(app->submenu, "English", LanguageEnglish, impostor_scene_language_callback, app);
    submenu_add_item(app->submenu, "Espanol", LanguageSpanish, impostor_scene_language_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewSubmenu);
}

bool impostor_scene_language_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void impostor_scene_language_on_exit(void* context) {
    ImpostorApp* app = context;
    submenu_reset(app->submenu);
}

// ============================================================================
// SCENE: PLAYER COUNT
// ============================================================================

static const char* player_count_values[] = {"3", "4", "5", "6", "7", "8", "9", "10"};

static void impostor_scene_player_count_changed(VariableItem* item) {
    ImpostorApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    app->game.player_count = index + MIN_PLAYERS;
    variable_item_set_current_value_text(item, player_count_values[index]);

    // Reset impostor count if it would exceed players - 1
    if(app->game.impostor_count >= app->game.player_count) {
        app->game.impostor_count = app->game.player_count - 1;
    }
}

static void impostor_scene_player_count_enter_callback(void* context, uint32_t index) {
    UNUSED(index);
    ImpostorApp* app = context;
    scene_manager_next_scene(app->scene_manager, ImpostorSceneImpostorCount);
}

void impostor_scene_player_count_on_enter(void* context) {
    ImpostorApp* app = context;
    variable_item_list_reset(app->variable_item_list);

    VariableItem* item = variable_item_list_add(
        app->variable_item_list,
        app->game.language == LanguageSpanish ? "Jugadores" : "Players",
        8, // 3-10 players
        impostor_scene_player_count_changed,
        app);

    uint8_t index = app->game.player_count - MIN_PLAYERS;
    variable_item_set_current_value_index(item, index);
    variable_item_set_current_value_text(item, player_count_values[index]);

    variable_item_list_set_enter_callback(app->variable_item_list, impostor_scene_player_count_enter_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewVariableItemList);
}

bool impostor_scene_player_count_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void impostor_scene_player_count_on_exit(void* context) {
    ImpostorApp* app = context;
    variable_item_list_reset(app->variable_item_list);
}

// ============================================================================
// SCENE: IMPOSTOR COUNT
// ============================================================================

static const char* impostor_count_values[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static void impostor_scene_impostor_count_changed(VariableItem* item) {
    ImpostorApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    app->game.impostor_count = index + 1;
    variable_item_set_current_value_text(item, impostor_count_values[index]);
}

static void impostor_scene_impostor_count_enter_callback(void* context, uint32_t index) {
    UNUSED(index);
    ImpostorApp* app = context;
    scene_manager_next_scene(app->scene_manager, ImpostorSceneCategory);
}

void impostor_scene_impostor_count_on_enter(void* context) {
    ImpostorApp* app = context;
    variable_item_list_reset(app->variable_item_list);

    uint8_t max_impostors = app->game.player_count - 1;

    VariableItem* item = variable_item_list_add(
        app->variable_item_list,
        app->game.language == LanguageSpanish ? "Impostores" : "Impostors",
        max_impostors,
        impostor_scene_impostor_count_changed,
        app);

    uint8_t index = app->game.impostor_count - 1;
    if(index >= max_impostors) index = max_impostors - 1;
    app->game.impostor_count = index + 1;

    variable_item_set_current_value_index(item, index);
    variable_item_set_current_value_text(item, impostor_count_values[index]);

    variable_item_list_set_enter_callback(app->variable_item_list, impostor_scene_impostor_count_enter_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewVariableItemList);
}

bool impostor_scene_impostor_count_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void impostor_scene_impostor_count_on_exit(void* context) {
    ImpostorApp* app = context;
    variable_item_list_reset(app->variable_item_list);
}

// ============================================================================
// SCENE: CATEGORY
// ============================================================================

static void impostor_scene_category_callback(void* context, uint32_t index) {
    ImpostorApp* app = context;
    app->game.category = (Category)index;

    // Initialize game - select word and impostors
    app->game.secret_word = impostor_get_random_word(app->game.category, app->game.language);
    impostor_select_impostors(&app->game);
    app->game.current_player = 0;
    app->game.reveal_state = RoleRevealStateWaiting;

    scene_manager_next_scene(app->scene_manager, ImpostorSceneRoleReveal);
}

void impostor_scene_category_on_enter(void* context) {
    ImpostorApp* app = context;
    submenu_reset(app->submenu);

    const char* header = app->game.language == LanguageSpanish ? "Categoria" : "Category";
    submenu_set_header(app->submenu, header);

    for(uint8_t i = 0; i < CategoryCount; i++) {
        submenu_add_item(
            app->submenu,
            impostor_get_category_name((Category)i, app->game.language),
            i,
            impostor_scene_category_callback,
            app);
    }

    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewSubmenu);
}

bool impostor_scene_category_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void impostor_scene_category_on_exit(void* context) {
    ImpostorApp* app = context;
    submenu_reset(app->submenu);
}

// ============================================================================
// SCENE: ROLE REVEAL
// ============================================================================

typedef enum {
    RoleRevealEventOkPressed = 0,
} RoleRevealEvent;

static void impostor_scene_role_reveal_widget_callback(GuiButtonType result, InputType type, void* context) {
    ImpostorApp* app = context;
    if(type == InputTypeShort && result == GuiButtonTypeCenter) {
        view_dispatcher_send_custom_event(app->view_dispatcher, RoleRevealEventOkPressed);
    }
}

static void impostor_scene_role_reveal_update_widget(ImpostorApp* app) {
    widget_reset(app->widget);

    FuriString* text = furi_string_alloc();

    if(app->game.reveal_state == RoleRevealStateWaiting) {
        // Show "Player X, press OK to see your role"
        if(app->game.language == LanguageSpanish) {
            furi_string_printf(text, "Jugador %d\n\nPresiona OK\npara ver tu rol",
                              app->game.current_player + 1);
        } else {
            furi_string_printf(text, "Player %d\n\nPress OK\nto see your role",
                              app->game.current_player + 1);
        }
        widget_add_string_multiline_element(app->widget, 64, 20, AlignCenter, AlignTop,
                                            FontPrimary, furi_string_get_cstr(text));
        widget_add_button_element(app->widget, GuiButtonTypeCenter, "OK",
                                  impostor_scene_role_reveal_widget_callback, app);
    } else if(app->game.reveal_state == RoleRevealStateRevealed) {
        // Show role
        bool is_impostor = app->game.is_impostor[app->game.current_player];

        if(is_impostor) {
            if(app->game.language == LanguageSpanish) {
                furi_string_set(text, "IMPOSTOR!\n\nNo conoces\nla palabra");
            } else {
                furi_string_set(text, "IMPOSTOR!\n\nYou don't know\nthe word");
            }
        } else {
            if(app->game.language == LanguageSpanish) {
                furi_string_printf(text, "La palabra es:\n\n%s", app->game.secret_word);
            } else {
                furi_string_printf(text, "The word is:\n\n%s", app->game.secret_word);
            }
        }

        widget_add_string_multiline_element(app->widget, 64, 12, AlignCenter, AlignTop,
                                            FontPrimary, furi_string_get_cstr(text));
        widget_add_button_element(app->widget, GuiButtonTypeCenter, "OK",
                                  impostor_scene_role_reveal_widget_callback, app);

        // Vibrate to indicate role shown
        notification_message(app->notifications, &sequence_single_vibro);
    } else if(app->game.reveal_state == RoleRevealStatePassDevice) {
        // Show "Pass device to next player"
        if(app->game.language == LanguageSpanish) {
            furi_string_printf(text, "Pasa el dispositivo\nal Jugador %d",
                              app->game.current_player + 1);
        } else {
            furi_string_printf(text, "Pass device\nto Player %d",
                              app->game.current_player + 1);
        }
        widget_add_string_multiline_element(app->widget, 64, 24, AlignCenter, AlignTop,
                                            FontPrimary, furi_string_get_cstr(text));
        widget_add_button_element(app->widget, GuiButtonTypeCenter, "Ready",
                                  impostor_scene_role_reveal_widget_callback, app);
    }

    furi_string_free(text);
}

void impostor_scene_role_reveal_on_enter(void* context) {
    ImpostorApp* app = context;
    impostor_scene_role_reveal_update_widget(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewWidget);
}

bool impostor_scene_role_reveal_on_event(void* context, SceneManagerEvent event) {
    ImpostorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom && event.event == RoleRevealEventOkPressed) {
        if(app->game.reveal_state == RoleRevealStateWaiting) {
            // Show role
            app->game.reveal_state = RoleRevealStateRevealed;
            impostor_scene_role_reveal_update_widget(app);
            consumed = true;
        } else if(app->game.reveal_state == RoleRevealStateRevealed) {
            // Move to next player or start discussion
            app->game.current_player++;
            if(app->game.current_player >= app->game.player_count) {
                // All players have seen their roles, start discussion
                scene_manager_next_scene(app->scene_manager, ImpostorSceneDiscussion);
            } else {
                app->game.reveal_state = RoleRevealStatePassDevice;
                impostor_scene_role_reveal_update_widget(app);
            }
            consumed = true;
        } else if(app->game.reveal_state == RoleRevealStatePassDevice) {
            // Next player ready
            app->game.reveal_state = RoleRevealStateWaiting;
            impostor_scene_role_reveal_update_widget(app);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Block back button during role reveal - no cheating!
        consumed = true;
    }

    return consumed;
}

void impostor_scene_role_reveal_on_exit(void* context) {
    ImpostorApp* app = context;
    widget_reset(app->widget);
}

// ============================================================================
// SCENE: DISCUSSION
// ============================================================================

typedef enum {
    DiscussionEventEndGame = 1,
    DiscussionEventNewGame = 2,
} DiscussionEvent;

static bool discussion_showing_results = false;

static void impostor_scene_discussion_end_callback(GuiButtonType result, InputType type, void* context) {
    ImpostorApp* app = context;
    if(type == InputTypeShort && result == GuiButtonTypeCenter) {
        view_dispatcher_send_custom_event(app->view_dispatcher, DiscussionEventEndGame);
    }
}

static void impostor_scene_discussion_newgame_callback(GuiButtonType result, InputType type, void* context) {
    ImpostorApp* app = context;
    if(type == InputTypeShort && result == GuiButtonTypeCenter) {
        view_dispatcher_send_custom_event(app->view_dispatcher, DiscussionEventNewGame);
    }
}

static void impostor_scene_discussion_update_widget(ImpostorApp* app, bool show_results) {
    widget_reset(app->widget);

    FuriString* text = furi_string_alloc();

    if(show_results) {
        // Show game results - who was impostor and what the word was
        if(app->game.language == LanguageSpanish) {
            furi_string_set(text, "JUEGO TERMINADO!\n\n");
            furi_string_cat_printf(text, "Palabra: %s\n\nImpostor(es): ", app->game.secret_word);
        } else {
            furi_string_set(text, "GAME OVER!\n\n");
            furi_string_cat_printf(text, "Word: %s\n\nImpostor(s): ", app->game.secret_word);
        }

        bool first = true;
        for(uint8_t i = 0; i < app->game.player_count; i++) {
            if(app->game.is_impostor[i]) {
                if(!first) furi_string_cat(text, ", ");
                furi_string_cat_printf(text, "%d", i + 1);
                first = false;
            }
        }

        widget_add_string_multiline_element(app->widget, 64, 4, AlignCenter, AlignTop,
                                            FontPrimary, furi_string_get_cstr(text));

        // Add "Play Again" button
        const char* btn_text = app->game.language == LanguageSpanish ? "Jugar" : "Play";
        widget_add_button_element(app->widget, GuiButtonTypeCenter, btn_text,
                                  impostor_scene_discussion_newgame_callback, app);
    } else {
        // Show discussion timer
        uint32_t elapsed_ticks = furi_get_tick() - app->game.timer_start_tick;
        uint32_t elapsed_seconds = elapsed_ticks / furi_kernel_get_tick_frequency();

        int32_t remaining = DISCUSSION_TIME_SECONDS - elapsed_seconds;
        if(remaining < 0) remaining = 0;

        uint32_t minutes = remaining / 60;
        uint32_t seconds = remaining % 60;

        if(app->game.language == LanguageSpanish) {
            furi_string_set(text, "DISCUSION!");
        } else {
            furi_string_set(text, "DISCUSSION!");
        }
        widget_add_string_element(app->widget, 64, 2, AlignCenter, AlignTop,
                                  FontPrimary, furi_string_get_cstr(text));

        furi_string_printf(text, "%lu:%02lu", minutes, seconds);
        widget_add_string_element(app->widget, 64, 28, AlignCenter, AlignCenter,
                                  FontBigNumbers, furi_string_get_cstr(text));

        // Add "End Game" button
        const char* btn_text = app->game.language == LanguageSpanish ? "Terminar" : "End";
        widget_add_button_element(app->widget, GuiButtonTypeCenter, btn_text,
                                  impostor_scene_discussion_end_callback, app);

        // Vibrate when time is up
        if(remaining == 0) {
            furi_timer_stop(app->timer);
            notification_message(app->notifications, &sequence_double_vibro);
        }
    }

    furi_string_free(text);
}

void impostor_scene_discussion_on_enter(void* context) {
    ImpostorApp* app = context;

    // Reset state
    discussion_showing_results = false;

    // Start timer
    app->game.timer_start_tick = furi_get_tick();
    furi_timer_start(app->timer, furi_kernel_get_tick_frequency()); // 1 second interval

    impostor_scene_discussion_update_widget(app, false);
    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewWidget);
}

bool impostor_scene_discussion_on_event(void* context, SceneManagerEvent event) {
    ImpostorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == ImpostorEventTimerTick) {
            impostor_scene_discussion_update_widget(app, false);
            consumed = true;
        } else if(event.event == DiscussionEventEndGame) {
            // Stop timer and show results
            furi_timer_stop(app->timer);
            discussion_showing_results = true;
            impostor_scene_discussion_update_widget(app, true);
            notification_message(app->notifications, &sequence_double_vibro);
            consumed = true;
        } else if(event.event == DiscussionEventNewGame) {
            // Go back to main menu for a new game
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, ImpostorSceneMainMenu);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        if(discussion_showing_results) {
            // Allow exit after game is over
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, ImpostorSceneMainMenu);
        }
        // Block back button during active game - can't go back to see the word!
        consumed = true;
    }

    return consumed;
}

void impostor_scene_discussion_on_exit(void* context) {
    ImpostorApp* app = context;
    furi_timer_stop(app->timer);
    discussion_showing_results = false;
    widget_reset(app->widget);
}

// ============================================================================
// SCENE: ABOUT
// ============================================================================

void impostor_scene_about_on_enter(void* context) {
    ImpostorApp* app = context;
    widget_reset(app->widget);

    widget_add_string_element(app->widget, 64, 5, AlignCenter, AlignTop,
                              FontPrimary, "Impostor");
    widget_add_string_element(app->widget, 64, 20, AlignCenter, AlignTop,
                              FontSecondary, "v1.0");
    widget_add_string_element(app->widget, 64, 35, AlignCenter, AlignTop,
                              FontSecondary, "Social deduction game");
    widget_add_string_element(app->widget, 64, 50, AlignCenter, AlignTop,
                              FontSecondary, "by nenobol");

    view_dispatcher_switch_to_view(app->view_dispatcher, ImpostorViewWidget);
}

bool impostor_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void impostor_scene_about_on_exit(void* context) {
    ImpostorApp* app = context;
    widget_reset(app->widget);
}

// ============================================================================
// ENTRY POINT
// ============================================================================

int32_t impostor_app(void* p) {
    UNUSED(p);

    ImpostorApp* app = impostor_app_alloc();
    if(!app) return 1;

    // Attach to GUI and start
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, ImpostorSceneMainMenu);
    view_dispatcher_run(app->view_dispatcher);

    impostor_app_free(app);
    return 0;
}
