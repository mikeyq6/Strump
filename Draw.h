#include <SDL.h>

#ifndef DRAW_H
#define DRAW_H

#define BACKGROUNDTILES 0x400
#define NUMPIXELS 0x5a00

#define S_WIDTH 160
#define S_HEIGHT 144

typedef struct _tile {
	uint8_t data[16];
} tile;

tile *background[BACKGROUNDTILES];
uint32_t screenPixels[NUMPIXELS];

void drawInit(int argc, char* argv[]);
void loadBackground();
void callRefresh();

#endif 