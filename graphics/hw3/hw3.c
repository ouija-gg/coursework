/*
 a. PROGRAMMER: Timothy Vicars 

 b. COURSE: CSCI 4350-1 
 c. DATE:  10/18/2018 

 d. ASSIGNMENT: Homework #3 

 e. ENVIRONMENT: Linux Clang/GCC 

 f. FILES INCLUDED: hw3.c 

 g. PURPOSE: this program draws an equilaterial triangle and a piece of text describing the triangle. 
             This program uses an event loop to change the color of the triangle when the mouse moves
             within the bounds of the triangle

 h. INPUT: this program uses mouse input within the event loop. 

 i. PRECONDITIONS:  

 j. OUTPUT: the output is a graphical window 

 n.  EXAMPLE: ./hw3
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
    gluOrtho2D(0.0,800.0,0.0,600.0);
}

// aaaaaaaaaaaaaaaaaaaahhhh a global variable
// we should figure out another way at some point
int mouse_inside = 0;

void triangle(void) {
    int ax , bx , cx = 0;
    int ay , by , cy = 0;
    int centerx = 400;
    int centery = 300;
   
	glClear(GL_COLOR_BUFFER_BIT);
    if (mouse_inside)
        glColor3f(0.0,0.0,1.0);
    else
	    glColor3f(1.0,0.0,0.0);

    glBegin(GL_TRIANGLES);
        glVertex2i(centerx-50,centery);
        glVertex2i(centerx+50,centery);
        glVertex2i(centerx, centery+80);
    glEnd();
}

//the area of a triangle
float area (int x1, int y1, int x2, int y2, int x3, int y3) {
    return fabs((x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))/2.0);
}
//this is a helper function to see if the a point is inside a polygon
int isInside(int testx, int testy) {
    int p1x = 350;
    int p1y = 300;
    int p2x = 450;
    int p2y = 300;
    int p3x = 400;
    int p3y = 380;
    
    //normalize mouse coords to screen coords
    testx = 800-testx;
    testy = 600-testy;
    float A = area(p1x, p1y, p2x, p2y, p3x, p3y);
    float A1= area(testx, testy, p2x, p2y, p3x, p3y);
    float A2= area(p1x, p1y, testx, testy, p3x,p3y);
    float A3= area(p1x, p1y, p2x, p2y, testx, testy);

    return (A == A1 + A2 + A3);
}

void drawText( ) {
	const char *string = "An equilateral triangle";
	int j = strlen( string );
 
	glColor3f( 0, 0, 1.0 ); //Blue
	glRasterPos2f( 75, 40 );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , string[i] );
	}
}

void drawTextAndTriangle( ) {
	triangle();
	drawText( );
    glFlush();
}

void mouse( int button, int state, int x, int y )
{
    if (isInside(x, y))
        mouse_inside = 1;
    else
        mouse_inside = 0;
    printf("hello: %d,( %d, %d )\n", mouse_inside, x, y);
    glutPostRedisplay();

}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);  			    // initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(800,600); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Homework #3"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawTextAndTriangle);	// what the display window has
    glutMouseFunc( mouse );                 // mouse event
	glutMainLoop();				        	// it activates all windows and graphics
	return 0;
}


// for compile :- gcc hw3.c -o hw3 -lGL -lGLU -lglut
// run : ./hw3

