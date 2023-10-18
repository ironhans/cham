#include "color_cache.h"

#include <stdlib.h>

#include "cham_color_proc.h"

inline static int hash_color(Color c) { return c.r + 2 * c.g + 3 * c.b; }

ColorCache create_color_cache(uint8_t cap)
{
	ColorCache cc;
	cc.len = 0;
	cc.cap = cap;
	cc.map = malloc(cc.cap * sizeof(*cc.map));
	cc.queue = malloc(cc.cap * sizeof(cc.map->key));
	return cc;
}

void get_color(ColorCache cc, Color c)
{
	int hash_value = hash_color(c);
	UNUSED(hash_value);
	UNUSED(cc);
}
