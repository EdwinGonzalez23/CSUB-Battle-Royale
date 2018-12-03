//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//mod spring 2018: X11 wrapper class
//This program is a game starting point for a 3350 project.
//
//
// #include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <ctime>
// #include <cmath>
// #include <unistd.h>
// #include <X11/Xlib.h>
// //#include <X11/Xutil.h>
// //#include <GL/gl.h>
// //#include <GL/glu.h>
// #include <X11/keysym.h>
// #include <GL/glx.h>
// //Audio library
// #include <AL/alut.h>
// #include <thread>
// #include "log.h"
// #include "fonts.h"
// //#include "csub.h"
#include "game.h"
using namespace std;
Game g;
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
			XStoreName(dpy, win, "CSUB BATTLE ROYALE!");
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
			gl.xres = w + 1000;
			gl.yres = h + 1000;
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
Image img[37]={"art.jpg","joel_pic.jpg","edwinImg.png","bryan_picture.jpg","1.jpg",

	"rifleCrate.png","shotgunCrate.png","machineGunCrate.png", "./images/models/handgun.png",
	"./images/models/rifle.png", "./images/models/shotgun.png", "./images/models/knife.png",
	"bullet2.png","bg2.jpeg","tree2.png","you_died.png","csubbattlegrounds.png","text.png","tile.png",
	"images/tiles/road.png", "images/tiles/grass.png", "images/tiles/housefloor.png", "images/tiles/wallB.png",
	"images/tiles/wallL.png", "images/tiles/wallR.png", "images/tiles/wallT.png", "images/tiles/wallCorner.png",
	"images/tiles/wallCenter.png","bullethole.png","hp.png", "images/tiles/rock1.png",
	"images/tiles/rock2.png", "images/tiles/bush1.png", "images/tiles/bush2.png","go.png","winFull2.png","./images/models/boss.png"}

void setup_sound(Global &gl){
	alutInit (NULL, NULL);
	gl.buffers[0] = alutCreateBufferFromFile ("./audio/gunshot.wav");
	gl.buffers[1] = alutCreateBufferFromFile ("./audio/You_Died.wav");
	gl.buffers[2] = alutCreateBufferFromFile ("./audio/playerHit.wav");
	gl.buffers[3] = alutCreateBufferFromFile ("./audio/mg.wav");
	gl.buffers[4] = alutCreateBufferFromFile ("./audio/sf.wav");
	gl.buffers[5] = alutCreateBufferFromFile ("./audio/pistol.wav");
	gl.buffers[6] = alutCreateBufferFromFile ("./audio/rifle.wav");
	gl.buffers[7] = alutCreateBufferFromFile ("./audio/shotgun.wav");
	gl.buffers[8] = alutCreateBufferFromFile ("./audio/ready.wav");
	gl.buffers[9] = alutCreateBufferFromFile ("./audio/go.wav");
	alGenSources (1, &gl.bulletSound);
	alGenSources (1, &gl.youDiedSound);
	alGenSources (1, &gl.playerHitSound);
	alGenSources (1, &gl.mgSound);
	alGenSources (1, &gl.sfSound);
	alGenSources (1, &gl.pSound);
	alGenSources (1, &gl.rSound);
	alGenSources (1, &gl.sSound);
	alGenSources (1, &gl.readySound);
	alGenSources (1, &gl.goSound);
	alSourcei (gl.bulletSound, AL_BUFFER, gl.buffers[0]);
	alSourcei (gl.youDiedSound, AL_BUFFER, gl.buffers[1]);
	alSourcei (gl.playerHitSound, AL_BUFFER, gl.buffers[2]);
	alSourcei (gl.mgSound, AL_BUFFER, gl.buffers[3]);
	alSourcei (gl.sfSound, AL_BUFFER, gl.buffers[4]);
	alSourcei (gl.pSound, AL_BUFFER, gl.buffers[5]);
	alSourcei (gl.rSound, AL_BUFFER, gl.buffers[6]);
	alSourcei (gl.sSound, AL_BUFFER, gl.buffers[7]);
	alSourcei (gl.readySound, AL_BUFFER, gl.buffers[8]);
	alSourcei (gl.goSound, AL_BUFFER, gl.buffers[9]);
}

void readyGo(){
	play_sound(gl.readySound);
	sleep(1);
	play_sound(gl.goSound);	
}


//function prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
//Added Function prototypes
void firePistol();
void fireRifle();
void eFireRifle(Asteroid *a, int);
void eFireMachineGun(Asteroid *a, int);
void eFirePistol(Asteroid *a, int);
void fireShotgun();
void generatePellet();
void fireMachineGun();
//External Prototypes
extern void play_sound(ALuint src);
extern void setCurrentWeapon(int newWeapon);
extern void printCurrentWeapon(int weapon, Rect r);
extern int getCurrentWeapon();
extern void health_bar(int x, int y);
extern void damagePlayer();
extern void healPlayer();
extern void gunSpawnManager(struct timespec &it);
extern void timeInit(struct timespec &it);
extern void genRifle(GLuint texture);
extern void genShotgun(GLuint texture);
extern void genMachineGun(GLuint texture);
extern void setItemBoundary(int x, int y);
extern bool rifleIsOnScreen();
extern bool shotgunIsOnScreen();
extern bool machineGunIsOnScreen();
extern void getRiflePosition(int x[2]);
extern void getShotgunPosition(int x[2]);
extern void getMachineGunPosition(int x[2]);
extern void pickUpRifle();
extern void pickUpShotgun();
extern void pickUpMachineGun();
extern bool doesPlayerHaveRifle();
extern bool doesPlayerHaveShotgun();
extern bool doesPlayerHaveMachineGun();
extern void printAmmo();
extern void decrementAmmo();
extern void genAmmo(GLuint texture, int x, int y);
extern void reloadAmmunition();
extern int hasBulletsLoaded(int x);
extern void genBackground(GLuint bg);
extern void genTrees(GLuint &tree);
extern bool playerIsAlive();
extern void drawYouDied(GLuint texture,int x, int y);
extern void drawYouDied2(GLuint texture, int x, int y);
extern bool deathSoundPlayed();
extern void fadeToBlack();
extern bool doneFading();
extern void genTitleScreen(GLuint texture,GLuint texture2, int x, int y);
extern bool menuFadedOut();
extern void beginFade();
extern void setColors();
extern void invuln();
extern bool playerIsInvulnerable();
extern void win();
extern bool playerHasWon();
extern void getPackLocations(int i, int x[2]);
extern void healthPack(GLuint texture, int x, int y,int i);
extern void pickUpPack(int i);
extern void shakeScreen(int l, int r, int b, int t);
extern int getShake();
extern void letterBoxes(int x, int y, GLuint z);
extern void drawLine(int x, int y);
extern bool getIntroComplete();
extern int Rocks[][2];
extern bool hasReadyGoBeenSaid();
extern bool isInsideDeath(float circlex, float circley, float rad, float x, float y);
extern void zoomOut(int l, int r, int b, int t);
extern bool isTransitionComplete();
extern int getZoom();
extern void drawWinText(GLuint texture, int x, int y);
extern bool winSoundPlayed();

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	init_opengl();
	srand(time(NULL));
	x11.set_mouse_position(100, 100);
	setup_sound(gl);
	timeInit(g.itemTimer);
	setItemBoundary(gl.xres,gl.yres);
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		if((menuFadedOut()&&playerIsAlive())&&playerHasWon()==0&&getIntroComplete()){
			physics();
		}
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	alutExit();
	logClose();
	return 0;
}
unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}
void init_opengl()
{
	//RIFLE TEXTURE
	glGenTextures(1,&gl.rTexture);
	int w = img[5].width;
	int h = img[5].height;
	glBindTexture(GL_TEXTURE_2D, gl.rTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[5].data);
	//SHOTGUN TEXTURE
	glGenTextures(1,&gl.sTexture);
	w = img[6].width;
	h = img[6].height;
	glBindTexture(GL_TEXTURE_2D, gl.sTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[6].data);
	//MACHINE GUN TEXTURE
	glGenTextures(1,&gl.mgTexture);
	w = img[7].width;
	h = img[7].height;
	glBindTexture(GL_TEXTURE_2D, gl.mgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[7].data);
	// ART TEXTURE
	//
	glGenTextures(1,&gl.artTexture);
	w = img[0].width;
	h = img[0].height;
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
	//Handgun Model
	glGenTextures(1, &gl.characterHandgun);
	w = img[8].width;
	h = img[8].height;
	glBindTexture(GL_TEXTURE_2D, gl.characterHandgun);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *HandgunData = buildAlphaData(&img[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, HandgunData);
	//Rifle Model
	glGenTextures(1, &gl.characterRifle);
	w = img[9].width;
	h = img[9].height;
	glBindTexture(GL_TEXTURE_2D, gl.characterRifle);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *RifleData = buildAlphaData(&img[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, RifleData);
	//Shotgun Model
	glGenTextures(1, &gl.characterShotgun);
	w = img[10].width;
	h = img[10].height;
	glBindTexture(GL_TEXTURE_2D, gl.characterShotgun);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *ShotgunData = buildAlphaData(&img[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, ShotgunData);
	//Knife Model
	glGenTextures(1, &gl.characterKnife);
	w = img[11].width;
	h = img[11].height;
	glBindTexture(GL_TEXTURE_2D, gl.characterKnife);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *KnifeData = buildAlphaData(&img[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, KnifeData);
	//Bullet Icon
	glGenTextures(1, &gl.bulletTexture);
	w = img[12].width;
	h = img[12].height;
	glBindTexture(GL_TEXTURE_2D, gl.bulletTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *bulletData = buildAlphaData(&img[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bulletData);
	//Background image
	glGenTextures(1,&gl.bgTexture);
	w = img[13].width;
	h = img[13].height;
	glBindTexture(GL_TEXTURE_2D, gl.bgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[13].data);
	//Tree Image
	glGenTextures(1, &gl.treeTexture);
	w = img[14].width;
	h = img[14].height;
	glBindTexture(GL_TEXTURE_2D, gl.treeTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *treeData = buildAlphaData(&img[14]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, treeData);
	//You Died Image
	glGenTextures(1, &gl.ydTexture);
	w = img[15].width;
	h = img[15].height;
	glBindTexture(GL_TEXTURE_2D, gl.ydTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *ydData = buildAlphaData(&img[15]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, ydData);
	//Logo Image
	glGenTextures(1, &gl.logoTexture);
	w = img[16].width;
	h = img[16].height;
	glBindTexture(GL_TEXTURE_2D, gl.logoTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *logoData = buildAlphaData(&img[16]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, logoData);
	//Intro Text
	glGenTextures(1, &gl.textTexture);
	w = img[17].width;
	h = img[17].height;
	glBindTexture(GL_TEXTURE_2D, gl.textTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *textData = buildAlphaData(&img[17]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, textData);
	// green tile TEXTURE
	glGenTextures(1,&gl.tileTexture);
	w = img[18].width;
	h = img[18].height;
	glBindTexture(GL_TEXTURE_2D, gl.tileTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[18].data);
	//Road Image
	glGenTextures(1,&gl.roadTexture);
	w = img[19].width;
	h = img[19].height;
	glBindTexture(GL_TEXTURE_2D, gl.roadTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[19].data);
	//Grass Image
	glGenTextures(1,&gl.grassTexture);
	w = img[20].width;
	h = img[20].height;
	glBindTexture(GL_TEXTURE_2D, gl.grassTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[20].data);
	//House Floor Image
	glGenTextures(1,&gl.floorTexture);
	w = img[21].width;
	h = img[21].height;
	glBindTexture(GL_TEXTURE_2D, gl.floorTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[21].data);
	//wallB Icon
	glGenTextures(1, &gl.wallB);
	w = img[22].width;
	h = img[22].height;
	glBindTexture(GL_TEXTURE_2D, gl.wallB);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *wallBData = buildAlphaData(&img[22]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, wallBData);
	//wallL Icon
	glGenTextures(1, &gl.wallL);
	w = img[23].width;
	h = img[23].height;
	glBindTexture(GL_TEXTURE_2D, gl.wallL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *wallLData = buildAlphaData(&img[23]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, wallLData);
	//wallR Icon
	glGenTextures(1, &gl.wallR);
	w = img[24].width;
	h = img[24].height;
	glBindTexture(GL_TEXTURE_2D, gl.wallR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *wallRData = buildAlphaData(&img[24]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, wallRData);
	//wallT Icon
	glGenTextures(1, &gl.wallT);
	w = img[25].width;
	h = img[25].height;
	glBindTexture(GL_TEXTURE_2D, gl.wallT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *wallTData = buildAlphaData(&img[25]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, wallTData);
	//House Corner Image
	glGenTextures(1,&gl.wallCorner);
	w = img[26].width;
	h = img[26].height;
	glBindTexture(GL_TEXTURE_2D, gl.wallCorner);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[26].data);
	//House Floor Empty Image
	glGenTextures(1,&gl.wallEmpty);
	w = img[27].width;
	h = img[27].height;
	glBindTexture(GL_TEXTURE_2D, gl.wallEmpty);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[27].data);
	//Bullet Hole Image
	glGenTextures(1, &gl.bhTexture);
	w = img[28].width;
	h = img[28].height;
	glBindTexture(GL_TEXTURE_2D, gl.bhTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *bhData = buildAlphaData(&img[28]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bhData);
	//Health Pack Texture
	glGenTextures(1, &gl.hpTexture);
	w = img[29].width;
	h = img[29].height;
	glBindTexture(GL_TEXTURE_2D, gl.hpTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *hpData = buildAlphaData(&img[29]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, hpData);
	//Rock Texture 1
	glGenTextures(1, &gl.rockTexture1);
	w = img[31].width;
	h = img[31].height;
	glBindTexture(GL_TEXTURE_2D, gl.rockTexture1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *rockTexture1Data = buildAlphaData(&img[30]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, rockTexture1Data);
	//Rock Texture 2
	glGenTextures(1, &gl.rockTexture2);
	w = img[31].width;
	h = img[31].height;
	glBindTexture(GL_TEXTURE_2D, gl.rockTexture2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *rockTexture2Data = buildAlphaData(&img[31]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, rockTexture2Data);
	//Bush Texture 1
	glGenTextures(1, &gl.bushTexture1);
	w = img[32].width;
	h = img[32].height;
	glBindTexture(GL_TEXTURE_2D, gl.bushTexture1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *bushTexture1Data = buildAlphaData(&img[32]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bushTexture1Data);
	//Bush Texture 2
	glGenTextures(1, &gl.bushTexture2);
	w = img[33].width;
	h = img[33].height;
	glBindTexture(GL_TEXTURE_2D, gl.bushTexture2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *bushTexture2Data = buildAlphaData(&img[33]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bushTexture2Data);
	//Go Image
	glGenTextures(1, &gl.goTexture);
	w = img[34].width;
	h = img[34].height;
	glBindTexture(GL_TEXTURE_2D, gl.goTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *goData = buildAlphaData(&img[34]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, goData);
			
	glGenTextures(1, &gl.ywTexture);
	w = img[35].width;
	h = img[35].height;
	glBindTexture(GL_TEXTURE_2D, gl.ywTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *ywData = buildAlphaData(&img[35]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, ywData);
    //Boss Texture
    glGenTextures(1, &gl.bossTexture);
	w = img[36].width;
	h = img[36].height;
	glBindTexture(GL_TEXTURE_2D, gl.bossTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *bossData = buildAlphaData(&img[36]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bossData);
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	// glMatrixMode(GL_PROJECTION); glLoadIdentity();
	// glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	// //This sets 2D mode (no perspective)
	// glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
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
			switch(getCurrentWeapon()){
				case 1:
					if(hasBulletsLoaded(getCurrentWeapon())){
						firePistol();
					}
					break;
				case 2:
					if(hasBulletsLoaded(getCurrentWeapon())){
						fireRifle();
					}
					break;
				case 3:
					if(hasBulletsLoaded(getCurrentWeapon())){
						fireShotgun();
					}
					break;
				case 4:
					if(hasBulletsLoaded(getCurrentWeapon())){
						fireMachineGun();
					}
					break;
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
		case XK_m:
			beginFade();
			/*
			   int getMenuState();
			   if(getMenuState()){
			   void toggleMenu();
			   toggleMenu();
			   }*/
		case XK_s:
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
		case XK_1:
			setCurrentWeapon(1);
			break;
		case XK_2:
			if(doesPlayerHaveRifle()){
				setCurrentWeapon(2);
			}
			break;
		case XK_3:
			if(doesPlayerHaveShotgun()){
				setCurrentWeapon(3);
			}
			break;
		case XK_4:
			if(doesPlayerHaveMachineGun()){
				setCurrentWeapon(4);
			}
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
extern int spawnRand_XSection(int x, int y);
extern int spawnRand_YSection(int x, int y);
extern bool spawned();
extern bool notSpawned();
extern float xPlusCheck(float x, int playerIndex);
extern float xNegCheck(float x, int playerIndex);
extern float yPlusCheck(float y, int playerIndex);
extern float yNegCheck(float y, int playerIndex);
extern float unitLock(float);
extern float lockOnAngle(float, float, float, float);
extern float setTrigger(float, float, float, float);
extern float setTriggerDist();
bool hasSpawned = notSpawned();
extern int setSectLen();
extern bool isInSector(float, float, int, float, float);
int vectorLen = 9;
vector<int> flipVel(vectorLen, 0);
extern void drawVertWall(int x, int y, Game &g);
extern void drawHorzWall(int x, int y, Game &g);
extern void drawDoorWall(int x, int y, Game &g);
extern void drawDoorRight(int x, int y, Game &g);
extern void drawHouse(int centerX, int centerY, Game &g);
extern void drawSecondHouse(int x, int y, Game &g);
extern void rockPlayerCollision(int x, int y, int px, int py, Game &g);
extern void regulateSpeed(Game &g);
extern void wallCollision(int x, int y, int i, Bullet *b, int check, Game &g);
extern void rockCollision(int x, int y, int bx, int by, int i, Bullet *b, int check, Game &g);
extern void enemyWallCollision(int x, int y, Asteroid *a, int velSwitchCounter, vector<int> &flipVel);
extern void enemyRockCollision(int x, int y, int ex, int ey, Asteroid *a, int velSwitchCounter, vector<int> &flipVel);
extern void spawn(Asteroid *a, vector<vector<int>> vector, float xLen, float yLen, Game &g);
bool spawnBoss = false;
vector<vector<int>> spawnVector{{240,1892},{1040,1892},{2395,1885},
	{338,1071},{1458,1017},{2459,779},
	{182,77},{1000,378},{2606,574}};
int path1=1, path2=0, path3=0, path4=0, path5=0, path6=0, path7=0, path8=0, path9=0;
void physics()
{
	regulateSpeed(g);
	float xLen = gl.xres;
	float yLen = gl.yres;
	Asteroid *a = g.ahead;
	//
	//Initial Spawn
	if(!hasSpawned) {
		int spawnX = xLen/3;
		int spawnY = yLen/3;
		int section = 1;
		Asteroid *a = g.ahead;
		int tracker = 1;
		int xCord, yCord;
		int ct = 0;
		while (a) {
			int num =  rand() % 3;
			a->gunNum =(int) num;
			//xCord = spawnRand_XSection(spawnX, section);
			//yCord = spawnRand_YSection(spawnY, section);
			a->pos[0] = spawnVector[ct][0]; //spawnRand_XSection(spawnX, section);
			a->pos[1] = spawnVector[ct][1];//yCord; //spawnRand_YSection(spawnY, section);
			a = a->next;
			section++;
			tracker++;
			ct++;
		}
		hasSpawned = spawned();
	}
	//Keep Enemies in sectors
	// while (a) {
	// 	if (a->pos[0] < -100.0) {
	// 		a->pos[0] += (float)gl.xres+200;
	// 	}
	// 	else if (a->pos[0] > (float)gl.xres+100) {
	// 		a->pos[0] -= (float)gl.xres+200;
	// 	}
	// 	else if (a->pos[1] < -100.0) {
	// 		a->pos[1] += (float)gl.yres+200;
	// 	}
	// 	else if (a->pos[1] > (float)gl.yres+100) {
	// 		a->pos[1] -= (float)gl.yres+200;
	// 	}
	// 	a = a->next;
	// }
	//Flt d0,d1,dist;
	//Update ship position
	invuln();
	//Check for collision with window edges
	//window collisions
	if (g.ship.pos[0] < 0.0) {
		//g.ship.pos[0] += (float)gl.xres;
		g.ship.pos[0] = 0.0;
	}
	if (g.ship.pos[0] > (float)gl.xres) {
		//g.ship.pos[0] -= (float)gl.xres;
		g.ship.pos[0] = (float)gl.xres;
	}
	if (g.ship.pos[1] < 0.0) {
		//g.ship.pos[1] += (float)gl.yres;
		g.ship.pos[1] = 0.0;
	}
	if (g.ship.pos[1] > (float)gl.yres) {
		//g.ship.pos[1] -= (float)gl.yres;
		g.ship.pos[1] = gl.yres;
	}
	//Check for collisions with weapon boxes.
	int boxLoc[2];
	if(rifleIsOnScreen()){
		getRiflePosition(boxLoc);
		if((g.ship.pos[0]>=boxLoc[0]-25&&g.ship.pos[0]<=boxLoc[0]+25)&&
				(g.ship.pos[1]>=boxLoc[1]-25&&g.ship.pos[1]<=boxLoc[1]+25)){
			pickUpRifle();
		}
	}
	if(shotgunIsOnScreen()){
		getShotgunPosition(boxLoc);
		if((g.ship.pos[0]>=boxLoc[0]-25&&g.ship.pos[0]<=boxLoc[0]+25)&&
				(g.ship.pos[1]>=boxLoc[1]-25&&g.ship.pos[1]<=boxLoc[1]+25)){
			pickUpShotgun();
		}
	}
	if(machineGunIsOnScreen()){
		getMachineGunPosition(boxLoc);
		if((g.ship.pos[0]>=boxLoc[0]-25&&g.ship.pos[0]<=boxLoc[0]+25)&&
				(g.ship.pos[1]>=boxLoc[1]-25&&g.ship.pos[1]<=boxLoc[1]+25)){
			pickUpMachineGun();
		}
	}
	for(int j = 0;j<5;j++){
		getPackLocations(j,boxLoc);
		if((g.ship.pos[0]>=boxLoc[0]-25&&g.ship.pos[0]<=boxLoc[0]+25)&&
				(g.ship.pos[1]>=boxLoc[1]-25&&g.ship.pos[1]<=boxLoc[1]+25)){
			pickUpPack(j);
		}
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
		wallCollision(723, 646, i, b, 1, g);
		wallCollision(1738, 1045, i, b, 1, g);
		wallCollision(149, 1115, i, b, 1, g);
		for (int i = 0; i < 27; i++){
			rockCollision(Rocks[i][0], Rocks[i][1], b->pos[0], b->pos[1], i, b, 1, g);
		}
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
		wallCollision(723, 646, i, bAst, 0, g);
		wallCollision(1738, 1045, i, bAst,0, g);
		wallCollision(149, 1115, i, bAst,0, g);
		for (int i = 0; i < 27; i++){
			rockCollision(Rocks[i][0], Rocks[i][1], bAst->pos[0], bAst->pos[1], i, bAst, 0, g);
		}
		if((bAst->pos[0]>=g.ship.pos[0]-25&&bAst->pos[0]<=g.ship.pos[0]+25)&&
				(bAst->pos[1]>=g.ship.pos[1]-25&&bAst->pos[1]<=g.ship.pos[1]+25)){
			if(playerIsInvulnerable()==0&&playerIsAlive()){
				thread t1(play_sound, gl.playerHitSound);
				t1.detach();
			}
			damagePlayer();
		}
		j++;
	}
	//Center Boss
	if (a->isBoss ==1) {
		if (!spawnBoss) {
			a->pos[0] = gl.xres/2;
			a->pos[1] = gl.yres/2;
		}
		spawnBoss = true;
	}

	//
	//Update asteroid positions
	extern void flipPos(vector<int> &flipVel, int velSwitchCounter, Asteroid *a);
	extern void flipNeg(vector<int> &flipVel, int velSwitchCounter, Asteroid *a);
	extern void bounceOffWall(vector<int> &flipVel, int velSwitchCounter);
	float angleLockOn;// trigger;
	float accuracy = 90;
	//float triggerDist = setTriggerDist();
	a->angle = unitLock(a->angle);
	int k = 0;
	int velSwitchCounter = 0;
	int enemyTracker = 1;
	while (a) {
		if (a->isBoss == 0) {
		enemyWallCollision(723, 646, a, velSwitchCounter, flipVel);
		enemyWallCollision(1738, 1045, a, velSwitchCounter, flipVel);
		enemyWallCollision(149, 1115, a, velSwitchCounter, flipVel);
		flipPos(flipVel, velSwitchCounter, a);
		flipNeg(flipVel, velSwitchCounter, a);
		//Check for collision with window edges
		if (a->pos[0] < xNegCheck(xLen, enemyTracker)) { //XNEG
			//a->pos[0] += (float)(gl.xres)+200;
			bounceOffWall(flipVel, velSwitchCounter);
		}
		else if (a->pos[0] > xPlusCheck(xLen, enemyTracker)) {//float)gl.xres+100
			//a->pos[0] -= (float)gl.xres+200; //XPOS
			bounceOffWall(flipVel, velSwitchCounter);
		}
		else if (a->pos[1] < yNegCheck(yLen, enemyTracker)) {//-100.0
			//a->pos[1] += (float)gl.yres+200; //YNEG
			bounceOffWall(flipVel, velSwitchCounter);
		}
		else if (a->pos[1] > yPlusCheck(yLen, enemyTracker)) {
			//a->pos[1] -= (float)gl.yres+200; //YPOS
			bounceOffWall(flipVel, velSwitchCounter);
		}
		for (int i = 0; i < 27; i++){
			enemyRockCollision(Rocks[i][0], Rocks[i][1], a->pos[0], a->pos[1], a, velSwitchCounter, flipVel);
		}
		}
		if (a->isBoss == 1) {
			if (path1 == 1) {
				a->pos[0] += a->vel[0]*2;
				cout << "en coord" << a->pos[0];
			}
			if (a->pos[0] < 200 && path1 == 1) {
				path1 = 0;
				path2 = 1;
			}
			if (path2 == 1){
				a->pos[1] -= a->vel[1]*8;
				cout << "en x coord" << a->pos[1] << endl;
			}
			if (a->pos[1] > 2000 && path2 == 1) {
				path1 = 0;
				path2 = 0;
				path3 = 1;
			}
			if (path3 == 1) {
				a->pos[0] -= a->vel[0]*3;
			}
			if (a->pos[0] > 2000 && path3 == 1) {
				path1 = 0;
				path2 = 0;
				path3 = 0;
				path4 = 1;
			}
			if (path4 == 1) {
				a->pos[0] += a->vel[0]*5;
				a->pos[1] += a->vel[0]*5;
			}
			if (a->pos[1] < 1800 && path4 == 1) {
				path1 = 0;
				path2 = 0;
				path3 = 0;
				path4 = 0;
				path5 = 1;
			}
			if (path5 == 1) {
				a->pos[1] += a->vel[1]*5;
			}			
			if (a->pos[1] < 100 && path5 == 1) {
				path1 = 0;
				path2 = 0;
				path3 = 0;
				path4 = 0;
				path5 = 0;
				path6 = 1;
			}
			if (path6 == 1) {
				a->pos[0] -= a->vel[0]*5;
				a->pos[1] -= a->vel[0]*5;
			}
			int xMid = (int) gl.xres/2;
			if (a->pos[1] >= gl.yres/2 && path6 == 1) {
				path1 = 0;
				path2 = 0;
				path3 = 0;
				path4 = 0;
				path5 = 0;
				path6 = 0;
				path7 = 1;
			}
			if (path7 == 1) {
				a->pos[0] += a->vel[0]*8;
			}
			if (a->pos[0] <= (gl.xres/2)-20 && path7 == 1) {
				a->pos[0] = gl.xres/2;
				a->pos[1] = gl.xres/2;
				path1 = 1;
				path2 = 0;
				path3 = 0;
				path4 = 0;
				path5 = 0;
				path6 = 0;
				path7 = 0;
			}
		}
		//This code checks for player bullet and enemy collision.
		int bulls=0;
		//Check asteroid invulnerability.
		a->checkInvuln();
		while (bulls < g.nbullets) {
			Bullet *b = &g.barr[bulls];
			//Assign proper hitbox.
			int enemySize = 25;
			if(a->isBoss==1){
				enemySize=100;
			}
			if((b->pos[0]>=a->pos[0]-enemySize&&b->pos[0]<=a->pos[0]+enemySize)&&
					(b->pos[1]>=a->pos[1]-enemySize&&b->pos[1]<=a->pos[1]+enemySize)&&a->invuln==0){
				thread t1(play_sound, gl.playerHitSound);
				t1.detach();
				a->health-=10;
				a->hpMissing+=10;
				clock_gettime(CLOCK_REALTIME, &a->invulnTimer);
				a->invuln=1;
				if(a->health<=0){
					if(g.nasteroids==1&&a->isBoss==1){
						cout<<"You Win!"<<endl;
						win();
					}else if(g.nasteroids==1&&a->isBoss==0){
						a->isBoss=1;
						a->health=50;
						a->hpMissing=0;
					}else if(a->next==nullptr){
						Asteroid *savea = a->prev;
						deleteAsteroid(&g, a);
						a = savea;
						g.nasteroids--;
					}else if(a->prev==nullptr){
						Asteroid *savea = a->next;
						deleteAsteroid(&g, a);
						a = savea;
						g.nasteroids--;
					}else{
						Asteroid *savea = a->next;
						deleteAsteroid(&g, a);
						a = savea;
						g.nasteroids--;
					}
				}
			}
			bulls++;
		}
		//	Bullet *bAst = &g.barrAst[g.astBull];
		//trigger = setTrigger(a->pos[0], a->pos[1], g.ship.pos[0], g.ship.pos[1]);
		//bool inSector = isInSector(xLen, yLen, enemyTracker, xLen, yLen);
		//All angles have been calculated, shoot if triggered
		extern bool enemyShoot(float, float, float, float);
		if (enemyShoot(a->pos[0], a->pos[1], g.ship.pos[0], g.ship.pos[1])) {
			angleLockOn = lockOnAngle(g.ship.pos[0], a->pos[0], g.ship.pos[1], a->pos[1]);
			a->angle = angleLockOn - accuracy;
			if (a->gunNum == 0)
				eFireRifle(a,k);
			if (a->gunNum == 1)
				eFireMachineGun(a,k);
			if (a->gunNum == 2)
				eFirePistol(a,k);
			k++;
		}
		enemyTracker++;
		velSwitchCounter++;
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
		switch(getCurrentWeapon()){
			case 1:
				firePistol();
				break;
			case 2:
				fireRifle();
				break;
			case 3:
				fireShotgun();
				break;
			case 4:
				fireMachineGun();
				break;
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
void firePistol(){
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if(ts>0.6){
		timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.pSound);
			t1.detach();
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
			b->vel[0] += xdir*15.0f + rnd()*0.1;
			b->vel[1] += ydir*15.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
			decrementAmmo();
		}
	}
}
void getCharacter()
{
	extern void character(int x, int y, int z, float angle, GLuint texid);
	extern int getCurrentWeapon();
	string texid;
	if (getCurrentWeapon() == 0){
		character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterHandgun);
		//texid = "characterPistol";
	}
	if (getCurrentWeapon() == 1){
		character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterHandgun);
		//texid = "characterPistol";
	}
	if (getCurrentWeapon() == 2){
		//texid = "characterRifle";
		character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterRifle);
	}
	if (getCurrentWeapon() == 3) {
		//texid = "characterShotgun";
		character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterShotgun);
	}
	if (getCurrentWeapon() == 4){
		//texid = "characterRifle";
		character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterRifle);
	}
}
void fireRifle(){
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if(ts>1.5){
		timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.rSound);
			t1.detach();
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
			b->vel[0] += xdir*20.0f + rnd()*0.1;
			b->vel[1] += ydir*20.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
			decrementAmmo();
		}
	}
}
void eFireRifle(Asteroid *a, int k)
{
	// Asteroid *a = g.ahead;
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.enemyBulletTimer[k], &bt);
	if(ts > 1.0) {
		timeCopy(&g.enemyBulletTimer[k], &bt);
		if (g.astBull < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.bulletSound);
			t1.detach();
			//Bullet *b = new Bullet;
			Bullet *bAst = &g.barrAst[g.astBull];
			timeCopy(&bAst->time, &bt);
			bAst->pos[0] = a->pos[0];
			bAst->pos[1] = a->pos[1];
			bAst->vel[0] = a->vel[0];
			bAst->vel[1] = a->vel[1];
			//convert ship angle to radians
			Flt rad = ((a->angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			bAst->pos[0] += xdir*20.0f;
			bAst->pos[1] += ydir*20.0f;
			bAst->vel[0] += xdir*20.0f + rnd()*0.1;
			bAst->vel[1] += ydir*20.0f + rnd()*0.1;
			bAst->color[0] = 1.0f;
			bAst->color[1] = 1.0f;
			bAst->color[2] = 1.0f;
			g.astBull++;
			// g.nbullets++;
			// decrementAmmo();
			if (g.astBull == MAX_BULLETS) {
				g.astBull = 0;
			}
		}
	}
}
void eFirePistol(Asteroid *a, int k)
{
	// Asteroid *a = g.ahead;
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.enemyBulletTimer[k], &bt);
	if(ts > 0.2) {
		//healPlayer();
		timeCopy(&g.enemyBulletTimer[k], &bt);
		if (g.astBull < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.bulletSound);
			t1.detach();
			//Bullet *b = new Bullet;
			Bullet *bAst = &g.barrAst[g.astBull];
			timeCopy(&bAst->time, &bt);
			bAst->pos[0] = a->pos[0];
			bAst->pos[1] = a->pos[1];
			bAst->vel[0] = a->vel[0];
			bAst->vel[1] = a->vel[1];
			//convert ship angle to radians
			Flt rad = ((a->angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			bAst->pos[0] += xdir*20.0f;
			bAst->pos[1] += ydir*20.0f;
			bAst->vel[0] += xdir*15.0f + rnd()*0.1;
			bAst->vel[1] += ydir*15.0f + rnd()*0.1;
			bAst->color[0] = 1.0f;
			bAst->color[1] = 1.0f;
			bAst->color[2] = 1.0f;
			g.astBull++;
			// g.nbullets++;
			// decrementAmmo();
			if (g.astBull == MAX_BULLETS) {
				g.astBull = 0;
			}
		}
	}
}
void eFireMachineGun(Asteroid *a, int k)
{
	// Asteroid *a = g.ahead;
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.enemyBulletTimer[k], &bt);
	if(ts > 0.2) {
		//healPlayer();
		timeCopy(&g.enemyBulletTimer[k], &bt);
		if (g.astBull < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.bulletSound);
			t1.detach();
			//Bullet *b = new Bullet;
			Bullet *bAst = &g.barrAst[g.astBull];
			timeCopy(&bAst->time, &bt);
			bAst->pos[0] = a->pos[0];
			bAst->pos[1] = a->pos[1];
			bAst->vel[0] = a->vel[0];
			bAst->vel[1] = a->vel[1];
			//convert ship angle to radians
			Flt rad = ((a->angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			bAst->pos[0] += xdir*20.0f;
			bAst->pos[1] += ydir*20.0f;
			bAst->vel[0] += xdir*15.0f + rnd()*0.1;
			bAst->vel[1] += ydir*15.0f + rnd()*0.1;
			bAst->color[0] = 1.0f;
			bAst->color[1] = 1.0f;
			bAst->color[2] = 1.0f;
			g.astBull++;
			// g.nbullets++;
			// decrementAmmo();
			if (g.astBull == MAX_BULLETS) {
				g.astBull = 0;
			}
		}
	}
}
void eFireShotgun(Asteroid *a, int k)
{
	// Asteroid *a = g.ahead;
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.enemyBulletTimer[k], &bt);
	if(ts > 1.0) {
		//healPlayer();
		timeCopy(&g.enemyBulletTimer[k], &bt);
		if (g.astBull < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.bulletSound);
			t1.detach();
			//Bullet *b = new Bullet;
			Bullet *bAst = &g.barrAst[g.astBull];
			timeCopy(&bAst->time, &bt);
			bAst->pos[0] = a->pos[0];
			bAst->pos[1] = a->pos[1];
			bAst->vel[0] = a->vel[0];
			bAst->vel[1] = a->vel[1];
			//convert ship angle to radians
			Flt rad = ((a->angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			bAst->pos[0] += xdir*20.0f;
			bAst->pos[1] += ydir*20.0f;
			bAst->vel[0] += xdir*15.0f + rnd()*0.1;
			bAst->vel[1] += ydir*15.0f + rnd()*0.1;
			bAst->color[0] = 1.0f;
			bAst->color[1] = 1.0f;
			bAst->color[2] = 1.0f;
			g.astBull++;
			// g.nbullets++;
			// decrementAmmo();
			if (g.astBull == MAX_BULLETS) {
				g.astBull = 0;
			}
		}
	}
}
void generatePellet(timespec bt){
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
	b->vel[0] += xdir*15.0f + rnd()*.5;
	b->vel[1] += ydir*15.0f + rnd()*.5;
	b->color[0] = 1;
	b->color[1] = 1.0f;
	b->color[2] = 1.0f;
	g.nbullets++;
}
void fireShotgun(){
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if(ts>1){
		timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < MAX_BULLETS-6) {
			thread t1(play_sound, gl.sSound);
			t1.detach();
			generatePellet(bt);
			generatePellet(bt);
			generatePellet(bt);
			generatePellet(bt);
			generatePellet(bt);
			generatePellet(bt);
			decrementAmmo();
		}
	}
}
void fireMachineGun(){
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if(ts>0.2){
		timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < MAX_BULLETS) {
			//shoot a bullet...
			thread t1(play_sound, gl.bulletSound);
			t1.detach();
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
			b->vel[0] += xdir*15.0f + rnd()*0.1;
			b->vel[1] += ydir*15.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
			decrementAmmo();
		}
	}
}/*
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
}*/
void show_credits()
{
	Rect r;
	r.bot=(gl.yres/5)+600;
	r.left=gl.xres/2;
	ggprint16(&r, 16, 0x00ff0000, "Members:");
	extern void art_credits(int x, int y);
	extern void edwin_credits(int x, int y);
	extern void andrew_credits(int x, int y);
	extern void bryan_credits(int x, int y);
	extern void joel_credits(int x, int y);
	art_credits( gl.xres/2-100,gl.yres/5+500);
	extern void art_picture(int x, int y, GLuint textid);
	extern void edwin_picture(int x, int y, GLuint textid);
	extern void andrew_picture(int x, int y, GLuint textid);
	extern void bryan_picture(int x, int y, GLuint textid);
	extern void joel_picture(int x, int y, GLuint textid);
	art_picture(gl.xres/2,gl.yres/5+500,gl.artTexture);
	edwin_credits(gl.xres/2-100,gl.yres/5+400);
	edwin_picture(gl.xres/2, gl.yres/5+400, gl.edwinTexture);
	andrew_credits(gl.yres/5+300,gl.xres/2-100);
	andrew_picture(gl.xres/2, gl.yres/5+300, gl.andrewTexture);
	bryan_credits(gl.xres/2-100,gl.yres/5+200);
	bryan_picture(gl.xres/2, gl.yres/5+200, gl.bryanTexture);
	joel_credits(gl.xres/2-100,gl.yres/5+100);
	joel_picture(gl.xres/2, gl.yres/5+100, gl.sTexture);
}
extern int  getCreditState();
extern int getMenuState();
void render()
{
	x11.clear_screen();
	Rect r;
	r.bot = g.ship.pos[1]+260;
	r.left = g.ship.pos[0]-450;
	r.center = 0;
	Rect bR;
	bR.bot = g.ship.pos[1] - 290;
	bR.left = g.ship.pos[0];
	r.center = 0;
	if(getMenuState()){
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
		genBackground(gl.tileTexture);
		genTitleScreen(gl.logoTexture,gl.textTexture, gl.xres/2,gl.yres/2);
		extern void main_menu(int x, int y);
		main_menu(gl.xres/2,gl.yres/2);
	}
	else if (getCreditState()) {
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
		//if (getCreditState()){
		show_credits();
	} else if(playerIsAlive()){
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		//glOrtho((g.ship.pos[0]-gl.xres/5), (g.ship.pos[0]+gl.xres/5), (g.ship.pos[1]-gl.yres/5)-getShake(), (g.ship.pos[1]+gl.yres/5)-getShake(), -100, 1000);
		
		if(!isTransitionComplete()){
			glOrtho((g.ship.pos[0]-gl.xres/5), (g.ship.pos[0]+gl.xres/5), (g.ship.pos[1]-gl.yres/5)-getShake(), (g.ship.pos[1]+gl.yres/5)-getShake(), -100, 1000);
		}else{
		if(isTransitionComplete()){
			zoomOut(0, gl.xres, 0, gl.yres);
			}
		}
		genBackground(gl.grassTexture);
		/*
		   healthPack(gl.hpTexture,500,600,0);
		   healthPack(gl.hpTexture,500,800,1);
		   healthPack(gl.hpTexture,500,1000,2);
		   healthPack(gl.hpTexture,500,1200,3);
		   healthPack(gl.hpTexture,500,1400,4);
		   */
		//Draw Map
		//Draw Roads
		extern void genRoadHorizontal(int x, int y, GLuint texid);
		extern void genRoadVertical(int x, int y, GLuint texid);
		for (int i = -7; i < 35; i++) {
			genRoadHorizontal((i*128), 192, gl.roadTexture);
		}
		for (int i = -7; i < 35; i++) {
			genRoadHorizontal((i*128), 1408, gl.roadTexture);
		}
		for (int i = -7; i < 35; i++) {
			genRoadVertical(512, (i*128), gl.roadTexture);
		}
		for (int i = -7; i < 35; i++) {
			genRoadVertical(1024, (i*128), gl.roadTexture);
		}
		for (int i = -7; i < 35; i++) {
			genRoadVertical(512, (i*128), gl.roadTexture);
		}
		//Draw Walls
		extern void genWall(int x, int y, GLuint texid);
		extern void genWallCorner(int x, int y, int angle, GLuint texid);
		extern void genRocks(GLuint &texid1, GLuint &texid2);
		extern void genBush(int x, int y, GLuint texid);
		for (int i = 0; i < 27; i++) {
			for (int j = 0; j < 2; j++){
				if (j == 0) {
					genRocks(gl.rockTexture1,gl.rockTexture2);
				}
				else {
					genRocks(gl.rockTexture1, gl.rockTexture2);
				}
			}
		}
		//House 1
		genWallCorner(1640, 768+412, 0, gl.wallCorner);
		genWall(1704, 768+412, gl.wallT);
		genWall(1768, 768+412, gl.wallEmpty);
		genWallCorner(1832, 768+412, 270, gl.wallCorner);
		genWall(1832, 704+412, gl.wallR);
		genWall(1832, 640+412, gl.wallR);
		genWall(1832, 576+412, gl.wallR);
		genWallCorner(1832, 512+412, 180, gl.wallCorner);
		genWall(1704, 512+412, gl.wallB);
		genWall(1768, 512+412, gl.wallB);
		genWallCorner(1640, 512+412, 90, gl.wallCorner);
		genWall(1640, 704+412,gl.wallL);
		genWall(1640, 640+412, gl.wallL);
		genWall(1640, 576+412, gl.wallL);
		genWall(1768, 576+412, gl.wallEmpty);
		genWall(1704, 576+412, gl.wallEmpty);
		genWall(1768, 640+412, gl.wallEmpty);
		genWall(1704, 640+412, gl.wallEmpty);
		genWall(1768, 704+412, gl.wallEmpty);
		genWall(1704, 704+412, gl.wallEmpty);
		//House 2
		genWallCorner(640, 768, 0, gl.wallCorner);
		genWall(704, 768, gl.wallT);
		genWall(768, 768, gl.wallT);
		genWallCorner(832, 768, 270, gl.wallCorner);
		genWall(832, 704, gl.wallR);
		genWall(832, 640, gl.wallR);
		genWall(832, 576, gl.wallEmpty);
		genWallCorner(832, 512, 180, gl.wallCorner);
		genWall(704, 512, gl.wallB);
		genWall(768, 512, gl.wallB);
		genWallCorner(640, 512, 90, gl.wallCorner);
		genWall(640, 704, gl.wallL);
		genWall(640, 640, gl.wallL);
		genWall(640, 576, gl.wallL);
		genWall(768, 576, gl.wallEmpty);
		genWall(704, 576, gl.wallEmpty);
		genWall(768, 640, gl.wallEmpty);
		genWall(704, 640, gl.wallEmpty);
		genWall(768, 704, gl.wallEmpty);
		genWall(704, 704, gl.wallEmpty);
		//House 3
		genWallCorner(640-576, 768+468, 0, gl.wallCorner);
		genWall(704-576, 768+468, gl.wallT);
		genWall(768-576, 768+468, gl.wallT);
		genWallCorner(832-576, 768+468, 270, gl.wallCorner);
		genWall(832-576, 704+468, gl.wallR);
		genWall(832-576, 640+468, gl.wallR);
		genWall(832-576, 576+468, gl.wallEmpty);
		genWallCorner(832-576, 512+468, 180, gl.wallCorner);
		genWall(704-576, 512+468, gl.wallB);
		genWall(768-576, 512+468, gl.wallB);
		genWallCorner(640-576, 512+468, 90, gl.wallCorner);
		genWall(640-576, 704+468, gl.wallL);
		genWall(640-576, 640+468, gl.wallL);
		genWall(640-576, 576+468, gl.wallL);
		genWall(768-576, 576+468, gl.wallEmpty);
		genWall(704-576, 576+468, gl.wallEmpty);
		genWall(768-576, 640+468, gl.wallEmpty);
		genWall(704-576, 640+468, gl.wallEmpty);
		genWall(768-576, 704+468, gl.wallEmpty);
		genWall(704-576, 704+468, gl.wallEmpty);
		genTrees(gl.treeTexture);
		//for (int i = 0; i)
		gunSpawnManager(g.itemTimer);
		if(!isTransitionComplete()){
			genRifle(gl.rTexture);
			genShotgun(gl.sTexture);
			genMachineGun(gl.mgTexture);
		}
		//-------------
		//Draw the ship/player
		glColor3fv(g.ship.color);
		glPushMatrix();
		glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
		glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(-2.0f, -0.0f);
		glVertex2f(  0.0f, 2.0f);
		glVertex2f(  0.0f, -1.0f);
		glVertex2f(  0.0f, -1.0f);
		glVertex2f(  0.0f, 2.0f);
		glVertex2f( 2.0f, -1.0f);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();
		cout << "X: " <<g.ship.pos[0] << endl;
		cout << "Y: " <<g.ship.pos[1] << endl;
		if(getIntroComplete()&&!isTransitionComplete()){
			getCharacter();
		}
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
		//Draw the enemies
		Asteroid *a = g.ahead;
		while (a) {
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(-1.0f, -1.0f);
			glVertex2f(  0.0f, 2.0f);
			glVertex2f(  0.0f, -1.0f);
			glVertex2f(  0.0f, -1.0f);
			glVertex2f(  0.0f, 2.0f);
			glVertex2f( 2.0f, -1.0f);
			glEnd();
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(0.0f, 0.0f);
			glEnd();
			glPopMatrix();
			if(getIntroComplete()&&!playerHasWon()){
				extern void enemy(int x, int y, int z, float angle, GLuint texid);
				extern void bigBoss(int x, int y, int z, float angle, GLuint texid);
				if(a->isBoss==1){

				bigBoss(a->pos[0], a->pos[1], a->pos[2], a->angle, gl.bossTexture);
			//	a->drawHealthBar(a->pos[0]-150,a->pos[1]+150);
				ggprint16(&bR, 16, 0x00ffffff, "Itheral, devourer of souls.");
				a->drawHealthBar(g.ship.pos[0]-150, g.ship.pos[1]-300);
				}else if (a->gunNum == 2&&a->isBoss==0) {

					enemy(a->pos[0], a->pos[1], a->pos[2], a->angle+90, gl.characterHandgun);
					a->drawHealthBar(a->pos[0]-20,a->pos[1]+25);
				}else if(a->isBoss==0){ enemy(a->pos[0], a->pos[1], a->pos[2], a->angle+90, gl.characterRifle);
					a->drawHealthBar(a->pos[0]-20,a->pos[1]+25);
				}
			}
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
		if(!isTransitionComplete()){
			for (int i=0; i<g.nbullets; i++) {
				//Log("draw bullet...\n");
				glColor3f(1.0, 1.0, 0.0);
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
				glColor3f(1.0, 1.0, 0.0);
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
		if(!playerHasWon()){
			healthPack(gl.hpTexture,500,600,0);
			healthPack(gl.hpTexture,500,800,1);
			healthPack(gl.hpTexture,500,1000,2);
			healthPack(gl.hpTexture,500,1200,3);
			healthPack(gl.hpTexture,500,1400,4);
			health_bar(g.ship.pos[0]-450,g.ship.pos[1]+350);
		}
		/*
		if(g.nasteroids==1){
			extern void bigBoss(int x, int y, int z, float angle, GLuint texid);
			Asteroid *a = g.ahead;
			a->health=500;
			a->maxHp=500;
			a->isBoss=1;
			bigBoss(a->pos[0], a->pos[1], a->pos[2], a->angle+90, gl.characterHandgun);
			extern void death_circle();
			a->pos[0]=1638;
			a->pos[1]=182;
			death_circle();
			isInsideDeath(1638, 182,1000,g.ship.pos[0],g.ship.pos[1]);
		}*/
		
		letterBoxes(g.ship.pos[0],g.ship.pos[1], gl.goTexture);
		if(!hasReadyGoBeenSaid()){
			thread r1(readyGo);
			r1.detach();
		}
		if(!playerHasWon()){
		ggprint16(&r, 16, 0x00ffffff, "3350 - CSUB Battle Royale");
		if (g.ahead->isBoss==0)
			ggprint16(&r, 16, 0x00bbbbbb, "Enemies Remaining: %i", g.nasteroids);
		else
			ggprint16(&r, 16, 0x00bbbbbb, "Enemies Remaining: %i", g.nasteroids);
		genAmmo(gl.bulletTexture, g.ship.pos[0]-460,g.ship.pos[1]+300);
		reloadAmmunition();
		printCurrentWeapon(getCurrentWeapon(),r);
		}
		}else if(!playerIsAlive()){
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			if(!doneFading()){
				fadeToBlack();
			}else{
				glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
				if(deathSoundPlayed()==0){
					thread td(play_sound,gl.youDiedSound);
					td.detach();
				}
				drawYouDied2(gl.ydTexture,gl.xres/2,gl.yres/2);
			}
		}
		if(playerHasWon()==1){
			drawLine(0,0);
			
			if(getZoom()>=599){
				if(!winSoundPlayed()){
					thread y1(play_sound, gl.mgSound);
					y1.detach();			
				}
				drawWinText(gl.ywTexture, gl.xres/2,gl.yres/2);

			}
		}	
	}
