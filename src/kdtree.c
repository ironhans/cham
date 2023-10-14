#include <stdlib.h>
#include "kdtree.h"

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
	// printf("ENTER %d\n", depth);
	// printf("CURR IND %d\n", curr_ind);
	// printf("SIZE %d\n", size);
	int axis = depth % 3;
	if (size <= 1) {
		Color c;
		c.r = *pal;
		c.g = *(pal + 1);
		c.b = *(pal + 2);
		// printf("MEDIAN COLOR: 0x%02x 0x%02x 0x%02x\n", c.r, c.g, c.b);
		// printf("0/1 CASE\n\n");
		return c;
	}
	// if (size != 256) {
	// printf("Before sorting by %d\n", axis);
	// for (int i = 0; i < size * 3; i += 3) {
	// 	printf("0x%x 0x%x 0x%x\n", palette[i], palette[i + 1], palette[i + 2]);
	// }
	// }
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
	// printf("After sorting by %d\n", axis);
	// for (int i = 0; i < size * 3; i += 3) {
	// 	printf("0x%02x 0x%02x 0x%02x\n", palette[i], palette[i + 1], palette[i +
	// 2]);
	// }
	Color median;
	int mid = 3 * (size / 2);
	median.r = pal[mid];
	median.g = pal[mid + 1];
	median.b = pal[mid + 2];
	// printf("MEDIAN COLOR: 0x%02x 0x%02x 0x%02x\n", median.r, median.g,
	// 	   median.b);
	// printf("MEDIAN IND %d\n\n", mid);
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
	// construct_kdtree(palette, depth + 1, kdtree, left_size, 2 * curr_ind +
	// 1); construct_kdtree(palette + median_ind + 3, depth + 1, kdtree,
	// right_size, 				 2 * curr_ind + 2);
	return median;
}

