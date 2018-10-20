//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//mod spring 2018: X11 wrapper class
//This program is a game starting point for a 3350 project.
//
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;
#include <unistd.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];
//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
							 (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float TIMESLICE = 1.0f;
const float GRAVITY = -0.6f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 1000;
const Flt MINIMUM_ASTEROID_SIZE = 10.0;
//-----------------------------------------------------------------------------
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
class Image {
public:
   int width, height;
   unsigned char *data;
   ~Image() { delete [] data; }
   Image(const char *fname) {
   if (fname[0] == '\0')
   return;
  //printf("fname **%s**\n", fname);
   int ppmFlag = 0;
   char name[40];
  strcpy(name, fname);
  int slen = strlen(name);
   char ppmname[80];
  if (strncmp(name+(slen-4), ".ppm", 4) == 0)
   ppmFlag = 1;
  if (ppmFlag) {
   strcpy(ppmname, name);
  } else {
   name[slen-4] = '\0';
   //printf("name **%s**\n", name);
   sprintf(ppmname,"%s.ppm", name);
  //printf("ppmname **%s**\n", ppmname);
   char ts[100];
   //system("convert eball.jpg eball.ppm");
  sprintf(ts, "convert %s %s", fname, ppmname);
   system(ts);
   }
  //sprintf(ts, "%s", name);
   FILE *fpi = fopen(ppmname, "r");
   if (fpi) {
   char line[200];
   fgets(line, 200, fpi);
   fgets(line, 200, fpi);
  //skip comments and blank lines
   while (line[0] == '#' || strlen(line) < 2)
   fgets(line, 200, fpi);
  sscanf(line, "%i %i", &width, &height);
   fgets(line, 200, fpi);
  //get pixel data
   int n = width * height * 3;
   data = new unsigned char[n];
   for (int i=0; i<n; i++)
   data[i] = fgetc(fpi);
   fclose(fpi);
  } else {
   printf("ERROR opening image: %s\n",ppmname);
  exit(0);
   }
   if (!ppmFlag)
   unlink(ppmname);
  }
};
Image img[6]={"art.jpg","joel_pic.jpg","edwinImg.jpg","bryan_picture.jpg","andrew_picture.jpg", "character.jpg"};
class Global {
	public:
		GLuint characterTexture;
	    GLuint artTexture;
	    GLuint bryanTexture;
	    GLuint joelTexture;
	    GLuint andrewTexture;
	    GLuint edwinTexture;
		int xres, yres;
		int credits;
		char keys[65536];
		Global() {
			credits=0;
			xres = 1250;
			yres = 900;
			memset(keys, 0, 65536);
		}
} gl;
class Ship {
	public:
		Vec dir;
		Vec pos;
		Vec vel;
		float angle;
		float color[3];
	public:
		Ship() {
			VecZero(dir);
			pos[0] = (Flt)(gl.xres/2);
			pos[1] = (Flt)(gl.yres/2);
			pos[2] = 0.0f;
			VecZero(vel);
			angle = 0.0;
			color[0] = color[1] = color[2] = 1.0;
		}
};
class Bullet {
	public:
		Vec pos;
		Vec vel;
		float color[3];
		struct timespec time;
	public:
		Bullet() { }
};
class Asteroid {
	public:
		Vec pos;
		Vec vel;
		int nverts;
		Flt radius;
		Vec vert[8];
		float angle;
		float rotate;
		float color[3];
		struct Asteroid *prev;
		struct Asteroid *next;
	public:
		Asteroid() {
			prev = NULL;
			next = NULL;
		}
};
class Game {
	public:
		int astr_destroyed;
		Ship ship;
		Asteroid *ahead;
		Bullet *barr;
		Bullet *barrAst;
		int nasteroids;
		int nbullets;
		int astBull;
		struct timespec bulletTimer;
		struct timespec enemyBulletTimer;
		struct timespec mouseThrustTimer;
		bool mouseThrustOn;
	public:
		Game() {
			astr_destroyed=0;
			ahead = NULL;
			barr = new Bullet[MAX_BULLETS];
			barrAst = new Bullet[MAX_BULLETS];
			nasteroids = 0;
			nbullets = 0;
			astBull = 0;
			mouseThrustOn = false;
			//build 10 asteroids...
			for (int j=0; j<3; j++) { //Change num asteroid
				Asteroid *a = new Asteroid;
				a->nverts = 8;
				a->radius = rnd()*80.0 + 40.0;
				//a->radius=10.0;
				Flt r2 = a->radius / 2.0;
				Flt angle = 0.0f;
				Flt inc = (PI * 2.0) / (Flt)a->nverts;
				for (int i=0; i<a->nverts; i++) {
					a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
					a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
					angle += inc;
				}
				a->pos[0] = (Flt)(rand() % gl.xres);
				a->pos[1] = (Flt)(rand() % gl.yres);
				a->pos[2] = 0.0f;
				a->angle = 0.0;
				a->rotate = rnd() * 4.0 - 2.0;
				a->color[0] = 0.8;
				//a->color[1] = 0.8;
				a->color[1] = 0.8;
				a->color[2] = 0.7;
				a->vel[0] = (Flt)(rnd()*2.0-1.0);
				a->vel[1] = (Flt)(rnd()*2.0-1.0);
				//std::cout << "asteroid" << std::endl;
				//add to front of linked list
				a->next = ahead;
				if (ahead != NULL)
					ahead->prev = a;
				ahead = a;
				++nasteroids;
			}
			clock_gettime(CLOCK_REALTIME, &bulletTimer);
			clock_gettime(CLOCK_REALTIME, &enemyBulletTimer);
		}
		~Game() {
			delete [] barr;
			delete [] barrAst;
		}
} g;
//X Windows variables
class X11_wrapper {
	private:
		Display *dpy;
		Window win;
		GLXContext glc;
	public:
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setup_screen_res(gl.xres, gl.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				std::cout << "\n\tcannot connect to X server" << std::endl;
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				std::cout << "\n\tno appropriate visual found\n" << std::endl;
				exit(EXIT_FAILURE);
			}
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
				StructureNotifyMask | SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
					vi->depth, InputOutput, vi->visual,
					CWColormap | CWEventMask, &swa);
			set_title();
			glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			show_mouse_cursor(0);
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void set_title() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "Asteroids template");
		}
		void check_resize(XEvent *e) {
			//The ConfigureNotify is sent by the
			//server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != gl.xres || xce.height != gl.yres) {
				//Window size did change.
				reshape_window(xce.width, xce.height);
			}
		}
		void reshape_window(int width, int height) {
			//window has been resized.
			setup_screen_res(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
			set_title();
		}
		void setup_screen_res(const int w, const int h) {
			gl.xres = w;
			gl.yres = h;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void set_mouse_position(int x, int y) {
			XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
		}
		void clear_screen() {
			XClearWindow(dpy, win);
		}
		void show_mouse_cursor(const int onoff) {
			if (onoff) {
				//this removes our own blank cursor.
				XUndefineCursor(dpy, win);
				return;
			}
			//vars to make blank cursor
			Pixmap blank;
			XColor dummy;
			char data[1] = {0};
			Cursor cursor;
			//make a blank cursor
			blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
			if (blank == None)
				std::cout << "error: out of memory." << std::endl;
			cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
			XFreePixmap(dpy, blank);
			//this makes you the cursor. then set it using this function
			XDefineCursor(dpy, win, cursor);
			//after you do not need the cursor anymore use this function.
			//it will undo the last change done by XDefineCursor
			//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
		}
} x11;
//function prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	init_opengl();
	srand(time(NULL));
	x11.set_mouse_position(100, 100);
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}
void init_opengl()
{
	//ART TEXTURE
	//
	glGenTextures(1,&gl.artTexture);
	int w = img[0].width;
   	int h = img[0].height;
	glBindTexture(GL_TEXTURE_2D, gl.artTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
   	GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	// Bryan TEXTURE
	//
   	glGenTextures(1,&gl.bryanTexture);
    w = img[3].width;
    h = img[3].height;
   	glBindTexture(GL_TEXTURE_2D, gl.bryanTexture);

   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
   	GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
	// Andrew TEXTURE
   	//
   	glGenTextures(1,&gl.andrewTexture);
    w = img[4].width;
    h = img[4].height;
   	glBindTexture(GL_TEXTURE_2D, gl.andrewTexture);

   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
   	GL_RGB, GL_UNSIGNED_BYTE, img[4].data);
	// Joel TEXTURE
   	//
   	glGenTextures(1,&gl.joelTexture);
    w = img[1].width;
    h = img[1].height;
   	glBindTexture(GL_TEXTURE_2D, gl.joelTexture);

   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
   	GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
	// edwin TEXTURE
   	//
   	glGenTextures(1,&gl.edwinTexture);
    w = img[2].width;
    h = img[2].height;
   	glBindTexture(GL_TEXTURE_2D, gl.edwinTexture);

   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
   	GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

	//Main character Texture
	glGenTextures(1,&gl.characterTexture);
    w = img[5].width;
    h = img[5].height;
	glBindTexture(GL_TEXTURE_2D, gl.characterTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	GL_RGB, GL_UNSIGNED_BYTE, img[5].data);

   	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}
void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}
void check_mouse(XEvent *e)
{
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	static int ct=0;
	if (e->type != ButtonPress &&
			e->type != ButtonRelease &&
			e->type != MotionNotify)
		return;
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			//a little time between each bullet
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&g.bulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {
					Bullet *b = &g.barr[g.nbullets];
					timeCopy(&b->time, &bt);
					b->pos[0] = g.ship.pos[0];
					b->pos[1] = g.ship.pos[1];
					b->vel[0] = g.ship.vel[0];
					b->vel[1] = g.ship.vel[1];
					//convert ship angle to radians
					Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f + rnd()*0.1;
					b->vel[1] += ydir*6.0f + rnd()*0.1;
					b->color[0] = 1.0f;
					b->color[1] = 1.0f;
					b->color[2] = 1.0f;
					++g.nbullets;
				}
			}
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (e->type == MotionNotify) {
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			//Mouse moved
			int xdiff = savex - e->xbutton.x;
			//int ydiff = savey - e->xbutton.y;
			if (++ct < 10)
				return;
			if (xdiff > 0) {
				//mouse moved along the x-axis.
				g.ship.angle += 0.05f * (float)xdiff;
				if (g.ship.angle >= 360.0f)
					g.ship.angle -= 360.0f;
			}
			else if (xdiff < 0) {
				g.ship.angle += 0.05f * (float)xdiff;
				if (g.ship.angle < 0.0f)
					g.ship.angle += 360.0f;
			}
			// if (ydiff > 0) {
			// 	//mouse moved along the y-axis.
			// 	//apply thrust
			// 	//convert ship angle to radians
			// 	Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
			// 	//convert angle to a vector
			// 	Flt xdir = cos(rad);
			// 	Flt ydir = sin(rad);
			// 	g.ship.vel[0] += xdir * (float)ydiff * 0.001f;
			// 	g.ship.vel[1] += ydir * (float)ydiff * 0.001f;
			// 	Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
			// 			g.ship.vel[1]*g.ship.vel[1]);
			// 	if (speed > 15.0f) {
			// 		speed = 15.0f;
			// 		normalize2d(g.ship.vel);
			// 		g.ship.vel[0] *= speed;
			// 		g.ship.vel[1] *= speed;
			// 	}
			// 	g.mouseThrustOn = true;
			// 	clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
			// }
			x11.set_mouse_position(100, 100);
			savex = savey = 100;
		}
	}
}
int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	gl.keys[key]=1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_f:
			break;
		case XK_c:
			extern void toggleCredits();
			toggleCredits();
			break;
		case XK_s:
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
}
void deleteAsteroid(Game *g, Asteroid *node)
{
	//Remove a node from doubly-linked list.
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ahead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ahead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
		g->astr_destroyed+=1;
}
void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0;
	Flt r2 = ta->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
	ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0;
	ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
	ta->color[0] = 0.8;
	ta->color[1] = 0.8;
	ta->color[2] = 0.7;
	ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
	ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
}
void physics()
{
	Flt d0,d1,dist;
	//Update ship position
	g.ship.pos[0] += g.ship.vel[0];
	g.ship.pos[1] += g.ship.vel[1];
	//Check for collision with window edges
	if (g.ship.pos[0] < 0.0) {
		g.ship.pos[0] += (float)gl.xres;
	}
	else if (g.ship.pos[0] > (float)gl.xres) {
		g.ship.pos[0] -= (float)gl.xres;
	}
	else if (g.ship.pos[1] < 0.0) {
		g.ship.pos[1] += (float)gl.yres;
	}
	else if (g.ship.pos[1] > (float)gl.yres) {
		g.ship.pos[1] -= (float)gl.yres;
	}
	//
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		i++;
	}
	//
	//update asteroid bullet position
	int j = 0;
	while (j < g.astBull) {
		Bullet *bAst = &g.barrAst[j];
		double ts = timeDiff(&bAst->time, &bt);
		if (ts > 2.5) {
			memcpy(&g.barrAst[j], &g.barrAst[g.astBull-1],
					sizeof(Bullet));
			g.astBull--;
			continue;
		}
	 	bAst->pos[0] += bAst->vel[0];
	 	bAst->pos[1] += bAst->vel[1];
		j++;
	}
	//
	//Update asteroid positions
	extern float unitLock(float);
	extern float lockOnAngle(float, float, float, float);
	extern float setTrigger(float, float, float, float);
	float angleLockOn, trigger;
	float accuracy = 90;
	float triggerDist = 200;
	Asteroid *a = g.ahead;
	a->angle = unitLock(a->angle);
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		a->angle += a->rotate;
		//Check for collision with window edges
		if (a->pos[0] < -100.0) {
			a->pos[0] += (float)gl.xres+200;
		}
		else if (a->pos[0] > (float)gl.xres+100) {
			a->pos[0] -= (float)gl.xres+200;
		}
		else if (a->pos[1] < -100.0) {
			a->pos[1] += (float)gl.yres+200;
		}
		else if (a->pos[1] > (float)gl.yres+100) {
			a->pos[1] -= (float)gl.yres+200;
		}
		Bullet *bAst = &g.barrAst[g.astBull];
		trigger = setTrigger(a->pos[0], a->pos[1], g.ship.pos[0], g.ship.pos[1]);
		//
		//All angles have been calculated, shoot if triggered
		if (trigger < triggerDist) {
			angleLockOn = lockOnAngle(g.ship.pos[0], a->pos[0], g.ship.pos[1], a->pos[1]);
			a->angle = angleLockOn - accuracy;
			//a little time between each bullet
			double ts = timeDiff(&g.enemyBulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&g.enemyBulletTimer, &bt);
				if (g.astBull < MAX_BULLETS) {
					//shoot a bullet...
					timeCopy(&bAst->time, &bt);
					bAst->pos[0] = a->pos[0];
					bAst->pos[1] = a->pos[1];
					bAst->vel[0] = (a->vel[0]);
					bAst->vel[1] = (a->vel[1]);
					//convert ship angle to radians
					Flt rad = ((a->angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					bAst->pos[0] += xdir*20.0f;
					bAst->pos[1] += ydir*20.0f;
					bAst->vel[0] += xdir*6.0f + rnd()*0.1;
					bAst->vel[1] += ydir*6.0f + rnd()*0.1;
					bAst->color[0] = 1.0f;
					bAst->color[1] = 1.0f;
					bAst->color[2] = 1.0f;
					g.astBull++;
					//Reload
					if (g.astBull == MAX_BULLETS) {
						g.astBull = 0;
					}
				}
			}
		}
			a = a->next;
	}
	//
	//Asteroid collision with bullets?
	//If collision detected:
	//   1. delete the bullet
	//   2. break the asteroid into pieces
	//      if asteroid small, delete it
	// a = g.ahead;
	// while (a) {
	// 	//is there a bullet within its radius?
	// 	int i=0;
	// 	while (i < g.nbullets) {
	// 		Bullet *b = &g.barr[i];
	// 		d0 = b->pos[0] - a->pos[0];
	// 		d1 = b->pos[1] - a->pos[1];
	// 		dist = (d0*d0 + d1*d1);
	// 		if (dist < (a->radius*a->radius)) {
	// 			//cout << "asteroid hit." << endl;
	// 			//this asteroid is hit.
	// 			if (a->radius > MINIMUM_ASTEROID_SIZE) {
	// 			// 	//break it into pieces.
	// 			// 	Asteroid *ta = a;
	// 			// 	buildAsteroidFragment(ta, a);
	// 			// 	int r = rand()%10+5;
	// 			// 	for (int k=0; k<r; k++) {
	// 			// 		//get the next asteroid position in the array
	// 			// 		Asteroid *ta = new Asteroid;
	// 			// 		buildAsteroidFragment(ta, a);
	// 			// 		//add to front of asteroid linked list
	// 			// 		ta->next = g.ahead;
	// 			// 		if (g.ahead != NULL)
	// 			// 			g.ahead->prev = ta;
	// 			// 		g.ahead = ta;
	// 			// 		g.nasteroids++;
	// 			// 	}
	// 			// } else {
	// 				a->color[0] = 1.0;
	// 				a->color[1] = 0.1;
	// 				a->color[2] = 0.1;
	// 				//asteroid is too small to break up
	// 				//delete the asteroid and bullet
	// 				Asteroid *savea = a->next;
	// 				deleteAsteroid(&g, a);
	// 				a = savea;
	// 				g.nasteroids--;
	// 			}
	// 			//delete the bullet...
	// 			memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
	// 			g.nbullets--;
	// 			if (a == NULL)
	// 				break;
	// 		}
	// 		i++;
	// 	}
	// 	if (a == NULL)
	// 		break;
	// 	a = a->next;
	// }
	//---------------------------------------------------
	//check keys pressed now
	extern float moveDownSpeed();
	extern float moveUpSpeed();
	extern float moveLeftSpeed();
	extern float moveRightSpeed();
	g.ship.vel[0] = 0;//0.02f
	g.ship.vel[1] = 0;
	//Left
	if (gl.keys[XK_a]) {
		g.ship.vel[0] = moveLeftSpeed();
	}
	//Right
	if (gl.keys[XK_d]) {
		g.ship.vel[0] = moveRightSpeed();
	}
	//Up
	if (gl.keys[XK_w]) {
		g.ship.vel[1] = moveUpSpeed();
	}
	//Down
	if (gl.keys[XK_s]){
		g.ship.vel[1] = moveDownSpeed();
	}
	if (gl.keys[XK_space]) {
		//a little time between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g.bulletTimer, &bt);
		if (ts > 0.1) {
			timeCopy(&g.bulletTimer, &bt);
			if (g.nbullets < MAX_BULLETS) {
				//shoot a bullet...
				//Bullet *b = new Bullet;
				Bullet *b = &g.barr[g.nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = g.ship.pos[0];
				b->pos[1] = g.ship.pos[1];
				b->vel[0] = g.ship.vel[0];
				b->vel[1] = g.ship.vel[1];
				//convert ship angle to radians
				Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				g.nbullets++;
			}
		}
	}
	if (g.mouseThrustOn) {
		//should thrust be turned off
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
		if (tdif < -0.3)
			g.mouseThrustOn = false;
	}
}
void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++)
	{
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
		float x = r * cosf(theta);//calculate the x component
		float y = r * sinf(theta);//calculate the y component
		glVertex2f(x + cx, y + cy);//output vertex
	}
	glEnd();
}
void show_credits()
{
	Rect r;
	r.bot=(gl.yres/2)+100+75;
	r.left=gl.xres/2+75;
	ggprint16(&r, 16, 0x00ff0000, "Members:");
	extern void art_credits(int x, int y);
	extern void edwin_credits(int x, int y);
	extern void andrew_credits(int x, int y);
	extern void bryan_credits(int x, int y);
	extern void joel_credits(int x, int y);
	art_credits( gl.xres/2,gl.yres/2-230);
	extern void art_picture(int x, int y, GLuint textid);
	extern void edwin_picture(int x, int y, GLuint textid);
	extern void andrew_picture(int x, int y, GLuint textid);
	extern void bryan_picture(int x, int y, GLuint textid);
	extern void joel_picture(int x, int y, GLuint textid);
	art_picture(gl.xres/2,gl.yres/2-320,gl.artTexture);
	edwin_credits(gl.xres/2,gl.yres/2+100);
	edwin_picture(200, 400, gl.edwinTexture);
	andrew_credits(gl.xres/2,((gl.yres/2)+40+75));
	andrew_picture(300, 100, gl.andrewTexture);
	bryan_credits(gl.xres/2,((gl.yres/2)+60+75));
	bryan_picture(400, 200, gl.bryanTexture);
	joel_credits(gl.xres/2,((gl.yres/2)+80+75));
	joel_picture(500, 300, gl.joelTexture);
}
void render()
{
	x11.clear_screen();

	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	//
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
extern int  getCreditState();
	if (getCreditState()){
		show_credits();/*
		glColor3ub(255,255,255);
		int wid=40;
		glPushMatrix();
		glTranslatef(200,200,0);
		glBindTexture(GL_TEXTURE_2D,gl.artTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex2i(-wid,-wid);
		glTexCoord2f(1.0f,0.0f); glVertex2i(-wid,wid);
		glTexCoord2f(1.0f,1.0f); glVertex2i(wid,wid);
		glTexCoord2f(0.0f,1.0f); glVertex2i(wid,-wid);
		glEnd();
		glPopMatrix();
		return;*/
	//	extern void art_picture(int x, int y, GLuint textid);
	//	art_picture(200,gl.yres-100,gl.artTexture);
	} else{
		ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
		ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
		ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
		ggprint8b(&r, 16, 0x00ffff00, "n asteroids destroyed: %i ",g.astr_destroyed);
		//
		//-------------
		//Draw the ship
		glColor3fv(g.ship.color);
		glPushMatrix();
		glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
		glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(-12.0f, -10.0f);
		glVertex2f(  0.0f, 20.0f);
		glVertex2f(  0.0f, -6.0f);
		glVertex2f(  0.0f, -6.0f);
		glVertex2f(  0.0f, 20.0f);
		glVertex2f( 12.0f, -10.0f);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();
        extern void character(int x, int y, int z, float angle, GLuint texid);
        character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterTexture);
		if (gl.keys[XK_Up] || g.mouseThrustOn) {
			int i;
			//draw thrust
			Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			Flt xs,ys,xe,ye,r;
			glBegin(GL_LINES);
			for (i=0; i<16; i++) {
				xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
				ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
				r = rnd()*40.0+40.0;
				xe = -xdir * r + rnd() * 18.0 - 9.0;
				ye = -ydir * r + rnd() * 18.0 - 9.0;
				glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
				glVertex2f(g.ship.pos[0]+xs,g.ship.pos[1]+ys);
				glVertex2f(g.ship.pos[0]+xe,g.ship.pos[1]+ye);
			}
			glEnd();
		}
		//DrawCircle((float)gl.xres/2,(float)gl.yres/2,44.0,33);
		//------------------
		//Draw the asteroids
		Asteroid *a = g.ahead;
		while (a) {
		glColor3fv(a->color);
		glPushMatrix();
		glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
		glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(-12.0f, -10.0f);
		glVertex2f(  0.0f, 20.0f);
		glVertex2f(  0.0f, -6.0f);
		glVertex2f(  0.0f, -6.0f);
		glVertex2f(  0.0f, 20.0f);
		glVertex2f( 12.0f, -10.0f);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();
        extern void character(int x, int y, int z, float angle, GLuint texid);
        //character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterTexture);
		// if (gl.keys[XK_Up] || g.mouseThrustOn) {
		// 	int i;
		// 	//draw thrust
		// 	Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
		// 	//convert angle to a vector
		// 	Flt xdir = cos(rad);
		// 	Flt ydir = sin(rad);
		// 	Flt xs,ys,xe,ye,r;
		// 	glBegin(GL_LINES);
		// 	for (i=0; i<16; i++) {
		// 		xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
		// 		ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
		// 		r = rnd()*40.0+40.0;
		// 		xe = -xdir * r + rnd() * 18.0 - 9.0;
		// 		ye = -ydir * r + rnd() * 18.0 - 9.0;
		// 		glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
		// 		glVertex2f(g.ship.pos[0]+xs,g.ship.pos[1]+ys);
		// 		glVertex2f(g.ship.pos[0]+xe,g.ship.pos[1]+ye);
		// 	}
			glEnd();
			a = a->next;
		}
		//ASTEROID HAS BEEN REPLACE WITH SHIP FOR TESTING

		// {
		// 	Asteroid *a = g.ahead;
		// 	while (a) {
		// 		//Log("draw asteroid...\n");
		// 		glColor3fv(a->color);
		// 		glPushMatrix();
		// 		glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
		// 		glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
		// 		glBegin(GL_LINE_LOOP);
		// 		//Log("%i verts\n",a->nverts);
		// 		for (int j=0; j<a->nverts; j++) {
		// 			glVertex2f(a->vert[j][0], a->vert[j][1]);
		// 		}
		// 		glEnd();
		// 		glPopMatrix();
		// 		glColor3f(1.0f, 0.0f, 0.0f);
		// 		glBegin(GL_POINTS);
		// 		glVertex2f(a->pos[0], a->pos[1]);
		// 		glEnd();
		// 		a = a->next;
		// 	}
		// }
		//----------------
		//Draw the bullets
		Bullet *b = &g.barr[0];
		for (int i=0; i<g.nbullets; i++) {
			//Log("draw bullet...\n");
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_POINTS);
			glVertex2f(b->pos[0],      b->pos[1]);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]);
			glVertex2f(b->pos[0],      b->pos[1]-1.0f);
			glVertex2f(b->pos[0],      b->pos[1]+1.0f);
			glColor3f(0.8, 0.8, 0.8);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
			glEnd();
			++b;
		}
		//CHANGE draw ast bullet
		Bullet *bAst = &g.barrAst[0];
		for (int i=0; i<g.astBull; i++) {
			//Log("draw bullet...\n");
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_POINTS);
			glVertex2f(bAst->pos[0],      bAst->pos[1]);
			glVertex2f(bAst->pos[0]-1.0f, bAst->pos[1]);
			glVertex2f(bAst->pos[0]+1.0f, bAst->pos[1]);
			glVertex2f(bAst->pos[0],      bAst->pos[1]-1.0f);
			glVertex2f(bAst->pos[0],      bAst->pos[1]+1.0f);
			glColor3f(0.8, 0.8, 0.8);
			glVertex2f(bAst->pos[0]-1.0f, bAst->pos[1]-1.0f);
			glVertex2f(bAst->pos[0]-1.0f, bAst->pos[1]+1.0f);
			glVertex2f(bAst->pos[0]+1.0f, bAst->pos[1]-1.0f);
			glVertex2f(bAst->pos[0]+1.0f, bAst->pos[1]+1.0f);
			glEnd();
			++bAst;
		}
	}
}
