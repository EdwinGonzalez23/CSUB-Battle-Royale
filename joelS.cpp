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
	playerCurrentHP-=10;
	playerHPMissing+=10;
	if(playerCurrentHP<=0){
		killPlayer();
	}
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
			ggprint8b(&r,34,0x00ffff00, "Weapon mode: Pistol");
			break;
		case 2: 
			ggprint8b(&r,16,0x00ffff00, "Weapon mode: Rifle");
			break;
		case 3:
			ggprint8b(&r,16,0x00ffff00, "Weapon mode: Shotgun");
			break;
		case 4:
			ggprint8b(&r,16,0x00ffff00, "Weapon mode: Machine Gun");
			break;
		default:
			ggprint8b(&r,16,0x00ffff00, "Weapon mode: Unknown");
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
