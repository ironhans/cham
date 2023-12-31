#include "cham_color_proc.h"

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

inline int cmp_r(const void *a, const void *b)
{
	return (((int)*(uint8_t *)a) - ((int)*(uint8_t *)b));
}
inline int cmp_g(const void *a, const void *b)
{
	int x = (int)*(((uint8_t *)a) + 1);
	int y = (int)*(((uint8_t *)b) + 1);
	return x - y;
}
inline int cmp_b(const void *a, const void *b)
{
	int x = (int)*(((uint8_t *)a) + 2);
	int y = (int)*(((uint8_t *)b) + 2);
	return x - y;
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

inline void set_color_error(unsigned char *img, int index, double weight,
							Color err)
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
		if (d == ATKINSON) {
			double w = 1.0 / 8;
			int x = GET_X(i, channels, width);
			int y = GET_Y(i, channels, width);
			if (x + 2 < width) {
				set_color_error(img, GET_IND(x + 2, y, channels, width), w,
								quant_err);
			}
			if (x + 1 < width) {
				set_color_error(img, GET_IND(x + 1, y, channels, width), w,
								quant_err);
			}
			if (y + 2 < height) {
				set_color_error(img, GET_IND(x, y + 2, channels, width), w,
								quant_err);
			}
			if (y + 1 < height) {
				set_color_error(img, GET_IND(x, y + 1, channels, width), w,
								quant_err);
				if (x > 0) {
					set_color_error(img, GET_IND(x - 1, y + 1, channels, width),
									w, quant_err);
				}
				if (x + 1 < width) {
					set_color_error(img, GET_IND(x + 1, y + 1, channels, width),
									w, quant_err);
				}
			}
		}
	}
	return pixels;
}

void median_cut(unsigned char *img, ColorBucket *buckets, int curr_buckets,
				int channels, int kcolors)
{
	if (curr_buckets + 1 == kcolors) {
		return;
	}
	int target_index = 0;
	int curr_largest_range = -1;
	for (int i = 0; i <= curr_buckets; i++) {
		if (buckets[i].size <= 1) {
			continue;
		}
		if (buckets[i].color_range != -1 &&
			buckets[i].color_range > curr_largest_range) {
			curr_largest_range = buckets[i].color_range;
			target_index = i;
			continue;
		}
		int r_min = INT_MAX;
		int g_min = INT_MAX;
		int b_min = INT_MAX;
		int r_max = -1;
		int g_max = -1;
		int b_max = -1;
		for (int j = channels * buckets[i].head;
			 j < channels * (buckets[i].head + buckets[i].size);
			 j += channels) {
			// printf("j: %d\n", j);
			if (img[j] > r_max) {
				r_max = img[j];
			}
			if (img[j] < r_min) {
				r_min = img[j];
			}
			if (img[j + 1] > g_max) {
				g_max = img[j + 1];
			}
			if (img[j + 1] < g_min) {
				g_min = img[j + 1];
			}
			if (img[j + 2] > b_max) {
				b_max = img[j + 2];
			}
			if (img[j + 2] < b_min) {
				b_min = img[j + 2];
			}
		}
		int r_range = r_max - r_min;
		int g_range = g_max - g_min;
		int b_range = b_max - b_min;
		int max_range;
		if (g_range >= r_range && g_range >= b_range) {
			max_range = g_range;
			buckets[i].color = 1;
		} else if (r_range >= g_range && r_range >= b_range) {
			max_range = r_range;
			buckets[i].color = 0;
		} else {
			max_range = b_range;
			buckets[i].color = 2;
		}
		buckets[i].color_range = max_range;
		if (curr_largest_range < max_range) {
			target_index = i;
			curr_largest_range = max_range;
		}
	}
	ColorBucket *target_bucket = &buckets[target_index];
	switch (target_bucket->color) {
		case 0:
			qsort(&img[target_bucket->head * channels], target_bucket->size,
				  sizeof(*img) * channels, cmp_r);
			break;
		case 1:
			qsort(&img[target_bucket->head * channels], target_bucket->size,
				  sizeof(*img) * channels, cmp_g);
			break;
		case 2:
			qsort(&img[target_bucket->head * channels], target_bucket->size,
				  sizeof(*img) * channels, cmp_b);
			break;
	}
	int end = target_bucket->head + target_bucket->size;
	int org_size = target_bucket->size;
	buckets[curr_buckets + 1].head = target_bucket->head;
	buckets[curr_buckets + 1].size = org_size / 2;
	target_bucket->head += buckets[curr_buckets + 1].size;
	target_bucket->size = end - target_bucket->head;

	target_bucket->color_range = -1;
	buckets[curr_buckets + 1].color_range = -1;
	median_cut(img, buckets, curr_buckets + 1, channels, kcolors);
}

inline static Color mean(unsigned char *img, ColorBucket *bucket, int ch)
{
	Color c;
	c.r = 0;
	c.g = 0;
	c.b = 0;
	for (int i = ch * bucket->head; i < ch * (bucket->head + bucket->size);
		 i += ch) {
		c.r += img[i];
		c.g += img[i + 1];
		c.b += img[i + 2];
	}
	c.r /= bucket->size;
	c.g /= bucket->size;
	c.b /= bucket->size;
	return c;
}

void generate_pal(unsigned char *img, int width, int height, int channels,
				  int kcolors, Palette *pal)
{
	int whc = width * height * channels;
	unsigned char *img_clone = malloc(sizeof(*img) * whc);
	memcpy(img_clone, img, sizeof(*img) * whc);
	ColorBucket *buckets = malloc(sizeof(*buckets) * kcolors);
	buckets[0].head = 0;
	buckets[0].size = width * height;
	buckets[0].color = -1;
	buckets[0].color_range = -1;
	median_cut(img_clone, buckets, 0, STBI_rgb, kcolors);
	uint8_t *generated_colors =
		malloc(sizeof(*generated_colors) * kcolors * channels);
	for (int i = 0; i < kcolors; i++) {
		Color c = mean(img_clone, &buckets[i], STBI_rgb);
		generated_colors[3 * i] = c.r;
		generated_colors[3 * i + 1] = c.g;
		generated_colors[3 * i + 2] = c.b;
	}
	pal->size = kcolors;
	pal->color_arr = generated_colors;
	if (kcolors > 8) {
		int x = ceil(log2(kcolors));
		uint8_t *kdtree = calloc((1 << x) * channels, sizeof(*kdtree));
		build_kdtree(generated_colors, 0, kdtree, kcolors, 0);
		pal->kdtree = kdtree;
		pal->size = (1 << x);  // NOTE: Bandage fix, no way to properly null
							   // check for kdtree
	}
}
