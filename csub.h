/*
 * =====================================================================================
 *
 *       Filename:  csub.h
 *
 *    Description:  ALL FUNCTIONS FROM CSUB.cpp to be used by others.
 *
 *        Version:  1.0
 *        Created:  10/28/2018 04:24:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arturo Urbina (), Arturo@Urbina.Email
 *   Organization:  CSUB
 *
 * =====================================================================================
 */

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
class Global {
	public:
	    GLuint artTexture;
	    GLuint bryanTexture;
	    GLuint joelTexture;
	    GLuint andrewTexture;
	    GLuint edwinTexture;
	    GLuint rTexture;
	    GLuint sTexture;
	    GLuint mgTexture;
	    //Store sound sources here.
	    ALuint bulletSound;
	    ALuint buffers[1];
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
		struct timespec enemyBulletTimer[3];
		struct timespec itemTimer;
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
			clock_gettime(CLOCK_REALTIME, &enemyBulletTimer[3]);
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
			gl.xres = w + 480;
			gl.yres = h + 270;
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
