#ifndef ROOMS_H
#define ROOMS_H

// Constants
#define MAX_ROOM_DESC 256      // Maximum number of characters for room description
#define MAX_ROOM_ITEMS 5       // Maximum number of items that can be in a room
#define MAX_ITEM_NAME_LEN 64   // Maximum length for item names

typedef struct Room {
    char description[MAX_ROOM_DESC];                // Room description
    int up;                                         // Index of the room above
    int down;                                       // Index of the room below
    int left;                                       // Index of the room to the left
    int right;                                      // Index of the room to the right

    char items[MAX_ROOM_ITEMS][MAX_ITEM_NAME_LEN];  // Items in the room
    int item_count;                                 // Number of items in the room

    int creature_index;                             // Creature index (-1: None)
} Room;

/*
 * load_rooms:
 * Reads room data from a file and loads the rooms using dynamic memory allocation.
 * Parameters:
 *   - filename: Path to the file containing room data.
 *   - rooms: Pointer to hold the loaded rooms (output parameter).
 *   - count: Pointer to hold the number of loaded rooms (output parameter).
 * Return Value:
 *   - 0: Success
 *   - -1: Error
 */
int load_rooms(const char* filename, Room** rooms, int* count);

/*
 * free_rooms:
 * Frees the dynamically loaded room data.
 * Parameters:
 *   - rooms: Array of rooms to be freed.
 *   - count: Number of rooms (currently unused, present for compatibility).
 */
void free_rooms(Room* rooms, int count);

#endif // ROOMS_H
