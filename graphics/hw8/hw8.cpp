/*
 a. PROGRAMMER: Timothy Vicars 
 b. COURSE: CSCI 4350-1 
 c. DATE:  12/7/2018 
 d. ASSIGNMENT: Homework #8
 e. ENVIRONMENT: Linux Clang/GCC 
 f. FILES INCLUDED: hw8.cpp 
 g. PURPOSE: this program runs a "bouncing circles" game
 h. INPUT:  
 i. PRECONDITIONS:  
 j. OUTPUT: the output is a graphical window  that displays a
            bouncing balls game
 n.  EXAMPLE: ./hw8
*/

#include<GL/glut.h>
#include<string>
#include<ctime>
#include<random>
#include <vector>
using std::vector;

///////////
struct circle {
    float x,y,
          movx,movy,
          colorr,colorg,colorb;
    const int segments = 24;
    const int r = 10;
    bool captured = false;
    bool between(int x, int min, int max);
    void init();
    void draw();
    void move();
};
namespace game {
    const int scenex = 800;
    const int sceney = 600;
    const int wall_left = 25;
    const int wall_right=775;
    const int wall_mid=scenex/2;
    const int wall_top = sceney-25;
    const int wall_bot = 25;
    enum State {
        intro,
        start,
        point,
        end
    };

    struct gate {
        int open_bot,
            open_top;
        bool opened = false;
        void open_toggle(int y) {
            if (opened) {
                opened = false;
            }
            else {
                //invert Glut Y because it is opposite
                //of opengl
                y = game::sceney - y;
                if (y < 50) {
                    open_bot = wall_bot;
                    open_top = 100;
                }
                else if (y > wall_top-100) {
                    open_top = wall_top;
                    open_bot = open_top - 100;
                }
                else {
                    open_top = y + 49;
                    open_bot = y - 50;
                }
                opened = true;
            }
        }
    };
}

//start game in intro state
namespace game {
    State gamestate = intro;
    gate opening = gate();
    vector<circle*> sevenb;
    int points = 0;
}
 
//free functions for the game
void toStart() {
    using namespace game;
    circle* startc = new circle;
    startc->init();
    sevenb.push_back(startc);
    gamestate = start;
}
void onPoint() {
    game::points++;
    toStart();
}
void toEnd() {
    game::gamestate = game::end;
}

/////////////////
//circle

bool circle::between(int x, int min, int max) {
    if ((x >= min) && (x <= max))
        return true;
    else
        return false;
}

void circle::init() {
	using namespace game;
	//random num generation seed
	 static std::default_random_engine rng;
	// gen numbers between playable area in x direction
	static std::uniform_int_distribution<int>
	genx( wall_mid+100, wall_right-r);
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

void circle::draw() {
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
void circle::move() {
	using namespace game;
	const int ballRight = x + r + movx;
	const int ballLeft  = x - r + movx;
	const int ballTop   = y + r + movy;
	const int ballBot   = y - r + movy;

	if (( ballRight > wall_right ) || (ballLeft < (wall_left))){
		//we hit the left or right wall
		//so reverse the x mov direction
		movx = -movx;
	} 
	else if (( ballTop > wall_top)||( ballBot < wall_bot)) {
		//we hit the top or bottom
		//so reverse y mov direction
		movy = -movy;
	}
	else if (between(x+movx, wall_mid-10, wall_mid+10)) {
		if (opening.opened) {
			if (between(y, wall_bot, opening.open_bot)) {
				//we hit the bottom barrier
				movx = -movx;
			}
			else if (ballBot <= opening.open_bot) {
				// we hit the topcap of the bottom barrier
				movy= -movy;
			}
			else if (between(y, opening.open_top, wall_top)) {
				//we hit the top barrier
				movx=-movx;
			}
			else if (ballTop >= opening.open_top) {
				//we hit the bottom of the top barrier
				movy=-movy;
			}
		}
		else
			movx = -movx;
	}
    if (!captured && between(x,wall_left,wall_mid-5)) {
        captured = true;
        //spawn a new ball
        onPoint();
    }

    if (captured && between(x,wall_mid+25, wall_right)) {
        //the ball became loose
        toEnd();
    }
	x += movx;
	y += movy;
}
/////////////////
// end circle

void init()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,game::scenex,0.0,game::sceney);
}

void drawWall() {
    // a simple line loop
    using namespace game;
    glColor3f(0.0,0.0,0.0);
    //TODO:
    //use an IF to draw opening or just
    //the plain wall
    if (game::opening.opened) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(wall_left, wall_bot);
        glVertex2f(wall_mid-5, wall_bot); //barrier
        glVertex2f(wall_mid-5, opening.open_bot); //top of the barrier
        glVertex2f(wall_mid+5, opening.open_bot);
        glVertex2f(wall_mid+5,wall_bot);
        // right wall
        glVertex2f(wall_right,wall_bot);
        glVertex2f(wall_right,wall_top);
        // top wall
        glVertex2f(wall_mid+5,wall_top);
        glVertex2f(wall_mid+5,opening.open_top);
        glVertex2f(wall_mid-5,opening.open_top);
        glVertex2f(wall_mid-5,wall_top);
        //left wall, end
        glVertex2f(wall_left,wall_top);
        glVertex2f(wall_left,wall_bot);
        glEnd();
    }
    else {
        glBegin(GL_LINE_LOOP);
        glVertex2f(wall_left, wall_bot);
        glVertex2f(wall_mid-5,wall_bot);
        glVertex2f(wall_mid-5,wall_top);
        glVertex2f(wall_left,wall_top);
        glVertex2f(wall_left,wall_bot);
        glEnd();
        glBegin(GL_LINE_LOOP);
        glVertex2f(wall_mid+5, wall_bot);
        glVertex2f(wall_right, wall_bot);
        glVertex2f(wall_right, wall_top);
        glVertex2f(wall_mid+5, wall_top);
        glVertex2f(wall_mid+5, wall_bot);
        glEnd();
    }
}

void drawCircles() {
    using namespace game;
    for (auto &k : sevenb)
        k->draw();
}

void drawText( ) {
     const std::string msg = "Click to Start the game";

     glColor3f( 0, 0, 1.0 ); //Blue
     glRasterPos2f( game::scenex/2, game::sceney/2);
     for (auto &k : msg)
         glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, k );
}

void drawPoints() {
    const std::string msg = "POINTS: " + std::to_string(game::points);
    glColor3f( 0.5, 0.7, 0.7);
    glRasterPos2f( game::wall_left, game::wall_top);
    for (auto &k : msg)
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, k );
}

void drawing( ) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (game::gamestate != game::intro) {
        drawPoints();
        drawWall();
        //draw the circles
        using namespace game;
        for (auto &k : sevenb)
            k->draw();
    }
    else if (game::gamestate == game::intro)
        drawText();
    glFlush();
}
void timer(int t) {
    // move all the balls
    using namespace game;
    if (gamestate == start)
        for (auto &k : sevenb)
            k->move();
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, t+1);
}

void mouse(int button, int state, int x, int y) {
    using namespace game;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        switch (gamestate) {
            case intro:
                toStart();
                break;
            case start:
                opening.open_toggle(y);
                break;
            case end:
            default:
                break;
        }
    }

}

int main(int argc, char** argv)
{
    //random seed
	glutInit(&argc, argv);  			// initialize glut
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 	// for buffering and to choose color mode (set the display mode)
	glutInitWindowSize(game::scenex,game::sceney); 			// intial pixel width and height of the display window
	glutInitWindowPosition(80,80);    		// intial poistion for window
	glutCreateWindow("Homework #8"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawing);			// what the display window is conatin
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}

// this is a c++ file so use G++ or clang++
// for compile :-   g++ -std=gnu++11 -o hw8 hw8.cpp -lGLU -lGL -lglut -lm
// run : ./hw8
