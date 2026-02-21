# Impostor - Flipper Zero Game

A social deduction party game for Flipper Zero. Everyone gets the same secret word except the impostor(s) - figure out who doesn't know!

Built to have fun with my family.

**Author:** nenobol

## How to Play

1. Select language (English / Espanol)
2. Select number of players (3-10)
3. Select number of impostors (1 to players-1)
4. Choose a word category (Animals, Food, Places, Objects, Actions)
5. Pass the Flipper around - each player privately views their role by pressing OK
6. The 3-minute discussion timer starts - try to figure out who the impostor is!
7. The impostor tries to blend in without knowing the secret word

## Game Rules

- Everyone except the impostor(s) sees the same secret word
- Players take turns giving clues about the word without revealing it
- The impostor must pretend to know the word by giving vague clues
- After discussion, vote to eliminate who you think is the impostor
- Impostors win if they're not caught, or if they guess the word!

## Building

### Prerequisites

- Flipper Zero firmware source (official or custom like Momentum/Unleashed)
- ARM GCC toolchain

### Build Steps

1. Clone this repo into your firmware's `applications_user/` folder:

```bash
cd /path/to/flipperzero-firmware
cp -r /path/to/impostor applications_user/
```

2. Build the FAP:

```bash
./fbt fap_impostor
```

3. The compiled `.fap` file will be in `build/f7-firmware-D/.extapps/`

### Installing

**Option A: USB Install (Build & Launch)**

```bash
./fbt launch APPSRC=applications_user/impostor
```

**Option B: Manual Copy**

Copy the `.fap` file to your Flipper's SD card under `/ext/apps/Games/`

## File Structure

```
impostor/
├── README.md                 # This file
├── application.fam           # App manifest
├── impostor.c                # Main app entry & game logic
├── impostor.h                # Types & definitions
├── words.h                   # Word lists (EN & ES)
├── scenes/
│   ├── scene_main_menu.c     # Main menu
│   ├── scene_language.c      # Language selection
│   ├── scene_player_count.c  # Player count selection
│   ├── scene_impostor_count.c # Impostor count selection
│   ├── scene_category.c      # Category selection
│   ├── scene_role_reveal.c   # Pass & reveal roles
│   ├── scene_discussion.c    # Discussion timer
│   └── scene_about.c         # About screen
└── images/
    └── impostor_10px.png     # App icon
```

## License

MIT License
