#include "kdtree.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
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

Color build_kdtree(uint8_t *pal, int depth, uint8_t *kdtree, int size, int ind)
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
	kdtree[ind] = median.r;
	kdtree[ind + 1] = median.g;
	kdtree[ind + 2] = median.b;
	int left_size = (mid / 3);
	int right_size = size - (mid / 3) - 1;
	if (left_size >= 1) {
		int l_ind = 2 * ind + 3;
		Color l = build_kdtree(pal, depth + 1, kdtree, left_size, l_ind);
		kdtree[l_ind] = l.r;
		kdtree[l_ind + 1] = l.g;
		kdtree[l_ind + 2] = l.b;
	}
	if (right_size >= 1) {
		int r_ind = 2 * ind + 6;
		Color r =
			build_kdtree(pal + mid + 3, depth + 1, kdtree, right_size, r_ind);
		kdtree[r_ind] = r.r;
		kdtree[r_ind + 1] = r.g;
		kdtree[r_ind + 2] = r.b;
	}
	return median;
}

static void search_recursive(Color o, Palette p, int i, int depth, KDTree_Return *best)
{
	if (i >= (p.size * 3)) {
		return;
	}
	Color root_color;
	int axis = depth % 3;
	root_color.r = p.kdtree[i];
	root_color.g = p.kdtree[i + 1];
	root_color.b = p.kdtree[i + 2];
	KDTree_Return curr = {distance_between_colors(o, root_color), i};
	if (curr.dist < best->dist) {
		best->dist = curr.dist;
		best->ind = curr.ind;
	}
	double dist_axis;
	switch (axis) {
		case 0:
			dist_axis = root_color.r - o.r;
			break;
		case 1:
			dist_axis = root_color.g - o.g;
			break;
		case 2:
			dist_axis = root_color.b - o.b;
			break;
	}
	int taken = 2 * i + 3; // default left
	int other = 2 * i + 6; // default right
	if (dist_axis < 0) {
		int temp = taken;
		taken = other;
		other = temp;
	}
	search_recursive(o, p, taken, depth + 1, best);
	if (best->dist > dist_axis * dist_axis) {
		search_recursive(o, p, other, depth + 1, best);
	}
	return;
}

int search_neighbor(Color org, Palette p)
{
	KDTree_Return val = {INT_MAX, 0};
	search_recursive(org, p, 0, 0, &val);
	return val.ind;
}
