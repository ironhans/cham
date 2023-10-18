#include <libgen.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arg_parser.h"
#include "cham_color_proc.h"
#include "cham_palettes_predefined.h"
#include "gifenc/gifenc.h"
#include "stb_image.h"
#include "stb_image_resize2.h"

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[])
{
	struct arguments args;

	/* Default values. */
	args.depth = 3;
	args.retain_transparency = false;
	args.dither_algo = NONE;
	args.output_file[0] = 0;
	args.palette = SIX_EIGHT_FIVE;	// TODO: Change to be CUSTOM
	args.width = 0;
	args.height = 0;

	argp_parse(&argp, argc, argv, 0, 0, &args);
	if (!args.output_file[0] ||
		args.output_file[strlen(args.output_file) - 1] == '/') {
		strncat(args.output_file, args.input_file,
				MAX_FILENAME_LEN - strlen(args.output_file));
		dirname(args.output_file);
		strncat(args.output_file, "/cham_", 7);
		strncat(args.output_file, basename(args.input_file),
				MAX_FILENAME_LEN - strlen(args.output_file));
		char *ext = strrchr(args.output_file, '.');
		strncpy(ext, ".gif", 5);
	}
	printf("%d\n", argc);
	printf("input name %s\n", args.input_file);
	printf("bit depth %d\n", args.depth);
	printf("dither_algo %d\n", args.dither_algo);
	printf("output name %s\n", args.output_file);
	printf("width %d\n", args.width);
	printf("height %d\n", args.height);

	if (args.retain_transparency) {
		printf("transparency is currently not supported\n");
		return EXIT_SUCCESS;
	}

	int width;
	int height;
	int channels;
	unsigned char *img =
		stbi_load(args.input_file, &width, &height, &channels, STBI_rgb);
	if (img == NULL) {
		fprintf(stderr, "Unabled to read %s\n", args.input_file);
		return EXIT_FAILURE;
	}
	printf("%d x %d x %d\n", width, height, channels);
	// return EXIT_SUCCESS;
	
	if (args.width || args.height) {
		if (args.width && !args.height){
			double ratio = (double) args.width / width;
			args.height = ratio * height;
			printf("NEW %d x %d\n", args.width, args.height);
		} else if (args.height && !args.width) {
			double ratio = (double) args.height / height;
			args.width = ratio * width;
			printf("NEW %d x %d\n", args.width, args.height);
		}
		img = stbir_resize_uint8_srgb(img, width, height, 0, 0, args.width, args.height, 0, STBIR_RGB);
		width = args.width;
		height = args.height;
	}

	ge_GIF *gif_handler;
	if (args.palette.kdtree) {
	gif_handler =
		ge_new_gif(args.output_file, width, height, args.palette.kdtree,
				   (int)ceil(log2(args.palette.size)), -1, -1);
	} else {
	gif_handler =
		ge_new_gif(args.output_file, width, height, args.palette.color_arr,
				   (int)ceil(log2(args.palette.size)), -1, -1);
	}
	uint8_t *pixels;
	// User given palettes
	if (args.dither_algo > 0) {
		pixels = cham_create_given_palette_d(args.palette, img, width, height,
											 STBI_rgb, args.dither_algo);
	} else {
		pixels = cham_create_given_palette(args.palette, img, width, height,
										   STBI_rgb);
	}
	memcpy(gif_handler->frame, pixels, sizeof(*pixels) * width * height);
	ge_add_frame(gif_handler, 0);
	ge_close_gif(gif_handler);
	return EXIT_SUCCESS;
}
