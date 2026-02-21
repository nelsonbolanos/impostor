#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>

#define MAX_PLAYERS 10
#define MIN_PLAYERS 3
#define DISCUSSION_TIME_SECONDS (3 * 60)

// Language enum
typedef enum {
    LanguageEnglish = 0,
    LanguageSpanish = 1,
} Language;

// Category enum
typedef enum {
    CategoryAnimals = 0,
    CategoryFood,
    CategoryPlaces,
    CategoryObjects,
    CategoryActions,
    CategoryCount,
} Category;

// Scene IDs
typedef enum {
    ImpostorSceneMainMenu,
    ImpostorSceneLanguage,
    ImpostorScenePlayerCount,
    ImpostorSceneImpostorCount,
    ImpostorSceneCategory,
    ImpostorSceneRoleReveal,
    ImpostorSceneDiscussion,
    ImpostorSceneAbout,
    ImpostorSceneCount,
} ImpostorScene;

// View IDs
typedef enum {
    ImpostorViewSubmenu,
    ImpostorViewVariableItemList,
    ImpostorViewWidget,
} ImpostorView;

// Custom events
typedef enum {
    ImpostorEventTimerTick = 100,
} ImpostorEvent;

// Role reveal state machine
typedef enum {
    RoleRevealStateWaiting,
    RoleRevealStateRevealed,
    RoleRevealStatePassDevice,
} RoleRevealState;

// Game state
typedef struct {
    Language language;
    uint8_t player_count;
    uint8_t impostor_count;
    uint8_t current_player;
    Category category;
    bool is_impostor[MAX_PLAYERS];
    const char* secret_word;
    RoleRevealState reveal_state;
    uint32_t timer_start_tick;
} GameState;

// Main app structure
typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    NotificationApp* notifications;

    // Views
    Submenu* submenu;
    VariableItemList* variable_item_list;
    Widget* widget;

    // Timer for discussion
    FuriTimer* timer;

    // Game state
    GameState game;
} ImpostorApp;

// App lifecycle
ImpostorApp* impostor_app_alloc(void);
void impostor_app_free(ImpostorApp* app);

// Scene handlers - defined in scene files
extern void (*const impostor_scene_on_enter_handlers[])(void*);
extern bool (*const impostor_scene_on_event_handlers[])(void*, SceneManagerEvent);
extern void (*const impostor_scene_on_exit_handlers[])(void*);

// Scene functions for each scene
void impostor_scene_main_menu_on_enter(void* context);
bool impostor_scene_main_menu_on_event(void* context, SceneManagerEvent event);
void impostor_scene_main_menu_on_exit(void* context);

void impostor_scene_language_on_enter(void* context);
bool impostor_scene_language_on_event(void* context, SceneManagerEvent event);
void impostor_scene_language_on_exit(void* context);

void impostor_scene_player_count_on_enter(void* context);
bool impostor_scene_player_count_on_event(void* context, SceneManagerEvent event);
void impostor_scene_player_count_on_exit(void* context);

void impostor_scene_impostor_count_on_enter(void* context);
bool impostor_scene_impostor_count_on_event(void* context, SceneManagerEvent event);
void impostor_scene_impostor_count_on_exit(void* context);

void impostor_scene_category_on_enter(void* context);
bool impostor_scene_category_on_event(void* context, SceneManagerEvent event);
void impostor_scene_category_on_exit(void* context);

void impostor_scene_role_reveal_on_enter(void* context);
bool impostor_scene_role_reveal_on_event(void* context, SceneManagerEvent event);
void impostor_scene_role_reveal_on_exit(void* context);

void impostor_scene_discussion_on_enter(void* context);
bool impostor_scene_discussion_on_event(void* context, SceneManagerEvent event);
void impostor_scene_discussion_on_exit(void* context);

void impostor_scene_about_on_enter(void* context);
bool impostor_scene_about_on_event(void* context, SceneManagerEvent event);
void impostor_scene_about_on_exit(void* context);

// Helper functions
void impostor_select_impostors(GameState* game);
const char* impostor_get_random_word(Category category, Language language);
const char* impostor_get_category_name(Category category, Language language);
