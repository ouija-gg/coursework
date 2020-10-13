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

#include<GL/glut.h>
#include<string>
#include<ctime>
#include<random>

///////////

static struct rect {
    int x,y;
    bool start;
} scene = { 
    .x = 800,
    .y = 600
},
wall = {
    .x=750,
    .y=550
};

static struct circle {
    float x,y,
          movx,movy,
          colorr,colorg,colorb;
    const int segments,r;

    void init() {
        //random num generation seed
        std::mt19937 rng(std::time(nullptr));
        // gen numbers between playable area in x direction
        std::uniform_int_distribution<int> genx( scene.x - wall.x + r, wall.x - r);
        // gen numbers between playable area in y direction
        std::uniform_int_distribution<int> geny( scene.y - wall.y + r, wall.y - r);

        //random movement directions between 2 and 5
        //to be added to the ball's X,Y values.
        //Using a negative minimum lets the distribution include numbers
        //that are too close to 0 and makes the ball move too slowly, 
        //so we avoid that by using only positive numbers.
        std::uniform_real_distribution<float> genm( 2.0 , 5.0);
        // genc is for random colors
        std::uniform_real_distribution<float> genc( 0.0 , 0.9);

        // random gen the start position of the circle
        x = genx(rng);
        y = geny(rng);
        // random gen the movement slope
        movx = genm(rng);
        movy = genm(rng);
        // generate the color
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
        if (( x + r + movx > wall.x ) || (x - r + movx < (scene.x-wall.x))){
            //we hit the left or right wall
            //so reverse the x mov direction
            movx = -movx;
        } 
        else if (( y +r + movy > wall.y )||( y -r +movy < (scene.y-wall.y))) {
            //we hit the top or bottom
            //so reverse y mov direction
            movy = -movy;
        }
        x += movx;
        y += movy;
    }
} myball = {
    // instantiate the ball from circle struct
    .segments = 256,
    .r = 100,
};
/////////////////

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,scene.x,0.0,scene.y);
    myball.init();
}

void drawWall() {
    // a simple line loop
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(scene.x-wall.x, scene.y-wall.y);
        glVertex2f(scene.x-wall.x, wall.y);
        glVertex2f(wall.x, wall.y);
        glVertex2f(wall.x, scene.y-wall.y);
    glEnd();
}

void drawText( ) {
     const std::string msg = "Click to see the bouncing ball";

     glColor3f( 0, 0, 1.0 ); //Blue
     glRasterPos2f( scene.x/2, scene.y/2);
     for (auto k : msg)
         glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, k );
}

void drawing( ) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (scene.start) {
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
        scene.start = true;
}

int main(int argc, char** argv)
{
    //random seed
	glutInit(&argc, argv);  			// initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(scene.x,scene.y); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Homework #5"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawing);			// what the display window is conatin
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}

// this is a c++ file so use G++ or clang++
// for compile :- g++ hw5.cpp -o hw5 -lGL -lGLU -lglut -lm
// OR $ make
// run : ./hw5

