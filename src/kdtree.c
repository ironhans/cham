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

int visited;

static void search_recursive(Color o, Palette p, int i, int depth, KDTree_Return *best)
{
	if (i >= (p.size * 3)) {
		// return best;
		return;
	}
	visited++;
	Color root_color;
	int axis = depth % 3;
	root_color.r = p.kdtree[i];
	root_color.g = p.kdtree[i + 1];
	root_color.b = p.kdtree[i + 2];
	// switch (axis) {
	// 	case 0:
	// 		if (o.r > root_color.r) {
	// 			int temp = taken;
	// 			taken = other;
	// 			other = temp;
	// 		}
	// 		break;
	// 	case 1:
	// 		if (o.g > root_color.g) {
	// 			int temp = taken;
	// 			taken = other;
	// 			other = temp;
	// 		}
	// 		break;
	// 	case 2:
	// 		if (o.b > root_color.b) {
	// 			int temp = taken;
	// 			taken = other;
	// 			other = temp;
	// 		}
	// 		break;
	// }
	KDTree_Return curr = {distance_between_colors(o, root_color), i};
	// if (curr.dist == 0) {
	// 	return curr;
	// }
	// KDTree_Return found = search_recursive(o, p, taken, depth + 1, curr);
	// KDTree_Return best = curr;
	if (curr.dist < best->dist) {
		best->dist = curr.dist;
		best->ind = curr.ind;
	}
	double dist_axis;
	int taken = 2 * i + 3; // default left
	int other = 2 * i + 6; // default right
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
	if (dist_axis < 0) {
		int temp = taken;
		taken = other;
		other = temp;
	}
	// printf("BEST %f\n", best->dist);
	// printf("BEST IND %d\n", best->ind);
	// Color b;
	// b.r = p.kdtree[best.ind];
	// b.g = p.kdtree[best.ind + 1];
	// b.b = p.kdtree[best.ind + 2];
	// UNUSED(b);
	// switch (axis) {
	// 	case 0:
	// 		dist_axis = b.r - c.r;
	// 		break;
	// 	case 1:
	// 		dist_axis = b.g - c.g;
	// 		break;
	// 	case 2:
	// 		dist_axis = b.b - c.b;
	// 		break;
	// }
	search_recursive(o, p, taken, depth + 1, best);
	// if (best.dist >= fabs(dist_axis)) { // correct but wrongs much slower than even linear
	if (best->dist > dist_axis * dist_axis) { // faster but incorrect
		search_recursive(o, p, other, depth + 1, best);
		// KDTree_Return found = search_recursive(o, p, other, depth + 1, best);
		// if (found.dist < best.dist) {
		// 	best = found;
		// }
	}
	// return best;
}

int search_neighbor(Color org, Palette p)
{
	// KDTree_Return start = {INT_MAX, 0};
	KDTree_Return val = {INT_MAX, 0};
	visited = 0;
	// KDTree_Return val = search_recursive(org, p, 0, 0, start);
	search_recursive(org, p, 0, 0, &val);
	// printf("final dist: %f\n", val.dist);
	// printf("final index: %d\n", val.ind / 3);
	printf("visited: %d\n\n", visited);
	visited = 0;
	// printf("DIST %f\n", val.dist);
	return val.ind;
	// return search_recursive(org, p, 0, 0, start).ind / 3;
	// UNUSED(search_recursive);
	// int index = 0;
	// double min = INT_MAX;
	// for (int i = 0; i < p.size * 3; i += 3) {
	// 	Color c;
	// 	c.r = p.kdtree[i];
	// 	c.g = p.kdtree[i + 1];
	// 	c.b = p.kdtree[i + 2];
	// 	double curr = distance_between_colors(org, c);
	// 	if (curr < min) {
	// 		min = curr;
	// 		index = i / 3;
	// 		if (curr == 0) {
	// 			printf("DIST %f\n", min);
	// 			return index;
	// 		}
	// 	}
	// }
	// printf("DIST %f\n", min);
	// return index;
	// int i = 0;
	// int depth = 0;
	// int min_ind = 0;
	// double min = INT_MAX;
	// while (i < (p.size * 3)) {
	// 	Color t;
	// 	int axis = depth % 3;
	// 	t.r = p.kdtree[i];
	// 	t.g = p.kdtree[i + 1];
	// 	t.b = p.kdtree[i + 2];
	// 	double curr = distance_between_colors(org, t);
	// 	if (curr == 0) {
	// 		return i / 3;
	// 	}
	// 	if (curr < min) {
	// 		min = curr;
	// 		min_ind = i;
	// 	}
	// 	int right = 0;
	// 	switch (axis) {
	// 		case 0:
	// 			if (org.r > t.r) {
	// 				right = 1;
	// 			}
	// 			break;
	// 		case 1:
	// 			if (org.g > t.g) {
	// 				right = 1;
	// 			}
	// 			break;
	// 		case 2:
	// 			if (org.b > t.b) {
	// 				right = 1;
	// 			}
	// 			break;
	// 	}
	// 	if (right) {
	// 		i = 2 * i + 6;
	// 	} else {
	// 		i = 2 * i + 3;
	// 	}
	// 	depth++;
	// }
	// return min_ind / 3;
}
