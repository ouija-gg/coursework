/*
 a. PROGRAMMER: Timothy Vicars 

 b. COURSE: CSCI 4350-1 
 c. DATE:  10/25/2018 

 d. ASSIGNMENT: Homework #4 

 e. ENVIRONMENT: Linux Clang/GCC 

 f. FILES INCLUDED: hw4.c 

 g. PURPOSE: this program draws a "bouncing circle" when the mouse is clicked in the window. 

 h. INPUT:  

 i. PRECONDITIONS:  

 j. OUTPUT: the output is a graphical window 

 n.  EXAMPLE: ./hw4
*/


#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<string.h>
#include<math.h>

///////////////////
float loc = 100;
int bouncing = 0;
///////////////////

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,800.0,0.0,600.0);
}

void drawText( ) {
	const char *string = "Click to see the bouncing ball";
	int j = strlen( string );
 
	glColor3f( 0, 0, 1.0 ); //Blue
	glRasterPos2f( 75, 40 );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , string[i] );
	}
//	glFlush();
}

void drawCircle(float fx, float fy, float r) {
    int segments = 256;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * (i) / segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);

        glVertex2f(x + fx, y + fy);
    }
    glEnd();
}
void drawing( ) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (bouncing)
        drawCircle( 400, loc, 100);
    else
        drawText( );
    glFlush();
}
void timer(int t) {
    float movement = (float)(sinf(t/30)*5);
    loc += movement;
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, t+1);
}
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        bouncing = 1;
    //start the animation
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);  			// initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(800,600); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Homework #4"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawing);			// what the display window is conatin
    glutTimerFunc(100, timer, 0);
    glutMouseFunc(mouse);
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}


// for compile :- gcc hw4.c -o hw4 -lGL -lGLU -lglut -lm
// run : ./hw4

