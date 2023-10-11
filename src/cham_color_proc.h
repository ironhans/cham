#include <stdint.h>
#include <math.h>

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
