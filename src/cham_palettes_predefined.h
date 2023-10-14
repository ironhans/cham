#ifndef CHAM_PALETTES
#define CHAM_PALETTES

#include <stdint.h>

#define DEFAULT_2   BW
#define DEFAULT_256 SIX_EIGHT_FIVE

typedef struct {
	int size;
	uint8_t *palette;
	// uint8_t *kdtree;
} Palette;


extern Palette CUSTOM;

extern uint8_t bw_palette[];
extern Palette BW;

extern uint8_t blue_mono_palette[];
extern Palette BLUE_MONO;

// 16 intermediary greys
extern uint8_t six_eight_five_palette[];
extern Palette SIX_EIGHT_FIVE;


#endif
