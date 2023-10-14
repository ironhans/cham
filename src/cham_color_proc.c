#include "cham_color_proc.h"

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cham_palettes_predefined.h"
#include "stb_image.h"

double distance_between_colors(Color x, Color y)
{
	int r_diff = y.r - x.r;
	int g_diff = y.g - x.g;
	int b_diff = y.b - x.b;
	return ((r_diff * r_diff) + (g_diff * g_diff) + (b_diff * b_diff));
}

double sq_distance_between_colors(Color x, Color y)
{
	return sqrt((pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
				(pow((y.b - x.b), 2)));
}

double distance_between_colors_a(ColorAlpha x, ColorAlpha y)
{
	return (pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
		   (pow((y.b - x.b), 2));
}

double sq_distance_between_colors_a(ColorAlpha x, ColorAlpha y)
{
	return sqrt((pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
				(pow((y.b - x.b), 2)));
}

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

int find_closest_color(Color original, Palette p)
{
	int index = 0;
	double min = INT_MAX;
	for (int i = 0; i < p.size * 3; i += 3) {
		Color c;
		c.r = p.palette[i];
		c.g = p.palette[i + 1];
		c.b = p.palette[i + 2];
		double curr = distance_between_colors(original, c);
		if (curr < min) {
			min = curr;
			index = i / 3;
			if (curr == 0) {
				return index;
			}
		}
	}
	return index;
}

Color quant_error(Color old_color, Color new_color)
{
	Color t;
	t.r = (old_color.r - new_color.r);
	t.g = (old_color.g - new_color.g);
	t.b = (old_color.b - new_color.b);
	return t;
}

void set_color_error(unsigned char *img, int index, double weight, Color err)
{
	PLUS_TRUNCATE_UCHAR(img[index], weight * err.r);
	PLUS_TRUNCATE_UCHAR(img[index + 1], weight * err.g);
	PLUS_TRUNCATE_UCHAR(img[index + 2], weight * err.b);
}

uint8_t *cham_create_given_palette(Palette pal, unsigned char *img, int width,
								   int height, int channels)
{
	if (channels == 4) {
		return NULL;
	}
	Color *kdtree = malloc(sizeof(*kdtree) * pal.size);
	build_kdtree(pal.palette, 0, kdtree, pal.size, 0);
	// printf("START\n");
	// for (int i = 0; i < pal.size; i++) {
	// 	printf("0x%02x, 0x%02x, 0x%02x\n", kdtree[i].r, kdtree[i].g,
	// 		   kdtree[i].b);
	// }
	// printf("STOP\n");
	uint8_t *pixels = malloc(sizeof(*pixels) * width * height);
	// TODO: Optimize this, it runs poorly
	// TREE Structure needed
	for (int i = 0; i < width * height * channels; i += channels) {
		Color c;
		c.r = img[i];
		c.g = img[i + 1];
		c.b = img[i + 2];
		pixels[i / channels] = find_closest_color(c, pal);
	}
	return pixels;
}

uint8_t *cham_create_given_palette_d(Palette pal, unsigned char *img, int width,
									 int height, int channels,
									 DitherAlgorithm d)
{
	if (channels == 4) {
		return NULL;
	}
	uint8_t *pixels = malloc(sizeof(*pixels) * width * height);
	// TODO: Optimize this, it runs poorly
	// TREE Structure needed
	for (int i = 0; i < width * height * channels; i += channels) {
		Color c;
		c.r = img[i];
		c.g = img[i + 1];
		c.b = img[i + 2];
		int index = find_closest_color(c, pal);
		pixels[i / channels] = index;
		Color p;
		p.r = pal.palette[index * 3];
		p.g = pal.palette[index * 3 + 1];
		p.b = pal.palette[index * 3 + 2];
		Color quant_err = quant_error(c, p);
		if (d == FLOYD_STEINBERG) {
			double w1 = 7.0 / 16;
			double w2 = 5.0 / 16;
			double w3 = 3.0 / 16;
			double w4 = 1.0 / 16;
			int x = GET_X(i, channels, width);
			int y = GET_Y(i, channels, width);
			if (x + 1 < width) {
				set_color_error(img, GET_IND(x + 1, y, channels, width), w1,
								quant_err);
			}
			if (y + 1 < height) {
				set_color_error(img, GET_IND(x, y + 1, channels, width), w2,
								quant_err);
				if (x > 0) {
					set_color_error(img, GET_IND(x - 1, y + 1, channels, width),
									w3, quant_err);
				}
				if (x + 1 < width) {
					set_color_error(img, GET_IND(x + 1, y + 1, channels, width),
									w4, quant_err);
				}
			}
		}
	}
	return pixels;
}

uint8_t *cham_create_input_palette_kcolors(unsigned char *img, int kcolors,
										   int width, int height, int channels)
{
	if (channels == 4) {
		return NULL;
	}
	UNUSED(kcolors);
	Color *pixels = malloc(sizeof(*pixels) * width * height);
	int count = 0;
	for (int i = 0; i < width * height * channels; i += channels) {
		pixels[count].r = img[i];
		pixels[count].g = img[i + 1];
		pixels[count].b = img[i + 2];
		count++;
	}
	// uint8_t num_colors = ceil(sqrt(kcolors));
	uint8_t *result = malloc(sizeof(*result) * width * height);
	return result;
}
