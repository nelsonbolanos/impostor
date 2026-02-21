# Impostor - Flipper Zero Game

A social deduction party game for Flipper Zero. Everyone gets the same secret word except the impostor(s) - figure out who doesn't know!

Built to have fun with my family.

**Author:** nelsonbolanos

## How to Play

1. Select language (English / Español)
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

- [uFBT](https://github.com/flipperdevices/flipperzero-ufbt) (micro Flipper Build Tool)

Install with:
```bash
pip install ufbt
```

### Build Steps

1. Clone this repo:

```bash
git clone https://github.com/nelsonbolanos/impostor.git
cd impostor
```

2. Build the FAP:

```bash
ufbt
```

3. The compiled `.fap` file will be in `dist/`

### Installing

**Option A: Build & Launch (Flipper connected via USB)**

```bash
ufbt launch
```

**Option B: Manual Copy**

Copy `dist/impostor.fap` to your Flipper's SD card under `/ext/apps/Games/`

## File Structure

```
impostor/
├── README.md           # This file
├── application.fam     # App manifest
├── impostor.c          # Main app entry & game logic
├── impostor.h          # Types & definitions
├── words.h             # Word lists (EN & ES)
└── images/
    └── impostor_10px.png   # App icon
```

## License

MIT License
