#ifndef CHAM_KDTREE
#define CHAM_KDTREE
#include "cham_color_proc.h"

typedef struct {
	double dist;
	int ind;
} KDTree_Return;

Color build_kdtree(uint8_t *pal, int depth, uint8_t *kdtree, int size, int ind);
int search_neighbor(Color org, Palette p);
#endif /* ifndef  CHAM_KDTREE */
