#ifndef CHAM_PALETTES
#define CHAM_PALETTES

#include <stdint.h>

#define DEFAULT_2   BW
#define DEFAULT_256 SIX_EIGHT_FIVE

typedef struct {
	int r;
	int g;
	int b;
} Color;

typedef struct {
	int r;
	int g;
	int b;
	int a;
} ColorAlpha;

typedef struct {
	int size;
	uint8_t *color_arr;
	uint8_t *kdtree;
} Palette;


extern Palette CUSTOM;

extern uint8_t bw_palette[];
extern Palette BW;

extern uint8_t blue_mono_palette[];
extern Palette BLUE_MONO;

// 16 intermediary greys
extern uint8_t six_eight_five_palette[];
extern uint8_t six_eight_five_kdtree[];
extern Palette SIX_EIGHT_FIVE;


#endif
