#include "cham_color_proc.h"

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cham_palettes_predefined.h"
#include "kdtree.h"
#include "stb_image.h"

inline double distance_between_colors(Color x, Color y)
{
	int r_diff = y.r - x.r;
	int g_diff = y.g - x.g;
	int b_diff = y.b - x.b;
	return ((r_diff * r_diff) + (g_diff * g_diff) + (b_diff * b_diff));
}

inline double sq_distance_between_colors(Color x, Color y)
{
	int r_diff = y.r - x.r;
	int g_diff = y.g - x.g;
	int b_diff = y.b - x.b;
	return sqrt((r_diff * r_diff) + (g_diff * g_diff) + (b_diff * b_diff));
}

int find_closest_color(Color original, Palette p)
{
	int index = 0;
	double min = INT_MAX;
	for (int i = 0; i < p.size * 3; i += 3) {
		Color c;
		c.r = p.color_arr[i];
		c.g = p.color_arr[i + 1];
		c.b = p.color_arr[i + 2];
		double curr = distance_between_colors(original, c);
		if (curr < min) {
			min = curr;
			index = i;
			if (curr == 0) {
				return index;
			}
		}
	}
	return index;
}

inline Color quant_error(Color old_color, Color new_color)
{
	Color t;
	t.r = (old_color.r - new_color.r);
	t.g = (old_color.g - new_color.g);
	t.b = (old_color.b - new_color.b);
	return t;
}

inline void set_color_error(unsigned char *img, int index, double weight, Color err)
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
	// uint8_t *kdtree = malloc(sizeof(*kdtree) * pal.size * channels);
	// build_kdtree(pal.color_arr, 0, kdtree, pal.size, 0);
	// printf("START\n");
	// for (int i = 0; i < pal.size * 3; i += 3) {
	// 	printf("0x%02x, 0x%02x, 0x%02x,\n", kdtree[i], kdtree[i + 1],
	// 		   kdtree[i + 2]);
	// }
	// printf("STOP\n");
	uint8_t *pixels = malloc(sizeof(*pixels) * width * height);
	int (*find_func)(Color, Palette) =
		(pal.kdtree) ? &search_neighbor : &find_closest_color;
	// if (pal.kdtree) {
	// 	printf("KDTREE\n");
	// } else {
	// 	printf("LINEAR\n");
	// }
	for (int i = 0; i < width * height * channels; i += channels) {
		Color c;
		c.r = img[i];
		c.g = img[i + 1];
		c.b = img[i + 2];
		pixels[i / channels] = (*find_func)(c, pal) / 3;
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
	uint8_t *color_source; 
	int (*find_func)(Color, Palette);
	if (pal.kdtree) {
		find_func = &search_neighbor;
		color_source = pal.kdtree;
	} else {
		find_func = &find_closest_color;
		color_source = pal.color_arr;
	}
	for (int i = 0; i < width * height * channels; i += channels) {
		Color c;
		c.r = img[i];
		c.g = img[i + 1];
		c.b = img[i + 2];
		int index = (*find_func)(c, pal);
		pixels[i / channels] = index / 3;
		Color p;
		p.r = color_source[index];
		p.g = color_source[index + 1];
		p.b = color_source[index + 2];
		Color quant_err = quant_error(c, p);
		// TODO: Remove this if condition and try a kernel data struct
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
