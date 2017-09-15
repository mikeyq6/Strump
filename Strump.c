#include <stdio.h> 
#include <conio.h>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Strump.h"
#include "CPU.h"

//ILE _iob[] = ;

extern FILE * __cdecl __iob_func(void)
{	
	FILE _iob[] ={*stdin, *stdout, *stderr};
    return _iob;
}

void displayMe(void);

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glFlush();
}
 
int main(int argc, char* argv[]) {
	//printf("Test %i, %i\n", argc, sizeof(argv)/sizeof(argv[0]));
	
	if(argc < 2) {
		printf("Usage: %s [gameboy rom]\n", argv[0]);
		return 1;
	}
	
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(displayMe);
    glutMainLoop();
	
	// Try to open ROM file
	FILE *fp;
	int err = fopen_s(&fp, argv[1], "r");
	if(err > 0) {
		printf("Coulnd't open file '%s'\n", argv[1]);
		return 2;
	}
	uint16_t size = (uint16_t)fread(Cartridge, sizeof(uint8_t), CARTRIDGE_SIZE, fp);
	/* printf("Size: %x, B5=%x, ac880=%x, bd330=%x", size, Cartridge[0xb5], Cartridge[0xac880], Cartridge[0xbd330]);
	_getch();
	return 0;*/
	initCPU();
	SetCartridgeInfo();
	DisplayCartridgeInfo();
	_getch();

	Start();
	 
	//printf("a=%u, f=%u, af=%u\n", AF.a, AF.f, AF.af);
	
	printf("\nAny key to quit");
	_getch();
	Quit();
	 
	return 0;
}