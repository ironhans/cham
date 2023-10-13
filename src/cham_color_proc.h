#ifndef CHAM_COLOR_PROC
#define CHAM_COLOR_PROC
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "stb_image.h"
#include "cham_palettes_predefined.h"

#define UNUSED(param) ((void)((param)))

typedef enum {
	FLOYD_STEINBERG,
	PLACEHOLDER,
} DitherAlgorithm;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Color;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} ColorAlpha;


double sq_distance_between_colors(Color x, Color y);
double distance_between_colors(Color x, Color y);
double sq_distance_between_colors_a(ColorAlpha x, ColorAlpha y);
double distance_between_colors_a(ColorAlpha x, ColorAlpha y);

uint8_t *cham_create_given_palette(Palette pal, unsigned char *img, int width, int height, int channels);
uint8_t *cham_create_input_palette_kcolors(unsigned char *img, int kcolors, int width, int height, int channels);

#endif	// !CHAM_COLOR_PROC
