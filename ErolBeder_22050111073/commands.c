#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "game.h"
#include "player.h"
#include "rooms.h"
#include "creatures.h"

static void cmd_help() {
    printf("Available commands:\n");
    printf("  move <direction>\n");
    printf("  look\n");
    printf("  inventory\n");
    printf("  pickup <item>\n");
    printf("  attack\n");
    printf("  list\n");
    printf("  save <filepath>\n");
    printf("  load <filepath>\n");
    printf("  exit\n");
}

static void cmd_move(char* direction) {
    Room* current_room = get_current_room();
    if (!current_room) {
        printf("You are nowhere! Cannot move.\n");
        return;
    }

    int next_room_index = -1;

    // Determine the next room based on the direction
    if (strcmp(direction, "up") == 0) {
        next_room_index = current_room->up;
    } else if (strcmp(direction, "down") == 0) {
        next_room_index = current_room->down;
    } else if (strcmp(direction, "left") == 0) {
        next_room_index = current_room->left;
    } else if (strcmp(direction, "right") == 0) {
        next_room_index = current_room->right;
    } else {
        printf("Invalid direction. Use 'up', 'down', 'left', or 'right'.\n");
        return;
    }

    // Check if the direction is valid (i.e., the next room exists)
    if (next_room_index < 0) {
        printf("You can't go that way.\n");
        return;
    }

    // Set the player's current room to the new room
    set_current_room_index(next_room_index);
    printf("You move %s.\n", direction);
}

static void cmd_look() {
    Room* r = get_current_room();
    if (!r) {
        printf("You see nothing...\n");
        return;
    }

    printf("Room: %s\n", r->description);

    if (r->item_count > 0) {
        printf("Items here:\n");
        for (int i = 0; i < r->item_count; i++) {
            printf(" - %s\n", r->items[i]);
        }
    }

    if (r->creature_index >= 0) {
        Creature* c = get_creature(r->creature_index);
        if (c && c->health > 0) {
            printf("A %s is here, looking dangerous.\n", c->name);
        }
    }
}


static void cmd_inventory() {
    Player* p = get_player();

    if (p->inventory_count == 0) {
        printf("Your inventory is empty.\n");
        return;
    }

    printf("You carry:\n");
    for (int i = 0; i < p->inventory_count; i++) {
        printf(" - %s\n", p->inventory[i]);
    }
}


static void cmd_pickup(char* item_name) {
    Room* r = get_current_room();
    Player* p = get_player();

    if (!r || !item_name) return;

    // Item control
    int found = -1;
    for (int i = 0; i < r->item_count; i++) {
        if (strcmp(r->items[i], item_name) == 0) {
            found = i;
            break;
        }
    }

    if (found < 0) {
        printf("No such item here.\n");
        return;
    }

    // Add to player inventory
    if (add_item_to_player(p, item_name) != 0) {
        printf("Your inventory is full.\n");
        return;
    }

    // Remove item from room
    memmove(&r->items[found], &r->items[found + 1], 
            (r->item_count - found - 1) * sizeof(r->items[0]));
    r->item_count--;

    printf("You picked up %s.\n", item_name);
}

static void cmd_attack() {
    Room* r = get_current_room();
    Player* p = get_player();
    if (!r || r->creature_index < 0) {
        printf("There's nothing to attack here.\n");
        return;
    }

    Creature* c = get_creature(r->creature_index);
    if (!c || c->health <= 0) {
        printf("There's nothing to attack here.\n");
        return;
    }

    // Simple turn-based fight until someone dies
    while (p->health > 0 && c->health > 0) {
        // Player hits creature
        c->health -= p->strength;
        if (c->health <= 0) {
            printf("You defeated the %s!\n", c->name);
            break;
        }
        // Creature hits player
        p->health -= c->strength;
        if (p->health <= 0) {
            printf("The %s defeated you...\n", c->name);
            break;
        }
    }
}

static void cmd_list() {
    // Dummy: just print a hardcoded line
    // In a real scenario, list saved files from a directory
    printf("Saved games:\n");
    printf("  - save1.dat\n");
    printf("  - save2.dat\n");
}

static void cmd_save(char* filepath) {
    if (!filepath) {
        printf("Usage: save <filepath>\n");
        return;
    }
    int result = save_game(filepath);
    if (result == 0) {
        printf("Game saved successfully to %s.\n", filepath);
    } else {
        perror("Failed to save game");
    }
}

static void cmd_load(char* filepath) {
    if (!filepath) {
        printf("Usage: load <filepath>\n");
        return;
    }
    int result = load_game(filepath);
    if (result == 0) {
        printf("Game loaded successfully from %s.\n", filepath);
    } else {
        perror("Failed to load game");
    }
}

void handle_command(const char* line) {
    char cmd[64] = {0}, arg[128] = {0};

    // Separate command and argument
    if (sscanf(line, "%63s %127[^\n]", cmd, arg) < 1) {
        printf("Unknown command. Type 'help' to see available commands.\n");
        return;
    }

    // One-word commands
    if (strcmp(cmd, "help") == 0)       { cmd_help(); return; }
    if (strcmp(cmd, "look") == 0)       { cmd_look(); return; }
    if (strcmp(cmd, "inventory") == 0)  { cmd_inventory(); return; }
    if (strcmp(cmd, "attack") == 0)     { cmd_attack(); return; }
    if (strcmp(cmd, "list") == 0)       { cmd_list(); return; }
    if (strcmp(cmd, "exit") == 0)       { get_player()->health = 0; return; }

    // Commands that require arguments
    if (strcmp(cmd, "move") == 0)       { cmd_move(arg); return; }
    if (strcmp(cmd, "pickup") == 0)     { cmd_pickup(arg); return; }
    if (strcmp(cmd, "save") == 0)       { cmd_save(arg); return; }
    if (strcmp(cmd, "load") == 0)       { cmd_load(arg); return; }

    // Unknown command
    printf("Unknown command. Type 'help' to see available commands.\n");
}

