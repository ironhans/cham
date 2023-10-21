#include "color_cache.h"

#include <stdlib.h>

#include "cham_color_proc.h"

// NOTE: Unfinished
// inline static int hash_color(Color c) { return c.r + 2 * c.g + 3 * c.b; }
// 
// inline static void update_qindex(ColorCache *cc, ColorHashEntry *lookup)
// {
// 	if (!lookup->prev) {
// 		return;
// 	}
// 	ColorHashEntry *prev = lookup->prev;
// 	ColorHashEntry *next = lookup->next;
// 	prev->next = next;
// 	if (next) {
// 		next->prev = prev;
// 	} else {
// 		cc->tail = prev;
// 	}
// 	cc->queue = lookup;
// }
// 
// inline static void pop(ColorCache *cc)
// {
// 	if (!cc->tail->prev) {
// 		cc->len = 0;
// 		return;
// 	}
// 	cc->len--;
// 	cc->tail = cc->tail->prev;
// 	cc->tail->next = NULL;
// }
// 
// 
// ColorCache create_color_cache(uint8_t cap)
// {
// 	ColorCache cc;
// 	cc.len = 0;
// 	cc.cap = cap;
// 	cc.map = malloc(cc.cap * sizeof(*cc.map));
// 	cc.queue = malloc(cc.cap * sizeof(cc.map->key));
// 	cc.tail = NULL;
// 	return cc;
// }
// 
// // returns null on miss
// ColorHashEntry *get_color(ColorCache *cc, Color c)
// {
// 	int hash_value = hash_color(c);
// 	ColorHashEntry *lookup = &cc->map[hash_value % cc->cap];
// 	if (lookup->key == hash_value) {
// 		// hash map hit :D
// 		return lookup;
// 	}
// 	update_qindex(cc, lookup);
// 	// TODO:
// 	// handling hash miss
// 	return NULL;
// }
// 
// void put_color(ColorCache *cc, Color c, int index)
// {
// 	int hash_value = hash_color(c);
// 	if (get_color(cc, c)->index != -1) {
// 		// queue does not contain entry
// 		if (cc->len == cc->cap) {
// 			pop(cc);
// 		} else if (cc->len == 0) {
// 			ColorHashEntry start = {hash_value, index, NULL, NULL};
// 			cc->queue = &start;
// 		} else {
// 			ColorHashEntry new = {hash_value, index, NULL, NULL};
// 			cc->tail->next = &new;
// 			cc->tail = &new;
// 		}
// 	}
// 	// queue already contains entry
// 	// update_qindex(cc, &cc.map[hash_value % cc.cap]);
// 	// ColorHashEntry new_entry = {hash_value, index, 0};
// }

