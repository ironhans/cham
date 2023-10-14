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
				break;
			}
		}
	}
	return index;
}

Color quant_error(Color old_color, Color new_color)
{
	Color t;
	t.r = (old_color.r - (int)new_color.r);
	t.g = (old_color.g - (int)new_color.g);
	t.b = (old_color.b - (int)new_color.b);
	return t;
}

void set_color_error(unsigned char *img, int index, double weight, Color err)
{
	// int r_sum = img[index] + weight * err.r;
	// int g_sum = img[index + 1] + weight * err.g;
	// int b_sum = img[index + 2] + weight * err.b;
	PLUS_TRUNCATE_UCHAR(img[index], weight * err.r);
	PLUS_TRUNCATE_UCHAR(img[index + 1], weight * err.g);
	PLUS_TRUNCATE_UCHAR(img[index + 2], weight * err.b);
}

Color get_color_error(unsigned char *img, int index, double weight, Color err)
{
	// int r_sum = img[index] + weight * err.r;
	// int g_sum = img[index + 1] + weight * err.g;
	// int b_sum = img[index + 2] + weight * err.b;
	// PLUS_TRUNCATE_UCHAR(img[index], weight * err.r);
	// PLUS_TRUNCATE_UCHAR(img[index + 1], weight * err.g);
	// PLUS_TRUNCATE_UCHAR(img[index + 2], weight * err.b);
	Color t;
	t.r = (img[index] + weight * err.r);
	t.g = (img[index + 1] + weight * err.g);
	t.b = (img[index + 2] + weight * err.b);
	// printf("sums: %d, %d, %d\n", r_sum, g_sum, b_sum);
	// printf("real sums: %d, %d, %d\n\n", img[index], img[index + 1], img[index + 2]);
	return t;
}

void set_pixel_color(unsigned char *img, int index, Color c)
{
	if (c.r > 255) c.r = 255;
	if (c.r < 0) c.r = 0;
	if (c.g > 255) c.g = 255;
	if (c.g < 0) c.g = 0;
	if (c.b > 255) c.b = 255;
	if (c.b < 0) c.b = 0;
	img[index] = c.r;
	img[index + 1] = c.g;
	img[index + 2] = c.b;
	// int r_sum = img[index] + weight * err.r;
	// int g_sum = img[index + 1] + weight * err.g;
	// int b_sum = img[index + 2] + weight * err.b;
	// printf("sums: %d, %d, %d\n", r_sum, g_sum, b_sum);
	// printf("real sums: %d, %d, %d\n\n", img[index], img[index + 1], img[index + 2]);
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
		pixels[i / channels] = find_closest_color(c, pal);
		// printf("\nInd %d\n", i );
		// int x = GET_X(i, channels, width);
		// int y = GET_Y(i, channels, width);
		// printf("%d, %d\n", x, y);
		// printf("Cal %d\n", GET_IND(x, y, channels, width));
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
				Color res = get_color_error(img, GET_IND(x + 1, y, channels, width), w1,
								quant_err);
				set_pixel_color(img, GET_IND(x + 1, y, channels, width), res);
			}
			if (y + 1 < height) {
				Color res = get_color_error(img, GET_IND(x, y + 1, channels, width), w2,
								quant_err);
				set_pixel_color(img, GET_IND(x, y + 1, channels, width), res);
				if (x > 0) {
					Color res = get_color_error(img, GET_IND(x - 1, y + 1, channels, width),
									w3, quant_err);
					set_pixel_color(img, GET_IND(x - 1, y + 1, channels, width), res);
				}
				if (x + 1 < width) {
					Color res = get_color_error(img, GET_IND(x + 1, y + 1, channels, width),
									w4, quant_err);
					set_pixel_color(img, GET_IND(x + 1, y + 1, channels, width), res);
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
