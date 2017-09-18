#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "alias.h"
#include "CPU.h"

#include "Draw.h"

typedef struct _tile {
	uint8_t data[16];
} tile;

tile *background[BACKGROUNDTILES];

extern FILE * __cdecl __iob_func(void)
{	
	FILE _iob[] = {*stdin, *stdout, *stderr};
    return _iob;
}

void displayMe(void);
uint8_t getPixel(tile *t, uint8_t row, uint8_t col, uint8_t *val);
void getTileAt(uint16_t address, tile *t);

void drawInit(int argc, char* argv[]) {
	for(int i=0; i<BACKGROUNDTILES; i++) {
		background[i] = malloc(sizeof(tile));
	}
	//
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(displayMe);
	glShadeModel(GL_SMOOTH);
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity (); 
	glClearColor(248,248,248,0);
	//gluPerspective(30.0, 1.5, 0.1, 10.0);
    glutMainLoopEvent();
}


void displayMe(void)
{
	loadBackground();
	
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_QUADS);
	
	glDrawPixels(8, 8, GL_RGBA, GL_UNSIGNED_BYTE, &background[0]);
	
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
 */
    glFlush();
	glutSwapBuffers();
}
 
void loadBackground() {
	// Draw some tiles, because why the hell not?
	uint16_t address = 0x8000;
	for(int i=0; i<0xff; i++) {
		getTileAt(address + i, background[i]);
	}
}

uint8_t getPixel(tile *t, uint8_t row, uint8_t col, uint8_t *val) {
	uint8_t bit = 1 << (8 - (col + 1));
	//printf("bit=%x, ", bit);
	uint8_t rIndex = (row * 2) - 1;
	//printf("t->data[%u] = %x, t->data[%u + 1] = %x\n", rIndex, t->data[rIndex], rIndex, t->data[rIndex+1]);
	*val = ((t->data[rIndex] & bit) ? 1 : 0) + (((t->data[rIndex + 1]) & bit) ? 2 : 0);
}
void getTileAt(uint16_t address, tile *t) {
	for(int i=0; i<16; i++) {
		t->data[i] = ReadMem(address + i);
		printf("row[%u] = %x, ReadMem(%02x) = %x\n", i, t->data[i], address + i, ReadMem(address + i));
	}
	
	uint8_t val = 0;
	for(int i=0; i<8; i++) {
		for(int j=0; j<8; j++) {
			getPixel(t, i, j, &val);
			printf("%u", val);
		}
		printf("\n");
	}
}