#include <stdio.h> 
#include <stdlib.h>
#include "alias.h"
#include "CPU.h"

#include "Draw.h"

SDL_Event event;
SDL_Window *screen;
SDL_Renderer *renderer;
SDL_Texture *texture;

void displayMe(void);
void timerCalled(int value);
void getPixel(tile *t, uint8_t col, uint8_t row, uint8_t *val);
void getTileAt(uint16_t address, tile *t);
void setBackgroundPixels();

void drawInit(int argc, char* argv[]) {
	for(int i=0; i<BACKGROUNDTILES; i++) {
		background[i] = malloc(sizeof(tile));
	}
	int gogogo = 1;


    SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_CreateWindow("Hello World!",
		10, 10, S_WIDTH, S_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(screen, -1, 0);
	texture = SDL_CreateTexture(renderer,
                               SDL_PIXELFORMAT_RGB888,
                               SDL_TEXTUREACCESS_STREAMING,
                               S_WIDTH, S_HEIGHT);

	/*while (gogogo) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            gogogo = 0;
    }*/
    //SDL_Quit();
	/*
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(160, 144);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(displayMe);
	glShadeModel(GL_SMOOTH);
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity (); 
	glClearColor(248,248,248,0);
	//gluPerspective(30.0, 1.5, 0.1, 10.0);
    //glutMainLoopEvent();
	glutMainLoopEvent();*/
}

void callRefresh() {
	//printf("Refresh...");
	//_getch();
	//glutMainLoopEvent();
	loadBackground();
	setBackgroundPixels();
	
	SDL_UpdateTexture(texture, NULL, screenPixels, S_WIDTH * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	//_getch();
}
void displayMe(void)
{
	printf("x");
	loadBackground();
	/*
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_QUADS);
	glColor3f( 1.0, 0.0, 0.0 );
	glVertex2i(20, 10);
	glVertex2i(20, 20);
	glVertex2i(10, 20);
	glVertex2i(10, 10);
	
	uint8_t test[2 * 2 * 3] = {10, 10, 10, 50, 50 ,50, 90,10,90, 200,0,0};
	
	glRasterPos2i(20,20);
	glDrawPixels(2, 2, GL_RGB, GL_UNSIGNED_BYTE, test);
	
	glEnd();
	/*
	//glLoadIdentity();
	//gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	// Red side - Bottom
    glBegin(GL_QUADS);
		glColor3f( 1.0, 0.0, 0.0 );
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
		
	// Green side - Left
        glColor3f( 0.0, 1.0, 0.0 );
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
	
	// Cyan side - Top
        glColor3f( 0.0, 1.0, 1.0 );
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, -0.5);
		
	// Blue side - Right
        glColor3f( 0.0, 0.0, 1.0 );
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
	
	// Purple side - Front
        glColor3f( 1.0, 0.0, 1.0 );
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, 0.5);
	
	// Yellow side - Back
        glColor3f( 1.0, 1.0, 0.0 );
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
	
	glRotatef( 14, 1.0, 0.0, 0.0 );
	glRotatef( 5, 0.0, 1.0, 0.0 );
	glRotatef( 2, 0.0, 0.0, 1.0 );
 /
    glFlush();
	glutSwapBuffers();
	*/
}
 
void loadBackground() {
	// Get wchich tile set to use
	uint16_t bMap = GetBackgroundTileMapLocation();
	// Draw some tiles, because why the hell not?
	uint16_t address = BGWindowTileLocation();
	uint16_t offset = 16;
	uint8_t tileNum;
	//printf("bMap=%04x, address=%04x\n", bMap, address);
		
	for(int i=0; i<BACKGROUNDTILES; i++) {
		tileNum = Memory[bMap + i];
		//printf("tileNum=%02x\n", Memory[bMap + i]);
		if(address == 0x8800) { // allow for negative numbers
			getTileAt((offset * (int8_t)tileNum) + address, background[i]);
		} else {
			getTileAt((offset * tileNum) + address, background[i]);
			if(tileNum > 0) {
				//printf("address=%04x\n", (offset * tileNum) + address);
				/*printf("Tile data: ");
				for(int k=0; k<16; k++) {
					printf("%02x ", background[i]->data[k]);
				}
				printf("\n");*/
			}
		}
	}
}

void setBackgroundPixels() {
	uint8_t sX = Memory[SCX];
	uint8_t sY = Memory[SCY];
	tile *cur;
	uint8_t pixel;
	int sPixelsIndex = 0;
	
	// Which tile to start with
	uint8_t pX = sX % 8;
	uint8_t pY = sY % 8;
	
	// printf("sX=%02x, sY=%02x\n", sX, sY);
	
	for(int y=sY; y<sY + S_HEIGHT; y++) {
		for(int x=sX; x<sX + S_WIDTH; x++, sPixelsIndex++) {
			pX = x % 8;
			pY = y % 8;
	
			int index = ((y / 8) * 32) + (x / 8);
			cur = background[index];
			
			getPixel(cur, pX, pY, &pixel);
			
			//printf("screenPixels[%04x] = %08x\n", sPixelsIndex, sPixel);
			screenPixels[sPixelsIndex] = GetColourFor(pixel);
		}
	}
	
}

void getPixel(tile *t, uint8_t col, uint8_t row, uint8_t *val) {
	uint8_t bit = 1 << (8 - (col + 1));
	//printf("bit=%x, ", bit);
	uint8_t rIndex = (row * 2);
	//printf("row=%u, col=%u, rIndex=%u, bit=%x\n", row, col, rIndex, bit);
	//printf("t->data[%u] = %x, t->data[%u + 1] = %x\n", rIndex, t->data[rIndex], rIndex, t->data[rIndex+1]);
	*val = ((t->data[rIndex] & bit) ? 1 : 0) + (((t->data[rIndex + 1]) & bit) ? 2 : 0);
}
void getTileAt(uint16_t address, tile *t) {
	//printf("address=%04x\n", address);
	for(int i=0; i<16; i++) {
		t->data[i] = ReadMem(address + i);
		//printf("row[%u] = %x, ReadMem(%02x) = %x\n", i, t->data[i], address + i, ReadMem(address + i));
	}
	
	// uint8_t val = 0;
	// if(address != 0x8000) {
		// printf("Tile data: ");
		// for(int k=0; k<16; k++) {
			// printf("%02x ", t->data[k]);
		// }
		// printf("\n");
		// for(int i=0; i<8; i++) {
			// for(int j=0; j<8; j++) {
				// getPixel(t, i, j, &val);
				// if(val > 0) {
					// printf("%u", val);
				// } else {
					// printf(" ");
				// }
			// }
			// printf("\n");
		// }
		// printf("\n");
	// }
	
}