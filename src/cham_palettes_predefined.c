#include "cham_palettes_predefined.h"
#include <stdint.h>
// predefined color palettes
// Defaults for depth

// BW palette
Palette BW = {2, bw_palette};
uint8_t bw_palette[] = {
    0x000, 0x000, 0x000,
    0xFF, 0xFF, 0xFF,
};

// 16 slots left used for intermediary greys
Palette SIX_EIGHT_FIVE = {256, six_eight_five_palette};
uint8_t six_eight_five_palette[] = {
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x40,
    0x00, 0x00, 0x80,
    0x00, 0x00, 0xBF,
    0x00, 0x00, 0xFF,
    0x00, 0x24, 0x00,
    0x00, 0x24, 0x40,
    0x00, 0x24, 0x80,
    0x00, 0x24, 0xBF,
    0x00, 0x24, 0xFF,
    0x00, 0x49, 0x00,
    0x00, 0x49, 0x40,
    0x00, 0x49, 0x80,
    0x00, 0x49, 0xBF,
    0x00, 0x49, 0xFF,
    0x00, 0x6D, 0x00,
    0x00, 0x6D, 0x40,
    0x00, 0x6D, 0x80,
    0x00, 0x6D, 0xBF,
    0x00, 0x6D, 0xFF,
    0x00, 0x92, 0x00,
    0x00, 0x92, 0x40,
    0x00, 0x92, 0x80,
    0x00, 0x92, 0xBF,
    0x00, 0x92, 0xFF,
    0x00, 0xB6, 0x00,
    0x00, 0xB6, 0x40,
    0x00, 0xB6, 0x80,
    0x00, 0xB6, 0xBF,
    0x00, 0xB6, 0xFF,
    0x00, 0xDB, 0x00,
    0x00, 0xDB, 0x40,
    0x00, 0xDB, 0x80,
    0x00, 0xDB, 0xBF,
    0x00, 0xDB, 0xFF,
    0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x40,
    0x00, 0xFF, 0x80,
    0x00, 0xFF, 0xBF,
    0x00, 0xFF, 0xFF,
    0x33, 0x00, 0x00,
    0x33, 0x00, 0x40,
    0x33, 0x00, 0x80,
    0x33, 0x00, 0xBF,
    0x33, 0x00, 0xFF,
    0x33, 0x24, 0x00,
    0x33, 0x24, 0x40,
    0x33, 0x24, 0x80,
    0x33, 0x24, 0xBF,
    0x33, 0x24, 0xFF,
    0x33, 0x49, 0x00,
    0x33, 0x49, 0x40,
    0x33, 0x49, 0x80,
    0x33, 0x49, 0xBF,
    0x33, 0x49, 0xFF,
    0x33, 0x6D, 0x00,
    0x33, 0x6D, 0x40,
    0x33, 0x6D, 0x80,
    0x33, 0x6D, 0xBF,
    0x33, 0x6D, 0xFF,
    0x33, 0x92, 0x00,
    0x33, 0x92, 0x40,
    0x33, 0x92, 0x80,
    0x33, 0x92, 0xBF,
    0x33, 0x92, 0xFF,
    0x33, 0xB6, 0x00,
    0x33, 0xB6, 0x40,
    0x33, 0xB6, 0x80,
    0x33, 0xB6, 0xBF,
    0x33, 0xB6, 0xFF,
    0x33, 0xDB, 0x00,
    0x33, 0xDB, 0x40,
    0x33, 0xDB, 0x80,
    0x33, 0xDB, 0xBF,
    0x33, 0xDB, 0xFF,
    0x33, 0xFF, 0x00,
    0x33, 0xFF, 0x40,
    0x33, 0xFF, 0x80,
    0x33, 0xFF, 0xBF,
    0x33, 0xFF, 0xFF,
    0x66, 0x00, 0x00,
    0x66, 0x00, 0x40,
    0x66, 0x00, 0x80,
    0x66, 0x00, 0xBF,
    0x66, 0x00, 0xFF,
    0x66, 0x24, 0x00,
    0x66, 0x24, 0x40,
    0x66, 0x24, 0x80,
    0x66, 0x24, 0xBF,
    0x66, 0x24, 0xFF,
    0x66, 0x49, 0x00,
    0x66, 0x49, 0x40,
    0x66, 0x49, 0x80,
    0x66, 0x49, 0xBF,
    0x66, 0x49, 0xFF,
    0x66, 0x6D, 0x00,
    0x66, 0x6D, 0x40,
    0x66, 0x6D, 0x80,
    0x66, 0x6D, 0xBF,
    0x66, 0x6D, 0xFF,
    0x66, 0x92, 0x00,
    0x66, 0x92, 0x40,
    0x66, 0x92, 0x80,
    0x66, 0x92, 0xBF,
    0x66, 0x92, 0xFF,
    0x66, 0xB6, 0x00,
    0x66, 0xB6, 0x40,
    0x66, 0xB6, 0x80,
    0x66, 0xB6, 0xBF,
    0x66, 0xB6, 0xFF,
    0x66, 0xDB, 0x00,
    0x66, 0xDB, 0x40,
    0x66, 0xDB, 0x80,
    0x66, 0xDB, 0xBF,
    0x66, 0xDB, 0xFF,
    0x66, 0xFF, 0x00,
    0x66, 0xFF, 0x40,
    0x66, 0xFF, 0x80,
    0x66, 0xFF, 0xBF,
    0x66, 0xFF, 0xFF,
    0x99, 0x00, 0x00,
    0x99, 0x00, 0x40,
    0x99, 0x00, 0x80,
    0x99, 0x00, 0xBF,
    0x99, 0x00, 0xFF,
    0x99, 0x24, 0x00,
    0x99, 0x24, 0x40,
    0x99, 0x24, 0x80,
    0x99, 0x24, 0xBF,
    0x99, 0x24, 0xFF,
    0x99, 0x49, 0x00,
    0x99, 0x49, 0x40,
    0x99, 0x49, 0x80,
    0x99, 0x49, 0xBF,
    0x99, 0x49, 0xFF,
    0x99, 0x6D, 0x00,
    0x99, 0x6D, 0x40,
    0x99, 0x6D, 0x80,
    0x99, 0x6D, 0xBF,
    0x99, 0x6D, 0xFF,
    0x99, 0x92, 0x00,
    0x99, 0x92, 0x40,
    0x99, 0x92, 0x80,
    0x99, 0x92, 0xBF,
    0x99, 0x92, 0xFF,
    0x99, 0xB6, 0x00,
    0x99, 0xB6, 0x40,
    0x99, 0xB6, 0x80,
    0x99, 0xB6, 0xBF,
    0x99, 0xB6, 0xFF,
    0x99, 0xDB, 0x00,
    0x99, 0xDB, 0x40,
    0x99, 0xDB, 0x80,
    0x99, 0xDB, 0xBF,
    0x99, 0xDB, 0xFF,
    0x99, 0xFF, 0x00,
    0x99, 0xFF, 0x40,
    0x99, 0xFF, 0x80,
    0x99, 0xFF, 0xBF,
    0x99, 0xFF, 0xFF,
    0xCC, 0x00, 0x00,
    0xCC, 0x00, 0x40,
    0xCC, 0x00, 0x80,
    0xCC, 0x00, 0xBF,
    0xCC, 0x00, 0xFF,
    0xCC, 0x24, 0x00,
    0xCC, 0x24, 0x40,
    0xCC, 0x24, 0x80,
    0xCC, 0x24, 0xBF,
    0xCC, 0x24, 0xFF,
    0xCC, 0x49, 0x00,
    0xCC, 0x49, 0x40,
    0xCC, 0x49, 0x80,
    0xCC, 0x49, 0xBF,
    0xCC, 0x49, 0xFF,
    0xCC, 0x6D, 0x00,
    0xCC, 0x6D, 0x40,
    0xCC, 0x6D, 0x80,
    0xCC, 0x6D, 0xBF,
    0xCC, 0x6D, 0xFF,
    0xCC, 0x92, 0x00,
    0xCC, 0x92, 0x40,
    0xCC, 0x92, 0x80,
    0xCC, 0x92, 0xBF,
    0xCC, 0x92, 0xFF,
    0xCC, 0xB6, 0x00,
    0xCC, 0xB6, 0x40,
    0xCC, 0xB6, 0x80,
    0xCC, 0xB6, 0xBF,
    0xCC, 0xB6, 0xFF,
    0xCC, 0xDB, 0x00,
    0xCC, 0xDB, 0x40,
    0xCC, 0xDB, 0x80,
    0xCC, 0xDB, 0xBF,
    0xCC, 0xDB, 0xFF,
    0xCC, 0xFF, 0x00,
    0xCC, 0xFF, 0x40,
    0xCC, 0xFF, 0x80,
    0xCC, 0xFF, 0xBF,
    0xCC, 0xFF, 0xFF,
    0xFF, 0x00, 0x00,
    0xFF, 0x00, 0x40,
    0xFF, 0x00, 0x80,
    0xFF, 0x00, 0xBF,
    0xFF, 0x00, 0xFF,
    0xFF, 0x24, 0x00,
    0xFF, 0x24, 0x40,
    0xFF, 0x24, 0x80,
    0xFF, 0x24, 0xBF,
    0xFF, 0x24, 0xFF,
    0xFF, 0x49, 0x00,
    0xFF, 0x49, 0x40,
    0xFF, 0x49, 0x80,
    0xFF, 0x49, 0xBF,
    0xFF, 0x49, 0xFF,
    0xFF, 0x6D, 0x00,
    0xFF, 0x6D, 0x40,
    0xFF, 0x6D, 0x80,
    0xFF, 0x6D, 0xBF,
    0xFF, 0x6D, 0xFF,
    0xFF, 0x92, 0x00,
    0xFF, 0x92, 0x40,
    0xFF, 0x92, 0x80,
    0xFF, 0x92, 0xBF,
    0xFF, 0x92, 0xFF,
    0xFF, 0xB6, 0x00,
    0xFF, 0xB6, 0x40,
    0xFF, 0xB6, 0x80,
    0xFF, 0xB6, 0xBF,
    0xFF, 0xB6, 0xFF,
    0xFF, 0xDB, 0x00,
    0xFF, 0xDB, 0x40,
    0xFF, 0xDB, 0x80,
    0xFF, 0xDB, 0xBF,
    0xFF, 0xDB, 0xFF,
    0xFF, 0xFF, 0x00,
    0xFF, 0xFF, 0x40,
    0xFF, 0xFF, 0x80,
    0xFF, 0xFF, 0xBF,
    0xFF, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F,
	0x1F, 0x1F, 0x1F,
	0x2F, 0x2F, 0x2F,
	0x3F, 0x3F, 0x3F,
	0x4F, 0x4F, 0x4F,
	0x5F, 0x5F, 0x5F,
	0x6F, 0x6F, 0x6F,
	0x7F, 0x7F, 0x7F,
	0x8F, 0x8F, 0x8F,
	0x9F, 0x9F, 0x9F,
	0xAE, 0xAE, 0xAE,
	0xBE, 0xBE, 0xBE,
	0xCE, 0xCE, 0xCE,
	0xDE, 0xDE, 0xDE,
	0xEE, 0xEE, 0xEE,
	0xFE, 0xFE, 0xFE,
};