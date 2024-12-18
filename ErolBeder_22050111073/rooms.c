#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rooms.h"

/*
 * load_rooms:
 * Reads room data from the specified file and dynamically allocates memory.
 * The file format is as follows:
 *  - NUM_ROOMS (number of rooms)
 *  - description (room description)
 *  - up, down, left, right (indices of neighboring rooms)
 *  - item_count (number of items in the room)
 *  - item1, item2, ... (names of the items)
 *  - creature_index (index of the creature in the room)
 */
int load_rooms(const char* filename, Room** rooms, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open rooms file: %s\n", filename);
        return -1;
    }

    int num_rooms;
    if (fscanf(file, "%d\n", &num_rooms) != 1) {
        fprintf(stderr, "Error: Invalid room file format.\n");
        fclose(file);
        return -1;
    }

    Room* room_array = malloc(sizeof(Room) * num_rooms);
    if (!room_array) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return -1;
    }

    // Read rooms from the file
    for (int i = 0; i < num_rooms; i++) {
        // Read the room description
        if (!fgets(room_array[i].description, MAX_ROOM_DESC, file)) {
            fprintf(stderr, "Error: Failed to read room description.\n");
            free(room_array);
            fclose(file);
            return -1;
        }
        // Remove the newline character
        char* newline = strchr(room_array[i].description, '\n');
        if (newline) *newline = '\0';

        // Read indices of neighboring rooms
        if (fscanf(file, "%d %d %d %d\n", 
                   &room_array[i].up, 
                   &room_array[i].down, 
                   &room_array[i].left, 
                   &room_array[i].right) != 4) {
            fprintf(stderr, "Error: Failed to read room connections.\n");
            free(room_array);
            fclose(file);
            return -1;
        }

        // Read the number of items
        if (fscanf(file, "%d\n", &room_array[i].item_count) != 1) {
            fprintf(stderr, "Error: Failed to read item count.\n");
            free(room_array);
            fclose(file);
            return -1;
        }

        // Read the items in the room
        for (int j = 0; j < room_array[i].item_count; j++) {
            if (!fgets(room_array[i].items[j], 64, file)) {
                fprintf(stderr, "Error: Failed to read item name.\n");
                free(room_array);
                fclose(file);
                return -1;
            }
            // Remove the newline character
            newline = strchr(room_array[i].items[j], '\n');
            if (newline) *newline = '\0';
        }

        // Read the creature index
        if (fscanf(file, "%d\n", &room_array[i].creature_index) != 1) {
            fprintf(stderr, "Error: Failed to read creature index.\n");
            free(room_array);
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    *rooms = room_array;
    *count = num_rooms;

    return 0; // Success
}

/*
 * free_rooms:
 * Frees the dynamically allocated memory for rooms.
 */
void free_rooms(Room* rooms, int count) {
    (void)count; // To indicate that the parameter is unused
    if (rooms) {
        free(rooms);
    }
}

