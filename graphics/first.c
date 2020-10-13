#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,200.0,0.0,150.0);
}

void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2i(120,45);
		glVertex2i(20,100);
	glEnd();
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);  			// initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(200,100); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Hello World"); 		// create the Display window with title
	init();
	glutDisplayFunc(lineSegment);			// what the display window is conatin
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}


// for compile :- gcc first.c -o first3 -lGL -lGLU -lglut
// run : ./first3

