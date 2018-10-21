//joelS.cpp
//Author: Joel Staggs

#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
//Audio library
#include <AL/alut.h>
#include <unistd.h>
#include <cstring>
#include <vector>
static int currentWeapon=1;
static int playerMaxHP = 100;
static int playerCurrentHP = 100;
static int playerHPMissing = 0;
static bool playerAliveStatus = 1;
static int xBoundary = 0;
static int yBoundary = 0;
static int itemPosX = 0;
static int itemPosY = 0;
static bool boxSpawned = 0;
static bool boxOnScreen = 0;
static struct timespec currentItemTime;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);



void setItemBoundary(int x, int y){
	xBoundary = x;
	yBoundary = y;
}

void setItemLocation(){
	itemPosX = rand() % xBoundary;
	itemPosY = rand() % yBoundary;
}

void getBoxPosition(int x[2]){
	x[0] = itemPosX;
	x[1] = itemPosY;	
}

bool boxIsOnScreen(){
	return boxOnScreen;	
}

void pickUpBox(){
	boxOnScreen=0;
}

void genBox(){
	if(boxOnScreen==1){
		glColor3f(1.0f,1.0f,1.0f);
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glBegin(GL_QUAD_STRIP);
		glVertex2f(itemPosX,itemPosY+50);
		glVertex2f(itemPosX,itemPosY);
		glVertex2f(itemPosX+50,itemPosY+50);
		glVertex2f(itemPosX+50,itemPosY);
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
	std::cout<<timeDifference<<std::endl;
	if(timeDifference>3){
		timeCopy(&lastItemTime, &currentItemTime);
		if(!boxOnScreen&&!boxSpawned){
			boxSpawned=1;
			setItemLocation();
			boxOnScreen=1;
		}
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

void healPlayer(){
	playerCurrentHP=playerMaxHP;
	playerHPMissing=0;
}

void health_bar(int x,int y)
{
	//Current Player HP
	glColor3f(0,1,0.3f);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex2f(10,y);
	glVertex2f(10, y-30);
	glVertex2f(310, y);
	glVertex2f(310,y-30);
	glEnd();
	glPopMatrix();
	//Current MISSING player HP
	glColor3f(0.5f,0,0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex2f(310-playerHPMissing*3,y);
	glVertex2f(310-playerHPMissing*3, y-30);
	glVertex2f(310, y);
	glVertex2f(310,y-30);
	glEnd();
	glPopMatrix();
}


void play_sound(ALuint src){
	alSourcePlay (src);
	sleep (1);
}

void setCurrentWeapon(int newWeapon){
	currentWeapon=newWeapon;
}

int getCurrentWeapon(){
	return currentWeapon;
}

void printCurrentWeapon(int weap, Rect r){
	switch (weap){
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
void play_BGM(ALuint bgmSrc){
}
*/
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
//Put these in global scope with other prototypes.
//extern void play_BGM(ALuint bgmSrc);
//extern void play_sound(ALuint src);



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
