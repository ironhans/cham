#ifndef CHAM_PAL
#define CHAM_PAL

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "cham_color_proc.h"


uint8_t *cham_create_default_palette_256(bool transparency);

uint8_t *cham_create_input_palette(int depth, bool transparency);

uint8_t *cham_create_input_palette_kcolors(int kcolors, bool transparency);

#endif // !CHAM_PAL
