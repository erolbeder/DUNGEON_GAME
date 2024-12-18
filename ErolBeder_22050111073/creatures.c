#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatures.h"

// Static variables
static Creature* creatures = NULL;
static int creature_count = 0;

//Dosyadan yaratıkları yükler 
int load_creatures(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }

    // Read the number of creatures
    if (fscanf(f, "%d\n", &creature_count) != 1) {
        fprintf(stderr, "Error: Failed to read creature count.\n");
        goto error_close_file;
    }

    // Allocate memory
    creatures = malloc(sizeof(Creature) * creature_count);
    if (!creatures) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        goto error_close_file;
    }

    // Load creatures
    for (int i = 0; i < creature_count; i++) {
        char buf[64] = {0};

        // Read name
        if (!fgets(buf, sizeof(buf), f)) {
            fprintf(stderr, "Error: Failed to read creature name.\n");
            goto error_free_memory;
        }
        buf[strcspn(buf, "\n")] = '\0'; // Remove newline
        snprintf(creatures[i].name, sizeof(creatures[i].name), "%s", buf);

        // Read health and strength values
        if (fscanf(f, "%d %d\n", &creatures[i].health, &creatures[i].strength) != 2) {
            fprintf(stderr, "Error: Failed to read health and strength.\n");
            goto error_free_memory;
        }
    }

    fclose(f);
    return 0;

error_free_memory:
    free(creatures);
    creatures = NULL;

error_close_file:
    fclose(f);
    return -1;
}

/* Clears memory and resets creature count */
void free_creatures() {
    if (creatures) {
        free(creatures);
        creatures = NULL;
    }
    creature_count = 0;
}

//Returns the creature at a given index 
Creature* get_creature(int index) {
    if (index < 0 || index >= creature_count) {
        fprintf(stderr, "Error: Creature index %d is out of bounds.\n", index);
        return NULL;
    }
    return &creatures[index];
}
