#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cham_palettes.h"
#include "cham_palettes_predefined.h"
#include "gifenc/gifenc.h"
#include "stb_image.h"

#define MAX_FILENAME_LEN 256

// TODO: Move this to the dithering handler header file or whatever
typedef enum {
	FLOYD_STEINBERG,
	PLACEHOLDER,
} DitherAlgorithm;

// Argp setup
const char *argp_program_version = "cham 0.1.0";
static char doc[] = "cham -- a dithering and image editting tool";

/* A description of the arguments we accept. */
static char args_doc[] = "<input file> -o <output file";

/* The options we understand. */
static struct argp_option options[] = {
	{"output", 'o', "OUTPUT", 0, "Output to OUTPUT instead of cham_INPUT", 0},

	{"bit-depth", 'b', "BIT DEPTH", 0,
	 "The bit color depth of the output, if not used default value 4", 1},

	{"dither", 'd', "DITHER", OPTION_ARG_OPTIONAL,
	 "The chosen dithering algorithm, default FLOYD_STEINBERG", 1},

	{"transparency", 't', "TRANSPARENCY", 0, "Retain transparency", 1},
	{0}};

struct arguments {
	char input_file[MAX_FILENAME_LEN];
	char output_file[MAX_FILENAME_LEN];
	unsigned int depth;
	bool retain_transparency;
	DitherAlgorithm dither_algo;
};

// int parse_opt(int argc, char *argv[], char *in_file, char *out_file)
// {
// 	// TODO: Either make globals or return config struct
// 	// TODO: Sanitize inputs
// 	// detect properly if input and output are given
// 	// make the control flow here cleaner
// 	for (int i = 1; i < argc; i++) {
// 		char *curr_arg = argv[i];
//
// 		// handle options
// 		char opt = 0;
// 		if (curr_arg[0] == '-' && curr_arg[1] == '-') {
// 			char *opt_str = curr_arg + 2;
// 			// check strings without initial letter string equivalent if need be
// 			opt = opt_str[0];
// 		} else if (curr_arg[0] == '-') {
// 			opt = curr_arg[1];
// 		} else if (i == 1) {
// 			if (strnlen(curr_arg, MAX_FILENAME_LEN + 1) ==
// 				MAX_FILENAME_LEN + 1) {
// 				fprintf(stderr,
// 						"Input filename is too long, maximum 256 characters\n");
// 				return EXIT_FAILURE;
// 			}
// 			snprintf(in_file, MAX_FILENAME_LEN, "%s", curr_arg);
// 			set_input = true;
// 		} else if (i == argc - 1) {
// 			if (strnlen(curr_arg, MAX_FILENAME_LEN + 1) ==
// 				MAX_FILENAME_LEN + 1) {
// 				fprintf(
// 					stderr,
// 					"Output filename is too long, maximum 256 characters\n");
// 				return EXIT_FAILURE;
// 			}
// 			snprintf(out_file, MAX_FILENAME_LEN, "%s", curr_arg);
// 			set_output = true;
// 		}
//
// 		if (opt != 0) {
// 			printf("yay tag:%c\n", opt);
// 			switch (opt) {
// 				case 'h':
// 					printf("cham usage: %s", "HELP_HERE");
// 					return EXIT_SUCCESS;
// 					break;
// 				case 't':
// 					printf("retain transparency\n");
// 					// retain_transparency = true;
// 					break;
// 				case 'd': {
// 					if ((i + 1 != argc - 1) && (argv[i + 1][0] != '-')) {
// 						dither_algo = PLACEHOLDER;
// 					} else {
// 						dither_algo = FLOYD_STEINBERG;
// 					}
// 					printf("dither %d\n", dither_algo);
// 					break;
// 				}
// 				default:
// 					fprintf(stderr,
// 							"Unknown option %c.\nSee 'cham --help' for more "
// 							"info.\n",
// 							opt);
// 					return EXIT_FAILURE;
// 					break;
// 			}
// 		}
// 	}
// 	if (!(set_input && set_output)) {
// 		fprintf(stderr, "error: Following arguments were not provided:\n\n");
// 		if (!set_input) {
// 			fprintf(stderr, "<input> ");
// 		}
// 		if (!set_output) {
// 			fprintf(stderr, "<output> ");
// 		}
// 		fprintf(stderr,
// 				"\n\nUSAGE:\n\n\tcham <input> [OPTIONS] <output>\n\n"
// 				"Try 'cham --help' for more info.\n");
// 		return EXIT_FAILURE;
// 	}
// 	// fprintf(stderr, "Missing argument\n");
// 	return EXIT_SUCCESS;
// }

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	   know is a pointer to our arguments structure. */
	struct arguments *args = state->input;
	printf("KEY %c\n", key);

	switch (key) {
		case 'b':
			args->depth = strtol(arg, NULL, 10);
			break;

		case 'd':
			// strncpy(args->output_file, arg, MAX_FILENAME_LEN);
			break;

		case 'o':
			strncpy(args->output_file, arg, MAX_FILENAME_LEN);
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				strncpy(args->input_file, arg, MAX_FILENAME_LEN);
			}
			// if (state->arg_num > 1) {
			// 	argp_usage(state);
			// }
			break;
		case ARGP_KEY_END:
			printf("KEY END\n");
			if (state->arg_num < 1) {
				argp_usage(state);
			}
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[])
{
	struct arguments args;

	/* Default values. */
	args.depth = 4;
	args.retain_transparency = false;
	args.dither_algo = FLOYD_STEINBERG;
	args.output_file[0] = 0;

	argp_parse(&argp, argc, argv, 0, 0, &args);
	if (!args.output_file[0]) {
		strncpy(args.output_file, "cham_", MAX_FILENAME_LEN);
		strncat(args.output_file, args.input_file, 5);
	}
	printf("%d\n", argc);
	printf("input name %s\n", args.input_file);
	printf("bit depth %d\n", args.depth);
	printf("output name %s\n", args.output_file);
	return EXIT_SUCCESS;

	// if (retain_transparency) {
	// 	return EXIT_SUCCESS;
	// }
	// int width;
	// int height;
	// int channels;
	// unsigned char *img =
	// 	stbi_load(input_filename, &width, &height, &channels, STBI_rgb);
	// if (img == NULL) {
	// 	fprintf(stderr, "Unabled to read %s\n", input_filename);
	// 	return EXIT_FAILURE;
	// }
	// int count = 0;
	// Color *pixels_l = malloc(sizeof(*pixels_l) * width * height);
	// for (int i = 0; i < width * height * STBI_rgb; i += 3) {
	// 	pixels_l[count].r = img[i];
	// 	pixels_l[count].g = img[i + 1];
	// 	pixels_l[count].b = img[i + 2];
	// 	count++;
	// }
	// for (int i = 0; i < width * height; i++) {
	// 	// printf("%d, %d, %d\n", pixels_l[i].r, pixels_l[i].g, pixels_l[i].b);
	// }
	// printf("%d x %d x %d\n", width, height, channels);
	// printf("%d\n", count);

	// ge_GIF *gif_handler =
	// 	ge_new_gif("output/test.gif", 4, 7, DEFAULT_256_PALETTE, 2, -1, -1);
	// uint8_t pixels[] = {2, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2,
	// 					2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0};
	// memcpy(gif_handler->frame, pixels, sizeof(pixels));
	// ge_add_frame(gif_handler, 0);
	// ge_close_gif(gif_handler);
	// return EXIT_SUCCESS;
}
