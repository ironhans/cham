#ifndef CHAM_COLOR_CACHE
#define CHAM_COLOR_CACHE
#include <stdint.h>

#include "cham_palettes_predefined.h"

typedef struct {
	int key;
	int index;
} ColorHashEntry;

typedef struct {
	uint8_t len;
	uint8_t cap;
	ColorHashEntry *map;
	int *queue;
} ColorCache;

ColorCache create_color_cache(uint8_t size);
void insert_color(ColorCache cc);

#endif /* ifndef CHAM_COLOR_CACHE */
