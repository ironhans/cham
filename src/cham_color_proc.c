#include "cham_color_proc.h"

double sq_distance_between_colors(Color x, Color y)
{
    return (pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) + (pow((y.b - x.b), 2));
}

double distance_between_colors(Color x, Color y)
{
    return sqrt((pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) + (pow((y.b - x.b), 2)));
}

double sq_distance_between_colors_a(ColorAlpha x, ColorAlpha y)
{
    return (pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) + (pow((y.b - x.b), 2));
}

double distance_between_colors_a(ColorAlpha x, ColorAlpha y)
{
    return sqrt((pow((y.r - x.r), 2)) + (pow((y.g - x.g), 2)) + (pow((y.b - x.b), 2)));
}
