#include "cham_palettes.h"
#include <math.h>
#include <stdlib.h>
#define UNUSED(param) ((void)((param)))

uint8_t *cham_create_input_palette_depth(int depth, bool transparency)
{
    uint8_t num_colors = pow(2, depth);
    uint8_t *palette = malloc(sizeof(*palette) * num_colors);
    UNUSED(transparency);
    return palette;
}

uint8_t *cham_create_input_palette_kcolors(int kcolors, bool transparency)
{
    uint8_t num_colors = ceil(sqrt(kcolors));
    uint8_t *palette = malloc(sizeof(*palette) * num_colors);
    UNUSED(transparency);
    return palette;
}
