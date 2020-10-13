/*
 a. PROGRAMMER: Timothy Vicars 
 b. COURSE: CSCI 4350-1 
 c. DATE:  11/19/2018 
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
#include<vector>
#include<math.h>

using std::vector;
///////////
static
struct rect {
    const int x,y;
    bool start;
}
wall = {
    .x=750,
    .y=550
},
scene = {
    .x = 800,
    .y = 600
};

struct point {
    int x, y;
    static point* lastpt;
    point(int xin, int yin) {
        x = xin;
        y = yin;
    }
    void toVert() {
        glVertex2i( x , y);
        lastpt = this;
    }
};

struct wallbarrier {
    // playing field
    //   ______________________
    //   | tl   ttr || ttl   tr|
    //   |          ||         |
    //   |      tbl :: tbr     |
    //   |                     |
    //   |                     |
    //   |                     |
    //   |      btl :: btr     |
    //   | bl   bbl || bbr   br|
    //   |__________||_________|
    /*
    point tl,
          bl,
          bbl,
          btl,
          btr,
          bbr,
          br,
          tr,
          ttl,
          tbr,
          tbl,
          ttr;
    */
   
   point topleft,
          bottomright,
          opening;
          //*lastpt;
   int    openingsize,
          openingwidth;
    
    /*void draw() {
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i( tl.x, tl.y );
        glVertex2i( bl.x, bl.y );
        glVertex2i( bbl.x, bbl.y );
        glVertex2i( btl.x, btl.y );
        glVertex2i( btr.x, btr.y );
        glVertex2i( bbr.x, bbr.y );
        glVertex2i( br.x, br.y );
        glVertex2i( tr.x, tr.y );
        glVertex2i( ttl.x, ttl.y);
        tbr.toVert();
        tbl.toVert();
        ttr.toVert();

        glEnd();
    }
    */
    void draw() {
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_LINE_LOOP);
        topleft.toVert();
        glVertex2i(topleft.x,bottomright.y);
        glVertex2i(opening.x-openingwidth, bottomright.y);
        glVertex2i(opening.x-openingwidth, bottomright.y+openingsize/2);
        glVertex2i(opening.x+openingwidth, bottomright.y+openingsize/2);
        glVertex2i(opening.x+openingwidth, bottomright.y);
        bottomright.toVert();
        glVertex2i(bottomright.x, topleft.y);
        glVertex2i(opening.x+openingwidth, topleft.y);
        glVertex2i(opening.x+openingwidth, topleft.y-openingsize/2);
        glVertex2i(opening.x-openingwidth, topleft.y-openingsize/2);
        glVertex2i(opening.x-openingwidth, topleft.y);
        topleft.toVert();
        glEnd();
    }
};

static struct wallbarrier wbar = {
   /* point(25,575), //tl
    point(25,25),  //bl
    point(395,25), //bbl
    point(395,50), //btl
    point(405,50), //btr
    point(405,25), //bbr
    point(775,25), //br
    point(775,575),//tr
    point(405,575),//ttl
    point(405,100),//tbr
    point(395,100),
    point(395,575)*/
    point(25,575),
    point(775,25),
    point(400,300),
    150,
    10
};

static
struct barrier {
    const int
        x,
        width;
    int openingtop,
        openingbottom;

    void draw() {
        glBegin(GL_LINE_LOOP);
            glVertex2f(x, wall.y);
            glVertex2f(x, openingtop);
            glVertex2f(x+width, openingtop);
            glVertex2f(x+width, wall.y);
        glEnd();
    }
} barr {
    .x = 390,
    .width = 10,
    .openingtop = 200,
    .openingbottom = 100
};

//////////
//our ball vector
vector<struct circle> sevenb(7);

// free function degree conversion
float deg2Rad(float degrees) {
  return (degrees * M_PI / 180);
}

// ball struct
struct circle {
    float x,y,
          movx,movy,
          colorr,colorg,colorb;
    int angle = 0;
    static const int segments = 24;
    static const int r = 10;

    void init() {
        //random num generation seed
        //static std::mt19937 rng(std::time(nullptr));
        static std::default_random_engine rng;
        // gen numbers between playable area in x direction
        static std::uniform_int_distribution<int>
            genx( scene.x - barr.x + r, wall.x - r);
        // gen numbers between playable area in y direction
        static std::uniform_int_distribution<int> 
            geny( scene.y - wall.y + r, wall.y - r);


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
        static std::uniform_real_distribution<float> genangle( 1, 5 ); // in radians;

        // random gen the start position of the circle
        x = genx(rng);
        y = geny(rng);
        // random gen the movement slope
        float radians = genangle(rng );
        movx = cosf( radians ) * 3;
        movy = sinf( radians ) * 3;

        // genc is for random colors
        std::uniform_real_distribution<float> genc( 0.0 , 0.9);
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
        const int centerxr = x+movx+r;
        const int centerxl = x+movx-r;
        const int centeryt = y+movy+r;
        const int centeryb = y+movy-r;
        const int barrierBt= wbar.bottomright.y+topleft.y-openingize/2;
        if ((centerxl < wbar.topleft.x) || (centerxr > wbar.bottomright.x))
            movx = -movx; //ball hit outside left or right wall
        if ((centeryb < wbar.bottomright.y) || (centeryt > wbar.topleft.y))
            movy = -movy; //ball hit outside top or bottom wall
        if ((centeryt < barrierBt)) {
            if ((centerxr > wbar.opening
            if ((centerxr > wbar.ttr.x ) && (centerxl < wbar.ttl.x))
                movx = -movx; //we hit the barrier from left or right


        /*if ((centeryt < wbar.btl.y) && (centerxr < wbar.btr.x)) {
            //bottom left quadrent of the playing screen
            // we can only bounce of 3 walls
            if ((centerxr < wbar.bl.x) || (centerxr > wbar.btl.x))
                movx = -movx; // we bounced off the left wall or barrior
            if ((centeryb < wbar.bl.y))
                movy = -movy; //bounced off the bottom
        }
        else if ((centery < wbar.btl.y) && (centerx > wbar.btl.x)) {
            //bottom right quad
            if ((centerxl < wbar.btr.x) || (centerxr > wbar.br.x))
                movx = -movx; //bounced off the right wall
            if ((centery < wbar.bl.y))
                movy = -movy; //bounced off the bottom
        }

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
      */
        // x+movx = centerx
        // y+movy = centery
        // wall.x = right wall
        // wall.y = top wall
        // scene.x - wall.x = left wall
        // scene.y - wall.y = bottom wall
        const int rwall = wall.x - r;
        const int lwall = scene.x - wall.x + r;
        const int twall = wall.y - r;
        const int bwall = scene.y - wall.y + r;
        const int rbar  = barr.x + 10 + r;
        const int lbar  = barr.x - r;
        const int tbar  = barr.openingtop - r;
        const int bbar  = barr.openingbottom + r;

        //we should check which side of the board
        //the circle is on to determine which walls
        //it can collide with
        /*if (centerx < lbar) {
            //we are on the left side
            if (centerx < lwall)
                movx = -movx;
            if ((centery > twall) || (centery < bwall))
                movy = -movy;
        }*/
        /*if ((centerx < lwall) || (centerx > rwall))
            movx = -movx;
        if ((centery < bwall) || (centery > twall))
            movy = -movy;
        if (( centery > tbar ) || (centery < bbar))
            if (( centerx > lbar ) && ( centerx < rbar) )
                    movx = -movx;

        if ((centerx > rwall) || (centerx < lwall))
            movx = -movx;
        if ((centery > twall) || (centery < bwall))
            movy = -movy;
        if ((centerx < rbar) )
            movx = -movx;
            */
      /*switch ( centerx ) {
      case rwall: //horizontal wall
      //case r+wall.x:
        movx = -movx;
      default:
        break;
      }
      switch ( y + movy) {
          case r+wall.y: // top wall
          case r-scene.y-wall.y:
              movy = -movy;
          default:
              break;
      }
      */
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
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(scene.x-wall.x, scene.y-wall.y);
        glVertex2f(scene.x-wall.x, wall.y);
        glVertex2f(wall.x, wall.y);
        glVertex2f(wall.x, scene.y-wall.y);
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
        //drawWall();
        wbar.draw();

        //draw the circles
        for (auto &k : sevenb)
            k.draw();
        //barr.draw();
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
	glutCreateWindow("Homework #7"); 		// create the Display window with title
	init();
	glutDisplayFunc(drawing);			// what the display window is conatin
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);
	glutMainLoop();					// it activates all windows and graphics
	return 0;
}

// this is a c++ file so use G++ or clang++
// for compile :- g++ hw7.cpp -o hw7 -lGL -lGLU -lglut -lm
// OR $ make
// run : ./hw7

