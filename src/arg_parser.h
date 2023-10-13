#include <argp.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

	{"bit-depth", 'b', "BIT-DEPTH", 0,
	 "The bit color depth of the output, if not used default value 4", 1},

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
};

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	   know is a pointer to our arguments structure. */
	struct arguments *args = state->input;

	switch (key) {
		case 'b':
			args->depth = strtol(arg, NULL, 10);
			break;

		case 'd':
			if (arg == NULL || strcmp("floyd", arg) == 0) {
				args->dither_algo = FLOYD_STEINBERG;
			} else if (strcmp("placeholder", arg) == 0) {
				args->dither_algo = PLACEHOLDER;
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
