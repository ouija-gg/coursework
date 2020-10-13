/*
 a. PROGRAMMER: Timothy Vicars 
 b. COURSE: CSCI 4350-1 
 c. DATE:  11/06/2018 
 d. ASSIGNMENT: Homework #5 
 e. ENVIRONMENT: Linux Clang/GCC 
 f. FILES INCLUDED: hw5.cpp
 g. PURPOSE: this program draws a "bouncing circle" inside a box
 h. INPUT:  
 i. PRECONDITIONS:  
 j. OUTPUT: the output is a graphical window 
 n.  EXAMPLE: ./hw5
*/

#include<stdlib.h>
#include<cstdlib>
#include<GL/glut.h>
#include<string.h>
#include<math.h>
#include<ctime>
#include<random>

///////////

static struct rect {
    int x,y;
    bool start;
} dim = { .x = 800,
          .y = 600 },
wall = {
    .x=750,
    .y=550
};

static struct circle {
    float x,y;
    const int segments,r;
    float movx,movy;
    float colorr,colorg,colorb;

    void init() {
        //random num generation
        std::mt19937 rng(std::time(0));
        std::uniform_int_distribution<int> genx( dim.x - wall.x + r, wall.x - r);
        std::uniform_int_distribution<int> geny( dim.y - wall.y + r, wall.y - r);
        std::uniform_real_distribution<float> genm( -3.0 , 3.0);
        std::uniform_real_distribution<float> genc( 0.0 , 0.9);

        x = genx(rng);
        y = geny(rng);
        movx = genm(rng);
        movy = genm(rng);
        colorr = genc(rng);
        colorg = genc(rng);
        colorb = genc(rng);
    }

    void draw() {
        glColor3f( colorr, colorg, colorb ); //random color
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x,y);
        for (int i = 0; i <= segments; i++) {
            float theta = 2.0f * M_PI * (i) / segments;
            float fx = r * cosf(theta);
            float fy = r * sinf(theta);

            glVertex2f(x + fx, y + fy);
        }
        glEnd();
    }
    void move() {
        if (( x + r + movx > wall.x ) || (x - r + movx < (dim.x-wall.x))){
            //we hit the left or right wall
            movx = -movx;
        } 
        else if (( y +r + movy > wall.y )||( y -r +movy < (dim.y-wall.y))) {
            //we hit the top or bottom
            movy = -movy;
        }
        x += movx;
        y += movy;
    }
} myball = {
    .segments = 256,
    .r = 100,
};
/////////////////

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,dim.x,0.0,dim.y);
    myball.init();
}

void drawWall() {
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(dim.x-wall.x, dim.y-wall.y);
        glVertex2f(dim.x-wall.x, wall.y);
        glVertex2f(wall.x, wall.y);
        glVertex2f(wall.x, dim.y-wall.y);
    glEnd();
}

void drawText( ) {
     const char *string = "Click to see the bouncing ball";
     int j = strlen( string );

     glColor3f( 0, 0, 1.0 ); //Blue
     glRasterPos2f( 75, 40 );
     for( int i = 0; i < j; i++ ) {
         glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , string[i] );
     }
}

void drawing( ) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (dim.start) {
        drawWall();
        myball.draw();
    }
    else 
        drawText();
    glFlush();
}
void timer(int t) {
    myball.move();
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, t+1);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        dim.start = true;
}

int main(int argc, char** argv)
{
    //random seed
	glutInit(&argc, argv);  			// initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(dim.x,dim.y); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Homework #5"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawing);			// what the display window is conatin
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}


// for compile :- g++ hw5.cpp -o hw4 -lGL -lGLU -lglut -lm
// OR $ make
// run : ./hw5

