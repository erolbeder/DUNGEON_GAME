# Dungeon Game

A simple text-based dungeon exploration game where you navigate rooms, collect items, fight creatures, and save or load your progress.

## Features
- **Explore**: Navigate through connected rooms in the dungeon.
- **Collect Items**: Pick up and manage items found in the dungeon.
- **Combat**: Engage in battles with creatures inhabiting the rooms.
- **Save/Load**: Save your progress and continue later.

## Gameplay

Use the following commands during the game:

- `move <direction>`: Move to another room (`up`, `down`, `left`, or `right`).
- `look`: Get details about the current room, items, and creatures.
- `inventory`: Show the items you are carrying.
- `pickup <item>`: Collect an item in the room.
- `attack`: Fight the creature in the current room.
- `save <filepath>`: Save your progress to a file.
- `load <filepath>`: Load a previously saved game.
- `exit`: Quit the game.

## Installation

### Prerequisites
- GCC or any compatible C compiler.
- `make` utility.

### Steps
1. Clone or download the game source files.
2. Compile the game using `make`:
   ```bash
   make
   ```
3. Run the game:
   ```bash
   ./dungeon
   ```
4. Clean up build files if needed:
   ```bash
   make clean
   ```


## File Structure
- `main.c`: Entry point for the game.
- `game.c`, `game.h`: Core game logic and state management.
- `player.c`, `player.h`: Player-related functionality.
- `rooms.c`, `rooms.h`: Room and dungeon setup.
- `creatures.c`, `creatures.h`: Creature handling.
- `items.c`, `items.h`: Item management.
- `commands.c`, `commands.h`: Command parsing and execution.
- `Makefile`: Build automation script.
- `data/rooms.txt`: Room configuration.
- `data/items.txt`: Item definitions.
- `data/creatures.txt`: Creature definitions.

