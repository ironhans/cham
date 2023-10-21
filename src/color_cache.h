#ifndef CHAM_COLOR_CACHE
#define CHAM_COLOR_CACHE
#include <stdint.h>

#include "cham_palettes_predefined.h"

typedef struct ColorHashEntry {
	int key;
	int index;
	struct ColorHashEntry *next;
	struct ColorHashEntry *prev;
} ColorHashEntry;

typedef struct {
	uint8_t len;
	uint8_t cap;
	ColorHashEntry *map;
	ColorHashEntry *queue;
	ColorHashEntry *tail;
} ColorCache;

ColorCache create_color_cache(uint8_t size);
ColorHashEntry *get_color(ColorCache *cc, Color c);
int insert_color(ColorCache *cc);
void put_color(ColorCache *cc, Color c, int index);

#endif /* ifndef CHAM_COLOR_CACHE */
