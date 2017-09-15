#include <stdio.h> 
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Draw.h"

extern FILE * __cdecl __iob_func(void)
{	
	FILE _iob[] = {*stdin, *stdout, *stderr};
    return _iob;
}

void displayMe(void);

void drawInit(int argc, char* argv[]) {

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
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
 
    glFlush();
	glutSwapBuffers();
}
 