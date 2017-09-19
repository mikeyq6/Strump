#ifndef DRAW_H
#define DRAW_H

#define BACKGROUNDTILES 0x400

typedef struct _tile {
	uint8_t data[16];
} tile;

tile *background[BACKGROUNDTILES];

void drawInit(int argc, char* argv[]);
void loadBackground();

#endif 