#ifndef CHAM_KDTREE
#define CHAM_KDTREE
#include "cham_color_proc.h"

Color build_kdtree(uint8_t *pal, int depth, Color *kdtree, int size, int ind);
int search_neighbor(Color org, Palette p);
#endif /* ifndef  CHAM_KDTREE */
