#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

static Player player;
static Room* rooms = NULL;
static int room_count = 0;
static int current_room_index = 0;

/* Initializes the game: loads rooms, items, and creatures */
int init_game(const char* rooms_file, const char* items_file, const char* creatures_file) {
    init_player(&player); // Initialize the player

    if (load_rooms(rooms_file, &rooms, &room_count) != 0) {
        fprintf(stderr, "Error: Failed to load rooms from %s\n", rooms_file);
        return -1;
    }

    if (load_items(items_file) != 0) {
        fprintf(stderr, "Error: Failed to load items from %s\n", items_file);
        return -1;
    }

    if (load_creatures(creatures_file) != 0) {
        fprintf(stderr, "Error: Failed to load creatures from %s\n", creatures_file);
        return -1;
    }

    current_room_index = 0; // Start at room zero
    return 0;
}

/* Cleans up the game: frees resources from memory */
void cleanup_game() {
    free_rooms(rooms, room_count);
    free_items();
    free_creatures();
}

/* Checks if the game is over */
int game_over() {
    return (player.health <= 0);
}

/* Returns player information */
Player* get_player() {
    return &player;
}

/* Returns the current room */
Room* get_current_room() {
    return (current_room_index >= 0 && current_room_index < room_count) ? &rooms[current_room_index] : NULL;
}

/* Gets the current room index */
int get_current_room_index() {
    return current_room_index;
}

/* Sets the current room index */
void set_current_room_index(int i) {
    if (i >= 0 && i < room_count) {
        current_room_index = i;
    }
}

/* Saves the game state to a file */
int save_game(const char* filepath) {
    FILE* f = fopen(filepath, "w");
    if (!f) {
        fprintf(stderr, "Error: Cannot open save file %s\n", filepath);
        return -1;
    }

    Player* p = get_player();

    // Save player information
    fprintf(f, "PLAYER\n%d %d %d\n", p->health, p->strength, p->inventory_count);
    for (int i = 0; i < p->inventory_count; i++) {
        fprintf(f, "%s\n", p->inventory[i]);
    }

    // Save the current room index
    fprintf(f, "CURRENT_ROOM %d\n", current_room_index);

    fclose(f);
    return 0;
}

/* Loads the game state from a file */
int load_game(const char* filepath) {
    FILE* f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open load file %s\n", filepath);
        return -1;
    }

    cleanup_game(); // Clear the current game state
    if (init_game("data/rooms.txt", "data/items.txt", "data/creatures.txt") != 0) {
        fclose(f);
        return -1;
    }

    char buffer[256];
    Player* p = get_player();

    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

        if (strcmp(buffer, "PLAYER") == 0) {
            if (fscanf(f, "%d %d %d\n", &p->health, &p->strength, &p->inventory_count) != 3) {
                fprintf(stderr, "Error: Failed to load player data.\n");
                fclose(f);
                return -1;
            }
            for (int i = 0; i < p->inventory_count; i++) {
                if (!fgets(buffer, sizeof(buffer), f)) {
                    fprintf(stderr, "Error: Failed to load inventory item.\n");
                    fclose(f);
                    return -1;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                snprintf(p->inventory[i], MAX_ITEM_NAME_LEN, "%s", buffer);
            }
        } else if (strncmp(buffer, "CURRENT_ROOM", 12) == 0) {
            int idx;
            if (sscanf(buffer, "CURRENT_ROOM %d", &idx) == 1) {
                set_current_room_index(idx);
            }
        }
    }

    fclose(f);
    return 0;
}

