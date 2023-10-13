#include "cham_color_proc.h"

double sq_distance_between_colors(Color x, Color y)
{
	return (pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
		   (pow((y.b - x.b), 2));
}

double distance_between_colors(Color x, Color y)
{
	return sqrt((pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
				(pow((y.b - x.b), 2)));
}

double sq_distance_between_colors_a(ColorAlpha x, ColorAlpha y)
{
	return (pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
		   (pow((y.b - x.b), 2));
}

double distance_between_colors_a(ColorAlpha x, ColorAlpha y)
{
	return sqrt((pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) +
				(pow((y.b - x.b), 2)));
}

uint8_t *cham_create_given_palette(Palette pal, unsigned char *img, int width,
										 int height, int channels)
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
		int index = 0;
		double min = INT_MAX;
		for (int j = 0; j < pal.size * 3; j += 3) {
			Color p;
			p.r = pal.palette[j];
			p.g = pal.palette[j + 1];
			p.b = pal.palette[j + 2];
			double curr = distance_between_colors(c, p);
			if (curr < min) {
				min = curr;
				index = j / 3;
				if (curr == 0) {
					break;
				}
			}
		}
		pixels[i / channels] = index;
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
