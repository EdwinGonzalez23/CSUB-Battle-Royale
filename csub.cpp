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
Image img[16]={"art.jpg","joel_pic.jpg","edwinImg.png","bryan_picture.jpg","1.jpg",
	"rifleCrate.png","shotgunCrate.png","machineGunCrate.png", "./images/models/handgun.png",
	"./images/models/rifle.png", "./images/models/shotgun.png", "./images/models/knife.png",
	"bullet2.png","bg2.jpeg","tree2.png","you_died.png"};
void setup_sound(Global &gl){
	alutInit (NULL, NULL);
	gl.buffers[0] = alutCreateBufferFromFile ("./audio/gunshot.wav");
	gl.buffers[1] = alutCreateBufferFromFile ("./audio/You_Died.wav");
	gl.buffers[2] = alutCreateBufferFromFile ("./audio/playerHit.wav");
	alGenSources (1, &gl.bulletSound);
	alGenSources (1, &gl.youDiedSound);
	alGenSources (1, &gl.playerHitSound);
	alSourcei (gl.bulletSound, AL_BUFFER, gl.buffers[0]);
	alSourcei (gl.youDiedSound, AL_BUFFER, gl.buffers[1]);
	alSourcei (gl.playerHitSound, AL_BUFFER, gl.buffers[2]);

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
extern void printAmmo();
extern void decrementAmmo();
extern void genAmmo(GLuint texture);
extern void reloadAmmunition();
extern int hasBulletsLoaded(int x);
extern void genBackground(GLuint bg);
extern void genTree(GLuint tree,int x, int y);
extern bool playerIsAlive();
extern void drawYouDied(GLuint texture,int x, int y);
extern void drawYouDied2(GLuint texture, int x, int y);
extern bool deathSoundPlayed();
extern void fadeToBlack();
extern bool doneFading();
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

	//Handgun Model
	glGenTextures(1, &gl.characterHandgun);
	w = img[8].width;
	h = img[8].height;

	glBindTexture(GL_TEXTURE_2D, gl.characterHandgun);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *HandgunData = buildAlphaData(&img[9]);
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
//int sectLen = setSectLen(); //Default 3 sections
int flipVel[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void physics()
{
	float xLen = gl.xres;
	float yLen = gl.yres;
	Asteroid *a = g.ahead;
	//Initial Spawn
	if(!hasSpawned) {
		int spawnX = xLen/3;
		int spawnY = yLen/3;
		int section = 1;
		Asteroid *a = g.ahead;
		int tracker = 1;
		int xCord, yCord;
		while (a) {
			xCord = spawnRand_XSection(spawnX, section);
			yCord = spawnRand_YSection(spawnY, section);
			a->pos[0] = xCord; //spawnRand_XSection(spawnX, section);
			a->pos[1] = yCord; //spawnRand_YSection(spawnY, section);
			a = a->next;
			section++;
			tracker++;
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
	g.ship.pos[0] += g.ship.vel[0];
	g.ship.pos[1] += g.ship.vel[1];
	//Check for collision with window edges
	//window collisions
	if (g.ship.pos[0] < 0.0) {
		//g.ship.pos[0] += (float)gl.xres;
		g.ship.pos[0] = 0.0;
	}
	else if (g.ship.pos[0] > (float)gl.xres) {
		//g.ship.pos[0] -= (float)gl.xres;
		g.ship.pos[0] = (float)gl.xres;
	}
	else if (g.ship.pos[1] < 0.0) {
		//g.ship.pos[1] += (float)gl.yres;
		g.ship.pos[1] = 0.0;
	}
	else if (g.ship.pos[1] > (float)gl.yres) {
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

	float angleLockOn;// trigger;
	float accuracy = 90;
	//float triggerDist = setTriggerDist();
	a->angle = unitLock(a->angle);
	int k = 0;
	int velSwitchCounter = 0;
	int enemyTracker = 1;
	while (a) {
		if (flipVel[velSwitchCounter]==0){
			a->pos[0] += a->vel[0];
			a->pos[1] += a->vel[1];
			a->angle += a->rotate;
		}
		if (flipVel[velSwitchCounter]==1) {
			a->pos[0] -= a->vel[0];
			a->pos[1] -= a->vel[1];
			a->angle += a->rotate;
		}
		//Check for collision with window edges
		if (a->pos[0] < xNegCheck(xLen, enemyTracker)) { //XNEG
			//a->pos[0] += (float)(gl.xres)+200;
			if (flipVel[velSwitchCounter] == 1) {
				flipVel[velSwitchCounter] = 0;
			} else
				flipVel[velSwitchCounter] = 1;
		}
		else if (a->pos[0] > xPlusCheck(xLen, enemyTracker)) {//float)gl.xres+100
			//a->pos[0] -= (float)gl.xres+200; //XPOS
			if (flipVel[velSwitchCounter] == 1) {
				flipVel[velSwitchCounter] = 0;
			} else
				flipVel[velSwitchCounter] = 1;
		}
		else if (a->pos[1] < yNegCheck(yLen, enemyTracker)) {//-100.0
			//a->pos[1] += (float)gl.yres+200; //YNEG
			if (flipVel[velSwitchCounter] == 1) {
				flipVel[velSwitchCounter] = 0;
			} else
				flipVel[velSwitchCounter] = 1;
		}
		else if (a->pos[1] > yPlusCheck(yLen, enemyTracker)) {
			//a->pos[1] -= (float)gl.yres+200; //YPOS
			if (flipVel[velSwitchCounter] == 1) {
				flipVel[velSwitchCounter] = 0;
			} else
				flipVel[velSwitchCounter] = 1;
		}
		Bullet *bAst = &g.barrAst[g.astBull];
		//trigger = setTrigger(a->pos[0], a->pos[1], g.ship.pos[0], g.ship.pos[1]);
		//bool inSector = isInSector(xLen, yLen, enemyTracker, xLen, yLen);
		//All angles have been calculated, shoot if triggered
		extern bool enemyShoot(float, float, float, float);
		if (enemyShoot(a->pos[0], a->pos[1], g.ship.pos[0], g.ship.pos[1])) {
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
	if(ts>0.2){
		damagePlayer();
		thread ph(play_sound, gl.playerHitSound);
		ph.detach();
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
			decrementAmmo();

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
			b->vel[0] += xdir*6.0f + rnd()*0.1;
			b->vel[1] += ydir*6.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g.nbullets++;
			decrementAmmo();
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
	r.bot = gl.yres - 55;
	r.left = 10;
	r.center = 0;
	if(getMenuState()){
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);

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
			glOrtho(g.ship.pos[0]-gl.xres/5, g.ship.pos[0]+gl.xres/5, g.ship.pos[1]-gl.yres/5, g.ship.pos[1]+gl.yres/5, -100, 1000);
			genBackground(gl.andrewTexture);
			health_bar(gl.xres,gl.yres);
			ggprint16(&r, 16, 0x00ffffff, "3350 - CSUB Battle Royale");
			ggprint16(&r, 16, 0x00bbbbbb, "Bullets On Screen: %i", g.nbullets);
			ggprint16(&r, 16, 0x00bbbbbb, "Enemies Remaining: %i", g.nasteroids);
			ggprint16(&r, 16, 0x00bbbbbb, "Enemies Defeated: %i ",g.astr_destroyed);
			genAmmo(gl.bulletTexture);
			reloadAmmunition();
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

			getCharacter();


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
				extern void enemy(int x, int y, int z, float angle, GLuint texid);
				enemy(a->pos[0], a->pos[1], a->pos[2], a->angle, gl.characterRifle);
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
			genTree(gl.treeTexture,100,550);
                	genTree(gl.treeTexture,1100,700);
                	genTree(gl.treeTexture,900,250);
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
		
}
