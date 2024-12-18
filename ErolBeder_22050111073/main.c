#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#include "commands.h"

int main() {
    // Initialize the game (load data files)
    if (init_game("data/rooms.txt", "data/items.txt", "data/creatures.txt") != 0) {
        fprintf(stderr, "Error: Unable to load game data. Exiting...\n");
        return EXIT_FAILURE;
    }

    printf("=== Welcome to the Dark Dungeon ===\n");
    printf("Type 'help' to see available commands.\n\n");

    char input[256];

    // Game loop
    while (!game_over()) {
        printf("> "); // Command prompt
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nGoodbye!\n"); // Handle EOF or input errors
            break;
        }

        // Remove the newline character from the input
        char *newline = strchr(input, '\n');
        if (newline) *newline = '\0';

        // Skip empty commands
        if (strlen(input) == 0) {
            continue;
        }

        // Process the command
        handle_command(input);
    }

    printf("\nGame Over. Thank you for playing!\n");

    // Cleanup
    cleanup_game();
    return EXIT_SUCCESS;
}
