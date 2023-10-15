#include "kdtree.h"

#include <limits.h>
#include <stdlib.h>

#include "cham_color_proc.h"

static int cmp_r(const void *a, const void *b)
{
	return (((int)*(uint8_t *)a) - ((int)*(uint8_t *)b));
}
static int cmp_g(const void *a, const void *b)
{
	int x = (int)*(((uint8_t *)a) + 1);
	int y = (int)*(((uint8_t *)b) + 1);
	return x - y;
}
static int cmp_b(const void *a, const void *b)
{
	int x = (int)*(((uint8_t *)a) + 2);
	int y = (int)*(((uint8_t *)b) + 2);
	return x - y;
}

Color build_kdtree(uint8_t *pal, int depth, Color *kdtree, int size, int ind)
{
	int axis = depth % 3;
	if (size <= 1) {
		Color c;
		c.r = *pal;
		c.g = *(pal + 1);
		c.b = *(pal + 2);
		return c;
	}
	switch (axis) {
		case 0:
			qsort(pal, size, (sizeof(*pal) * 3), cmp_r);
			break;
		case 1:
			qsort(pal, size, (sizeof(*pal) * 3), cmp_g);
			break;
		case 2:
			qsort(pal, size, (sizeof(*pal) * 3), cmp_b);
			break;
	}
	Color median;
	int mid = 3 * (size / 2);
	median.r = pal[mid];
	median.g = pal[mid + 1];
	median.b = pal[mid + 2];
	kdtree[ind] = median;
	int left_size = (mid / 3);
	int right_size = size - (mid / 3) - 1;
	if (left_size >= 1) {
		kdtree[2 * ind + 1] =
			build_kdtree(pal, depth + 1, kdtree, left_size, 2 * ind + 1);
	}
	if (right_size >= 1) {
		kdtree[2 * ind + 2] = build_kdtree(pal + mid + 3, depth + 1, kdtree,
										   right_size, 2 * ind + 2);
	}
	return median;
}

// static int search_recursive(Color o, Color *kdtree, int size, int i, int depth,
// 							int min)
// {
// 	UNUSED(kdtree);
// 	if (i > size) {
// 		return INT_MAX;
// 	}
// 	Color p = kdtree[i];
// 	if (o.r == p.r && o.g == p.g && o.b == p.b) {
// 		return i;
// 	}
// }
int search_neighbor(Color org, Palette p)
{
	UNUSED(org);
	UNUSED(p);
	return 1;
	// return search_recursive(org, p.kdtree, p.size, 0, 0, INT_MAX);
}
