/*
 a. PROGRAMMER: Timothy Vicars 

 b. COURSE: CSCI 4350-1 
 c. DATE:  9/20/2018 

 d. ASSIGNMENT: Homework #2 

 e. ENVIRONMENT: Linux Clang/GCC 

 f. FILES INCLUDED: hw2.c 

 g. PURPOSE: this program draws an equilaterial triangle and a piece of text describing the triangle. 

 h. INPUT:  

 i. PRECONDITIONS:  

 j. OUTPUT: the output is a graphical window 

 n.  EXAMPLE: ./hw2
*/


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>
#include<string.h>

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,200.0,0.0,150.0);
}

void triangle(void) {
    int ax , bx , cx = 0;
    int ay , by , cy = 0;
    int centerx = 100;
    int centery = 50;
   
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,0.0,0.0);
    glBegin(GL_TRIANGLES);
        glVertex2i(centerx-25,centery);
        glVertex2i(centerx+25,centery);
        glVertex2i(centerx, centery+40);
    glEnd();
}

void drawText( ) {
	const char *string = "An equilateral triangle";
	int j = strlen( string );
 
	glColor3f( 0, 0, 1.0 ); //Blue
	glRasterPos2f( 75, 40 );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , string[i] );
	}
//	glFlush();
}

void drawTextAndTriangle( ) {
	triangle();
	drawText( );
    glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);  			// initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(200,100); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Homework #2"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawTextAndTriangle);			// what the display window is conatin
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}


// for compile :- gcc hw2.c -o hw2 -lGL -lGLU -lglut
// run : ./hw2

