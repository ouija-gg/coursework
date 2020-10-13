#include <GL/glut.h>
#include <math.h>
#include <random>
#include <vector>
#include "gameconst.h"

struct circle {
    float x,y,
          movx,movy,
          colorr,colorg,colorb;
    const int segments = 24;
    const int r = 10;
    bool between(int x, int min, int max);
    void init();
    void draw();
    void move();
};

using std::vector;

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
	//else if ((movx > 0) && between( x+movx+r, wall_mid, wall_mid
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
	x += movx;
	y += movy;
}
