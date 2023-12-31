#ifndef CHAM_COLOR_PROC
#define CHAM_COLOR_PROC
#include <stdint.h>

#include "cham_palettes_predefined.h"

#define UNUSED(param) ((void)((param)))
#define GET_X(index, channels, width) ((index / channels) % width)
#define GET_Y(index, channels, width) ((index / channels) / width)
#define GET_IND(x, y, channels, width) (channels * ((y)*width + (x)))
#define PLUS_TRUNCATE_UCHAR(a, b)    \
	if (((int)(a)) + (b) < 0)        \
		(a) = 0;                     \
	else if (((int)(a)) + (b) > 255) \
		(a) = 255;                   \
	else                             \
		(a) += (b);

typedef enum {
	NONE,
	FLOYD_STEINBERG,
	ATKINSON,
} DitherAlgorithm;

typedef struct {
	int head;
	int size;
	int color;
	int color_range;
} ColorBucket;

double sq_distance_between_colors(Color x, Color y);
double distance_between_colors(Color x, Color y);
double sq_distance_between_colors_a(ColorAlpha x, ColorAlpha y);
double distance_between_colors_a(ColorAlpha x, ColorAlpha y);

int cmp_r(const void *a, const void *b);
int cmp_g(const void *a, const void *b);
int cmp_b(const void *a, const void *b);

int find_closest_color(Color original, Palette p);
Color quant_error(Color old_color, Color new_color);
void set_color_error(unsigned char *img, int index, double weight, Color err);
Color get_color_error(unsigned char *img, int index, double weight, Color err);
void set_pixel_color(unsigned char *img, int index, Color c);

// void median_cut(unsigned char *img, ColorBucket *buckets, int curr_buckets,
// 				int kcolors);
void median_cut(unsigned char *img, ColorBucket *buckets, int curr_buckets,
				int channels, int kcolors);
void generate_pal(unsigned char *img, int width, int height, int channels,
					 int kcolors, Palette *pal);

uint8_t *cham_create_given_palette(Palette pal, unsigned char *img, int width,
								   int height, int channels);

uint8_t *cham_create_given_palette_d(Palette pal, unsigned char *img, int width,
									 int height, int channels,
									 DitherAlgorithm d);

// uint8_t *cham_create_input_palette_kcolors(unsigned char *img, int kcolors,
// 										   int width, int height, int channels);
// 
// uint8_t *cham_create_input_palette_kcolors_d(unsigned char *img, int kcolors,
// 											 int width, int height,
// 											 int channels, DitherAlgorithm d);

#endif	// !CHAM_COLOR_PROC
