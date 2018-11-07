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
#include <unistd.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//Audio library
#include <AL/alut.h>
#include <thread>
#include "log.h"
#include "fonts.h"
#include "csub.h"
using namespace std;
Global gl;
Game g;
X11_wrapper x11;
Image img[8]={"art.jpg","joel_pic.jpg","edwinImg.png","bryan_picture.jpg","andrew_picture.jpg",
	"rCrate2.jpg","sCrate2.jpg","mgCrate2.jpg"};
void setup_sound(Global &gl){
	alutInit (NULL, NULL);
	gl.buffers[0] = alutCreateBufferFromFile ("./audio/gunshot.wav");
	alGenSources (1, &gl.bulletSound);
	alSourcei (gl.bulletSound, AL_BUFFER, gl.buffers[0]);
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
		physics();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	alutExit();
	logClose();
	return 0;
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
			int getMenuState();
			if(getMenuState()){
				void toggleMenu();
				toggleMenu();
			}
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
	float triggerDist = 600;
	Asteroid *a = g.ahead;
	a->angle = unitLock(a->angle);
	int k = 0;
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
			double ts = timeDiff(&g.enemyBulletTimer[3], &bt);
			if (ts > 0.1) {
				timeCopy(&g.enemyBulletTimer[k], &bt);
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
				} k++;
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
	if(ts>0.2){
		damagePlayer();
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
			b->vel[0] += xdir*1.0f + rnd()*0.1;
			b->vel[1] += ydir*1.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
		}
	}
}
void fireRifle(){
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if(ts>0.5){
		healPlayer();
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
			b->vel[0] += xdir*20.0f + rnd()*0.1;
			b->vel[1] += ydir*20.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
		}
	}
}
void generatePellet(timespec bt){
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
	b->vel[0] += xdir*10.0f + rnd()*.5;
	b->vel[1] += ydir*10.0f + rnd()*.5;
	b->color[0] = 1;
	b->color[1] = 1.0f;
	b->color[2] = 1.0f;
	g.nbullets++;
}
void fireShotgun(){
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.bulletTimer, &bt);
	if(ts>0.4){
		timeCopy(&g.bulletTimer, &bt);
		if (g.nbullets < MAX_BULLETS-6) {	
			generatePellet(bt);
			generatePellet(bt);
			generatePellet(bt);
			generatePellet(bt);	
			generatePellet(bt);
			generatePellet(bt);
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
			b->vel[0] += xdir*6.0f + rnd()*0.1;
			b->vel[1] += ydir*6.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
		}
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
	joel_picture(500, 300, gl.sTexture);
}
void render()
{
	x11.clear_screen();
	extern int getMenuState();
	if(getMenuState()){
		extern void main_menu(int x, int y);
		main_menu(gl.xres/2,gl.yres/2);
	}else {
		Rect r;
		glClear(GL_COLOR_BUFFER_BIT);
		//
		r.bot = gl.yres - 45;
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
			health_bar(gl.xres,gl.yres);
			ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
			ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
			ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
			ggprint8b(&r, 16, 0x00ffff00, "n asteroids destroyed: %i ",g.astr_destroyed);
			printCurrentWeapon(getCurrentWeapon(),r);
			gunSpawnManager(g.itemTimer);
			genRifle(gl.rTexture);
			genShotgun(gl.sTexture);
			genMachineGun(gl.mgTexture);
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
			// character(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2], g.ship.angle, gl.characterTexture);
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
	}
