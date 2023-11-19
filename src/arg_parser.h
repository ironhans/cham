#include <argp.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cham_color_proc.h"

#ifndef ARG_PARSER
#define ARG_PARSER

#define MAX_FILENAME_LEN 256

const char *argp_program_version = "cham 0.1.0";

// Argp setup
char doc[] = "cham -- a dithering and image editing tool for gifs";

/* A description of the arguments we accept. */
char args_doc[] = "<input file> -o <output file";

/* The options we understand. */
struct argp_option options[] = {
	{"output", 'o', "OUTPUT", 0, "Output to OUTPUT instead of cham_INPUT", 0},

	{"color-palette", 'p', "COLOR-PALETTE", 0,
	 "If given, will use predef color palette instead of generated one", 1},
	// TODO: Add option for user input palette, array of hexes?

	{"bit-depth", 'b', "BIT-DEPTH", 0,
	 "The bit color depth of the generated palette, if not used default value "
	 "3. If k arg is used, it takes priority",
	 1},

	{"k-colors", 'k', "K-COLORS", 0,
	 "The number of colors present in the generated palette, if not used "
	 "defaults to bit-depth value of 2^depth",
	 1},

	{"dither", 'd', "DITHER", OPTION_ARG_OPTIONAL,
	 "Enables dither with the chosen dithering algorithm, default floyd. "
	 "Avaliable: floyd.",
	 1},

	{"width", 'w', "WIDTH", 0,
	 "The width of the output. If height is not set it preserves the original "
	 "ratio.",
	 1},

	{"height", 'h', "HEIGHT", 0,
	 "The height of the output. If width is not set it preserves the original "
	 "ratio.",
	 1},

	{"transparency", 't', 0, 0, "Retain transparency", 1},
	{0}};

struct arguments {
	char input_file[MAX_FILENAME_LEN];
	uint depth;
	uint kcolors;
	int width;
	int height;
	bool retain_transparency;
	DitherAlgorithm dither_algo;
	Palette palette;
	char output_file[MAX_FILENAME_LEN];
};

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	   know is a pointer to our arguments structure. */
	struct arguments *args = state->input;

	switch (key) {
		case 'p':
			for (int i = 0; arg[i]; i++) {
				arg[i] = tolower(arg[i]);
			}
			if (strcmp("six_eight_five", arg) == 0 || strcmp("256", arg) == 0) {
				args->palette = SIX_EIGHT_FIVE;
			} else if (strcmp("bw", arg) == 0) {
				args->palette = BW;
			} else if (strcmp("blue_mono", arg) == 0) {
				args->palette = BLUE_MONO;
			} else {
				argp_error(state,
						   "Unknown value %s for color palette.\n"
						   "Valid palettes are bw, 256, blue_mono.",
						   arg);
				argp_usage(state);
			}
			break;

		case 'b':
			args->depth = strtol(arg, NULL, 10);
			if (args->depth > 8) {
				argp_error(state, "Bit depth  %d is too large, max value 8.\n",
						   args->depth);
				argp_usage(state);
			}
			if (args->depth < 1) {
				argp_error(state, "Bit depth  %d is too small, min value 1.\n",
						   args->depth);
				argp_usage(state);
			}
			break;

		case 'k':
			args->kcolors = strtol(arg, NULL, 10);
			if (args->kcolors > 256) {
				argp_error(state, "K-Color %d is too large, max value 256.\n",
						   args->kcolors);
				argp_usage(state);
			}
			if (args->kcolors < 1) {
				argp_error(state, "Bit depth  %d is too small, min value 1.\n",
						   args->kcolors);
				argp_usage(state);
			}
			break;

		case 'd':
			if (arg != NULL) {
				for (int i = 0; arg[i]; i++) {
					arg[i] = tolower(arg[i]);
				}
			}
			if (arg == NULL || strcmp("floyd", arg) == 0 ||
				strcmp("floydsteinberg", arg) == 0) {
				args->dither_algo = FLOYD_STEINBERG;
			} else if (strcmp("atkin", arg) == 0 ||
					   strcmp("atkinson", arg) == 0) {
				args->dither_algo = ATKINSON;
			} else {
				argp_error(state,
						   "Unknown value %s for dithering algorithm.\n"
						   "Valid algorithms are floyd, atkinson.",
						   arg);
				argp_usage(state);
			}
			break;

		case 'w':
			args->width = strtol(arg, NULL, 10);
			break;

		case 'h':
			args->height = strtol(arg, NULL, 10);
			break;

		case 'o':
			strncpy(args->output_file, arg, MAX_FILENAME_LEN - 1);
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				strncpy(args->input_file, arg, MAX_FILENAME_LEN - 1);
			}
			if (state->arg_num > 1) {
				argp_usage(state);
			}
			break;

		case ARGP_KEY_END:
			if (state->arg_num < 1) {
				argp_usage(state);
			}
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}
#endif /* ifndef ARG_PARSER */
