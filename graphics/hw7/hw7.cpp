/*
 a. PROGRAMMER: Timothy Vicars 
 b. COURSE: CSCI 4350-1 
 c. DATE:  11/27/2018 
 d. ASSIGNMENT: Homework #7
 e. ENVIRONMENT: Linux Clang/GCC 
 f. FILES INCLUDED: hw7.cpp , Makefile
 g. PURPOSE: this program draws seven "bouncing circles" inside a box
 h. INPUT:  
 i. PRECONDITIONS:  
 j. OUTPUT: the output is a graphical window  that displays 7
            bouncing balls
 n.  EXAMPLE: ./hw7
*/

#include<GL/glut.h>
#include<string>
#include<ctime>
#include<random>
#include <vector>

using std::vector;
///////////

namespace game {
    const int scenex = 800;
    const int sceney = 600;
    const int wall_left = 25;
    const int wall_right=775;
    const int wall_mid=scenex/2;
    const int wall_top = sceney-25;
    const int wall_bot = 25;
}
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

//free util functions
bool between(int x, int min, int max) {
    if ((x >= min) && (x <= max))
        return true;
    else
        return false;
}

//////////
//our ball vector
vector<struct circle> sevenb(7);


struct circle {
    float x,y,
          movx,movy,
          colorr,colorg,colorb;
    const int segments = 24;
    const int r = 10;

    void init() {
        using namespace game;
        //random num generation seed
         static std::default_random_engine rng;
        // gen numbers between playable area in x direction
        static std::uniform_int_distribution<int>
        genx( wall_mid+5+r, wall_right-r);
        // gen numbers between playable area in y direction
        static std::uniform_int_distribution<int>
        geny( wall_bot, wall_top );

		//this is a really stupid vector that holds a bunch of
        //viable radian values. these values are vaiable because
        //they do not contain values that are near or equal
        //to flat angles, which makes the game more interesting, 
        //and keeps balls from getting "stuck" bouncing up and down.
        static vector<float> usuable_angles = {
        /*     M_PI/6,   M_PI/4,   M_PI/3,
             5*M_PI/6, 3*M_PI/4, 2*M_PI/3,
             7*M_PI/6, 5*M_PI/4, 4*M_PI/3,
            11*M_PI/6, 7*M_PI/4, 5*M_PI/3 */
        };

        //this will make 4 runs, first run between 0.3 and 1.3,
        //the second between 2.8 and 3.8, etc.
        for (float k : {1.0/6, 2.0/3, 7.0/6}) {
            usuable_angles.push_back(M_PI*k);
            usuable_angles.push_back(M_PI*k+M_PI/12);
            usuable_angles.push_back(M_PI*k+M_PI/6);
        }


        static vector<int> weights = {};
        for (auto k : usuable_angles)
            weights.push_back(1);

        static std::piecewise_constant_distribution<double>
            distrangles(usuable_angles.begin(),usuable_angles.end(),weights.begin());
 
        /****************************************
        We need to generate a random angle. We use the angle
        to generate X and Y movements. Using a random angle
        let's us make the circles all have the same speed, whereas
        generating random +X and +Y values will create circles that are
        faster or slower than each other.
         ****************************************/

        // genc is for random colors
        std::uniform_real_distribution<float> genc( 0.0 , 0.9);

        // random gen the start position of the circle
        x = genx(rng);
        y = geny(rng);
        // random gen the movement slope
        float radians = distrangles(rng );
        movx = cosf( radians ) * 3;
        movy = sinf( radians ) * 3;

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
        using namespace game;
        if (( x + r + movx > wall_right ) || (x - r + movx < (wall_left))){
            //we hit the left or right wall
            //so reverse the x mov direction
            movx = -movx;
        } 
        else if (( y +r + movy > wall_top)||( y -r +movy < wall_bot)) {
            //we hit the top or bottom
            //so reverse y mov direction
            movy = -movy;
        }
        //else if ((movx > 0) && between( x+movx+r, wall_mid, wall_mid
        else if (between( (x + movx), wall_mid-5, wall_mid+5)
                && between( (y+r +movy), wall_bot, sceney/2)) {
            movx = -movx;
        }
        x += movx;
        y += movy;
    }
}; 
 
/////////////////

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,scene.x,0.0,scene.y);
    // create the 7 balls and init them
    for (auto &k : sevenb) {
        k.init();
#ifdef DEBUG
        printf("%f.2 , %f.2 \n",k.x,k.y);
#endif
    }


}

void drawWall() {
    // a simple line loop
    using namespace game;
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(wall_left, wall_bot);
        glVertex2f(wall_mid-5, wall_bot); //barrier
        glVertex2f(wall_mid-5, sceney/2); //top of the barrier
        glVertex2f(wall_mid+5, sceney/2);
        glVertex2f(wall_mid+5,wall_bot);
        glVertex2f(wall_right,wall_bot);
        glVertex2f(wall_right,wall_top);
        glVertex2f(wall_left,wall_top);
        glVertex2f(wall_left,wall_bot);
    glEnd();
}

void drawCircles() {
    for (auto &k : sevenb)
        k.draw();
}

void drawText( ) {
     const std::string msg = "Click to see the bouncing ball";

     glColor3f( 0, 0, 1.0 ); //Blue
     glRasterPos2f( scene.x/2, scene.y/2);
     for (auto &k : msg)
         glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, k );
}

void drawing( ) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (scene.start) {
        drawWall();
        //draw the circles
        for (auto &k : sevenb)
            k.draw();
    }
    else
        drawText();
    glFlush();
}
void timer(int t) {
    // move all the balls
    if (scene.start)
        for (auto &k : sevenb)
            k.move();
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
// for compile :-   g++ -std=gnu++11 -o hw7 hw7.cpp -lGLU -lGL -lglut -lm
// run : ./hw7

