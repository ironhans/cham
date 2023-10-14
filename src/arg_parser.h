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
char doc[] = "cham -- a dithering and image editing tool producing gifs";

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
	 "3",
	 1},

	{"dither", 'd', "DITHER", OPTION_ARG_OPTIONAL,
	 "The chosen dithering algorithm, default floyd. Avaliable: floyd.", 1},

	{"transparency", 't', 0, 0, "Retain transparency", 1},
	{0}};

struct arguments {
	char input_file[MAX_FILENAME_LEN];
	char output_file[MAX_FILENAME_LEN];
	uint depth;
	bool retain_transparency;
	DitherAlgorithm dither_algo;
	Palette palette;
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
			break;

		case 'd':
			if (arg == NULL || strcmp("floyd", arg) == 0 ||
				strcmp("floydsteinberg", arg) == 0) {
				args->dither_algo = FLOYD_STEINBERG;
			} else {
				argp_error(state,
						   "Unknown value %s for dithering algorithm.\n"
						   "Valid algorithms are floyd, placeholder.",
						   arg);
				argp_usage(state);
			}
			break;

		case 'o':
			strncpy(args->output_file, arg, MAX_FILENAME_LEN);
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				strncpy(args->input_file, arg, MAX_FILENAME_LEN);
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
