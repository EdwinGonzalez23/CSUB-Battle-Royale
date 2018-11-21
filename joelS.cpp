//joelS.cpp
//Author: Joel Staggs


//I am using ALL of these.

//Debugging
#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
#include <AL/alut.h>
//Sleep function.
#include <unistd.h>
static int currentWeapon=1;
static int playerMaxHP = 100;
static int playerCurrentHP = 100;
static int playerHPMissing = 0;
static bool playerAliveStatus = 1;
static int xBoundary = 0;
static int yBoundary = 0;
//Item Locations.
static int riflePosX = 0;
static int riflePosY = 0;
static int shotgunPosX = 0;
static int shotgunPosY = 0;
static int machineGunPosX = 0;
static int machineGunPosY = 0;
static int gunsSpawned = 0;
static bool rifleOnScreen = 0;
static bool shotgunOnScreen = 0;
static bool machineGunOnScreen = 0;
//Player's Stats.
static bool playerHasRifle = 0;
static bool playerHasShotgun = 0;
static bool playerHasMachineGun = 0;
static struct timespec currentItemTime;

extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//Ammunition Variables.
static int ammo = 10;
static int ammoCounts[] = {0,10,5,4,20};
static int hasBullets[] = {0,1,1,1,1};
static struct timespec reloadTimers[5];

static int bgPosX = 0;
static int bgPosY = 0;

//Externs
//csub.h
extern void toggleMenu();
extern void beginFade();
extern bool fadeBegin();
extern int getColors();


void genTree(GLuint texture, int x, int y){
	int w = 150;
	//int h = 150;
	glPushMatrix();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

}

void genBackground(GLuint texture){
/*
	glColor3f(0.01,0.45,0.15);
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glBegin(GL_QUAD_STRIP);
        glVertex2f(0,yBoundary);
        glVertex2f(0, 0);
        glVertex2f(xBoundary+25, yBoundary);
        glVertex2f(xBoundary+25,0);
        glEnd();
        glPopMatrix();
*/
	
	bgPosX=0;	
        for(bgPosX = -1000; bgPosX<xBoundary*2;) {
		bgPosY=0;
		for(bgPosY=-1000;bgPosY<yBoundary*2;){
			glColor3ub(getColors(),getColors(),getColors());
			int wid=100;
			glPushMatrix();
			glTranslatef(bgPosX,bgPosY,0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			bgPosY+=200;
		}
		bgPosX+=190;
	}
}


int hasBulletsLoaded(int weap){
	return hasBullets[weap];	
}

void printAmmo(){
	Rect r;
	r.left = 20;
	r.bot  = yBoundary-400;

	for(int i = 0;i<ammo;i++){
		ggprint16(&r,0,0x00bbbb00, " o ");
		r.left+=20;
	}
}

void reloadAmmunition(){
	struct timespec reloadComparison;
	clock_gettime(CLOCK_REALTIME, &reloadComparison);
	double timeDifference;
	for(int i = 1; i<5;i++){
		if(hasBullets[i]==0){
		        timeDifference = timeDiff(&reloadTimers[i], &reloadComparison);
			if(timeDifference>3){
				hasBullets[i]=1;
				if(i==1){
					ammoCounts[i] = 10;
				}else if(i==2){
					ammoCounts[i] = 5;
				}else if(i==3){
					ammoCounts[i] = 4;
				}else if(i==4){
					ammoCounts[i] = 20;
				}
			}	
		}
	}
}

void decrementAmmo(){
	ammoCounts[currentWeapon]--;
	if(ammoCounts[currentWeapon]==0){
		hasBullets[currentWeapon]=0;
		clock_gettime(CLOCK_REALTIME, &reloadTimers[currentWeapon]);
	}
}

void genAmmo(GLuint texture)
{
	int x = 0;
	for(int i = 0; i < ammoCounts[currentWeapon];i++) {
		glColor3ub(255,255,255);
		int w=12;
		int h=16;
		glPushMatrix();
		glTranslatef(x+h,yBoundary-300,0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-h);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( w, h);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		x+=20;

	}

}


//Vim defaults to an indentation of 8,
//So I'm using that.
void setItemBoundary(int x, int y)
{
	xBoundary = x;
	yBoundary = y;
}

void setGunLocation(int &x, int &y)
{
	x = rand() % (xBoundary-300)+50;
	y = rand() % (yBoundary-300)+50;
}

void getRiflePosition(int x[2])
{
	x[0] = riflePosX;
	x[1] = riflePosY;	
}

void getShotgunPosition(int x[2])
{
	x[0] = shotgunPosX;
	x[1] = shotgunPosY;
}

void getMachineGunPosition(int x[2])
{
	x[0] = machineGunPosX;
	x[1] = machineGunPosY;
}

bool rifleIsOnScreen()
{
	return rifleOnScreen;	
}

bool shotgunIsOnScreen()
{
	return shotgunOnScreen;
}

bool machineGunIsOnScreen()
{
	return machineGunOnScreen;
}

bool doesPlayerHaveRifle()
{
	return playerHasRifle;
}

bool doesPlayerHaveShotgun()
{
	return playerHasShotgun;
}

bool doesPlayerHaveMachineGun()
{
	return playerHasMachineGun;
}

void pickUpRifle()
{
	rifleOnScreen = 0;
	playerHasRifle = 1;
}

void pickUpShotgun()
{
	shotgunOnScreen = 0;
	playerHasShotgun = 1;
}

void pickUpMachineGun()
{
	machineGunOnScreen = 0;
	playerHasMachineGun = 1;
}

void genRifle(GLuint texture)
{
	if (rifleOnScreen == 1) {
		glColor3ub(255,255,255);
		int wid=25;
		glPushMatrix();
		glTranslatef(riflePosX,riflePosY,0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void genShotgun(GLuint texture)
{
        if (shotgunOnScreen == 1) {
                glColor3ub(255,255,255);
                int wid=25;
                glPushMatrix();
                glTranslatef(shotgunPosX,shotgunPosY,0);
                glBindTexture(GL_TEXTURE_2D, texture);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                glEnd();
                glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
        }
}

void genMachineGun(GLuint texture){
        if (machineGunOnScreen == 1) {
                glColor3ub(255,255,255);
                int wid=25;
                glPushMatrix();
                glTranslatef(machineGunPosX,machineGunPosY,0);
                glBindTexture(GL_TEXTURE_2D, texture);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
                glEnd();
                glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
        }
}


//This makes sure that the timers have a short gap at runtime.
void timeInit(struct timespec &lastItemTime){
	clock_gettime(CLOCK_REALTIME, &currentItemTime);
	timeCopy(&lastItemTime, &currentItemTime);	
}

void gunSpawnManager(struct timespec &lastItemTime){
	clock_gettime(CLOCK_REALTIME, &currentItemTime);
	double timeDifference = timeDiff(&lastItemTime, &currentItemTime);
	//std::cout<<timeDifference<<std::endl;
	if (timeDifference > 3) {
		timeCopy(&lastItemTime, &currentItemTime);
		switch (gunsSpawned) {
			case 0:
				setGunLocation(riflePosX, riflePosY);
				rifleOnScreen = 1;
				break;
			case 1:
				setGunLocation(shotgunPosX, shotgunPosY);
				shotgunOnScreen = 1;
				break;
			case 2:
				setGunLocation(machineGunPosX,machineGunPosY);
				machineGunOnScreen = 1;
				break;
			default:
				break;

		}
		gunsSpawned++;
	}
}

bool playerIsAlive()
{
	return playerAliveStatus;
}

void killPlayer()
{
	playerAliveStatus=0;
	std::cout<<"You Died!\n";
}
void setPlayerHPMissing()
{
	playerHPMissing = playerMaxHP-playerCurrentHP;
}

void damagePlayer()
{
	if(playerCurrentHP-10<=0){
		playerCurrentHP = 0;
		playerHPMissing = playerMaxHP; 
		killPlayer();
		return;
	}
	playerCurrentHP-=10;
	playerHPMissing+=10;
	std::cout<<playerCurrentHP<<std::endl;
		
}

void healPlayer()
{
	playerCurrentHP=playerMaxHP;
	playerHPMissing=0;
}

void health_bar(int x,int y)
{
	//Current Player HP
	glColor3f((double)playerHPMissing*.08,(double)playerCurrentHP*.01,.4f);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex2f(10,y-10);
	glVertex2f(10, y-30);
	glVertex2f(310, y-10);
	glVertex2f(310,y-30);
	glEnd();
	glPopMatrix();
	//Current MISSING player HP
	glColor3f(0.5f,0,0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex2f(310-playerHPMissing*3,y-10);
	glVertex2f(310-playerHPMissing*3, y-30);
	glVertex2f(310, y-10);
	glVertex2f(310,y-30);
	glEnd();
	glPopMatrix();
}

void play_sound(ALuint src)
{
	alSourcePlay (src);
	sleep (1);
}

void setCurrentWeapon(int newWeapon)
{
	currentWeapon=newWeapon;
}

int getCurrentWeapon()
{
	return currentWeapon;
}

void printCurrentWeapon(int weap, Rect r)
{
	switch (weap) {
		case 1:
			ggprint16(&r,34,0x00bbbbbb, "Weapon mode: Pistol");
			break;
		case 2: 
			ggprint16(&r,16,0x00bbbbbb, "Weapon mode: Rifle");
			break;
		case 3:
			ggprint16(&r,16,0x00bbbbbb, "Weapon mode: Shotgun");
			break;
		case 4:
			ggprint16(&r,16,0x00bbbbbb, "Weapon mode: Machine Gun");
			break;
		default:
			ggprint16(&r,16,0x00bbbbbb, "Weapon mode: Unknown");
	}
}
/*
void play_sound(ALuint src)
{
        alSourcePlay (src);
        sleep (1);
}

void play_BGM(ALuint bgmSrc)
{
        while(1){
                alSourcePlay(bgmSrc);
                sleep(268);
        }
}
*/
//Function required for lab 5
void joel_credits(int x, int y)
{

        Rect r;
        r.bot = y;
        r.left = x;
        ggprint8b(&r, 16, 0x00ff0000, "Joel Staggs");
}

void joel_picture(int x, int y, GLuint texture)
{
        glColor3ub(255,255,255);
        int wid=50;
        glPushMatrix();
        glTranslatef(x,y,0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

        glEnd();
        glPopMatrix();

}
