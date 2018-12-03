/* Edwin Gonzalez
 * Special Functions to interact with main project
 *
 */
#include <iostream>
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "game2.h"
Game g2;
extern int Rocks[][2];
using namespace std;
//Flip to 1 to set True
//TODO spawn function change constants to variables
//TODO make area, area array accessible outside functions
//Credits screen
void edwin_credits(int x, int y) {
	Rect r;
	r.bot = y;
	r.left = x;
	ggprint8b(&r, 16, 0x00ff0000, "Edwin Gonzalez");
}
void edwin_picture(int x, int y, GLuint texture)
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
//Movement speed
float moveDownSpeed()
{
	return -7.0f;
}

float moveUpSpeed()
{
	return 7.0f;
}

float moveLeftSpeed()
{
	return -7.0f;
}

float moveRightSpeed()
{
	return 7.0f;
}

//Enemy Locking mechanisms
int checkDistApart(int X1, int X2, int Y1, int Y2)
{
	//int X1, X2, Y1, Y2;
	int X = abs(X2 - X1);
	int Y = abs(Y2 - Y1);
	return sqrt((pow(X, 2)) - (pow(Y, 2)));
}

float unitLock(float angle)
{
	if (angle >= 360.0f) {
		return angle - 360.0f;
	} else
		return angle;

}

float lockOnAngle(float xShipPos, float xAstPos, float yShipPos, float yAstPos)
{
	float x = (xShipPos - xAstPos);
	float y = (yShipPos - yAstPos);
	float divTan = (y/x);
	float lockOnAngle;
	if (x < 0 && y >= 0) {
		lockOnAngle = (atan(divTan) * 180 / 3.1415) + 180;
	}
	else if (x < 0 && y < 0) {
		lockOnAngle = (atan(divTan) * 180 / 3.1415) + 180;
	}
	else if (x >= 0 && y <= 0) {
		lockOnAngle = (atan(divTan) * 180 / 3.1415) + 360;
	} else {
		lockOnAngle = abs(atan(divTan) * 180 / 3.1415);
	}
	return lockOnAngle;
}

float setTrigger(float xAstPos, float yAstPos, float xShipPos, float yShipPos)
{
	float astXY, shipXY, trigger;
	astXY = xAstPos + yAstPos;
	shipXY = xShipPos + yShipPos;
	return trigger = abs(astXY - shipXY);
}

float setTriggerDist()
{
	float triggerDist = 350.0;
	return triggerDist;
}

bool enemyShoot(float xEnemPos, float yEnemPos, float xShipPos, float yShipPos)
{
	if (abs(xEnemPos-xShipPos) < 350 && abs(yEnemPos-yShipPos) < 350) {
		return true;
	} else
		return false;
}

//Spawn Parameters
int numAI()
{
	int numAI = 9;
	return numAI;
}

bool spawned()
{
	return true;
}

bool notSpawned()
{
	return false;
}

int setSectLen()
{
	int sectLen = 3;
	return sectLen;
}

//coordiante plain spawning mechanics
int spawnRand_XSection(int x, int xSection)
{
	int randNum;
	//randNum = rand()%(max-min + 1) + min;
	switch (xSection) {
		case 1: randNum = rand()%((x)) + 1;
			return randNum;
			break;
		case 2: randNum = rand()%((x*2-x)+1) + x;
			return randNum;
			break;
		case 3: randNum = rand()%((x*3-(x*2))+1) + x*2;
			return randNum;
			break;
		case 4: randNum = rand()%((x)) + 1;
			return randNum;
			break;
		case 5: randNum = rand()%((x*2-x)+1) + x;
			return randNum;
			break;
		case 6: randNum = rand()%((x*3-(x*2))+1) + x*2;
			return randNum;
			break;
		case 7: randNum = rand()%((x)) + 1;
			return randNum;
			break;
		case 8: randNum = rand()%((x*2-x)+1) + x;
			return randNum;
			break;
		case 9: randNum = rand()%((x*3-(x*2))+1) + x*2;
			return randNum;
			break;
	}

	return 0;
}

int spawnRand_YSection(int y, int ySection)
{
	int randNum;
	//randNum = rand()%(max-min + 1) + min;
	switch (ySection) {
		case 1: randNum = rand()%((y*3-(y*2))+1) + y*2;
			return randNum;
			break;
		case 4: randNum = rand()%((y*2-y)+1) + y;
			return randNum;
			break;   //execution starts at this case label
		case 7: randNum = rand()%((y)) + 1;
			return randNum;
			break;
		case 2: randNum = rand()%((y*3-(y*2))+1) + y*2;
			return randNum;
			break;
		case 5: randNum = rand()%((y*2-y)+1) + y;
			return randNum;
			break;
		case 8: randNum = rand()%((y)) + 1;
			return randNum;
			break;
		case 3: randNum = rand()%((y*3-(y*2))+1) + y*2;
			return randNum;
			break;
		case 6: randNum = rand()%((y*2-y)+1) + y;
			return randNum;
			break;
		case 9: randNum = rand()%((y)) + 1;
			return randNum;
			break;
	}
	return 0;
}

//Check edges of sections
//TODO chane switch cases to two arrays with solutions
float xNegCheck(float totalLen, int playerIndex)
{
	int numSections = 3;
	float sectLen = (totalLen/numSections);
	switch (playerIndex) {
		case 1: return -1.0f;
		case 2: return sectLen;
		case 3: return (sectLen*2);
		case 4: return -1.0f;
		case 5: return sectLen;
		case 6: return (sectLen*2);
		case 7: return -1.0f;
		case 8: return sectLen;
		case 9: return (sectLen*2);
	}
	return -100.0f;
}

float xPlusCheck(float totalLen, int playerIndex)
{
	int numSections = 3;
	float sectLen = (totalLen/numSections);
	switch (playerIndex) {
		case 1: return sectLen;
		case 2: return (sectLen*2);
		case 3: return totalLen;
		case 4: return sectLen;
		case 5: return (sectLen*2);
		case 6: return totalLen;
		case 7: return sectLen;
		case 8: return (sectLen*2);
		case 9: return totalLen;
	}
	return sectLen;
}

float yPlusCheck(float totalLen, int playerIndex)
{
	int numSections = 3;
	float sectLen = (totalLen/numSections);
	switch (playerIndex) {
		case 1: return (totalLen);
		case 2: return (totalLen);
		case 3: return (totalLen);
		case 4: return (totalLen-sectLen);
		case 5: return (totalLen-sectLen);
		case 6: return (totalLen-sectLen);
		case 7: return (totalLen-(sectLen*2));
		case 8: return (totalLen-(sectLen*2));
		case 9: return (totalLen-(sectLen*2));
	}
	return (totalLen+100.0f);
}

float yNegCheck(float totalLen, int playerIndex)
{
	int numSections = 3;
	float sectLen = (totalLen/numSections);
	switch (playerIndex) {
		case 1: return (totalLen-sectLen);
		case 2: return (totalLen-sectLen);
		case 3: return (totalLen-sectLen);
		case 4: return (totalLen-(sectLen*2));
		case 5: return (totalLen-(sectLen*2));
		case 6: return (totalLen-(sectLen*2));
		case 7: return (-1.0f);
		case 8: return (-1.0f);
		case 9: return (-1.0f);
	}
	return (totalLen-sectLen);
}
//All referencing csub.cpp
void drawVertWall(int x, int y, Game &g)
{
	int w = 15;
	int h = 155;
	if((g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+w)&&
			(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+h)){
		if((g.ship.pos[0]>=x-w || g.ship.pos[0]<=x+w)&&
				(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+h)){
			g.ship.pos[0] -= g.ship.vel[0];
		}
		if((g.ship.pos[1] >=y-h || g.ship.pos[1] <=y+h)&&
				(g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+w)){
			g.ship.pos[1] -= g.ship.vel[1];
			g.ship.pos[0] += g.ship.vel[0];
		}
	}
}
void drawHorzWall(int x, int y, Game &g)
{
	int w = 120;
	int h = 15;
	if((g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+w)&&
			(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+h)){
		if((g.ship.pos[0]>=x-w || g.ship.pos[0]<=x+w)&&
				(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+h)){
			g.ship.pos[0] -= g.ship.vel[0];
		}
		if((g.ship.pos[1] >=y-h || g.ship.pos[1] <=y+h)&&
				(g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+w)){
			g.ship.pos[1] -= g.ship.vel[1];
			g.ship.pos[0] += g.ship.vel[0];
		}
	}
}
void drawDoorWall(int x, int y, Game &g)
{
	int xs = x;
	x = x - 20;
	int w = 80;
	int h = 10;
	//LONGER SIDE
	if((g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+(w-40))&&
			(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+(h+20))){
		if((g.ship.pos[0]>=x-w || g.ship.pos[0]<=x+(w-40))&&
				(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+(h+20))){
			g.ship.pos[0] -= g.ship.vel[0];
		}
		if((g.ship.pos[1] >=y-h || g.ship.pos[1] <=y+(h+20))&&
				(g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+(w-40))){
			g.ship.pos[1] -= g.ship.vel[1];
			g.ship.pos[0] += g.ship.vel[0];
		}
	}
	//SHORTER SIDE
	xs = xs + 80;
	int ws = 30;
	int hs = 10;
	if((g.ship.pos[0]>=xs-ws&&g.ship.pos[0]<=xs+(ws+10))&&
			(g.ship.pos[1]>=y-hs&&g.ship.pos[1]<=y+(hs+20))){
		if((g.ship.pos[0]>=xs-ws || g.ship.pos[0]<=xs+(ws+10))&&
				(g.ship.pos[1]>=y-hs&&g.ship.pos[1]<=y+(hs+20))){
			g.ship.pos[0] -= g.ship.vel[0];
		}
		if((g.ship.pos[1] >=y-hs || g.ship.pos[1] <=y+(hs+20))&&
				(g.ship.pos[0]>=xs-ws&&g.ship.pos[0]<=xs+(ws+10))){
			g.ship.pos[1] -= g.ship.vel[1];
			g.ship.pos[0] += g.ship.vel[0];
		}
	}
}
void drawDoorRight(int x, int y, Game &g)
{
	int xs = x;
	int ys = y;
	y = y + 20;
	int w = 10;
	int h = 80;
	//LONGER SIDE
	if((g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+(w+20))&&
			(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+(h+40))){
		if((g.ship.pos[0]>=x-w || g.ship.pos[0]<=x+(w+20))&&
				(g.ship.pos[1]>=y-h&&g.ship.pos[1]<=y+(h+40))){
			g.ship.pos[0] -= g.ship.vel[0];
		}
		if((g.ship.pos[1] >=y-h || g.ship.pos[1] <=y+(h+40))&&
				(g.ship.pos[0]>=x-w&&g.ship.pos[0]<=x+(w+20))){
			g.ship.pos[1] -= g.ship.vel[1];
			g.ship.pos[0] += g.ship.vel[0];
		}
	}
	//SHORTER SIDE
	xs = xs + 10;
	ys = ys - 120;
	int ws = 20;
	int hs = 30;
	if((g.ship.pos[0]>=xs-ws&&g.ship.pos[0]<=xs+(ws))&&
			(g.ship.pos[1]>=ys-hs&&g.ship.pos[1]<=ys+(hs))){
		if((g.ship.pos[0]>=xs-ws || g.ship.pos[0]<=xs+(ws))&&
				(g.ship.pos[1]>=ys-hs&&g.ship.pos[1]<=ys+(hs))){
			g.ship.pos[0] -= g.ship.vel[0];
		}
		if((g.ship.pos[1] >=ys-hs || g.ship.pos[1] <=ys+(hs))&&
				(g.ship.pos[0]>=xs-ws&&g.ship.pos[0]<=xs+(ws))){
			g.ship.pos[1] -= g.ship.vel[1];
			g.ship.pos[0] += g.ship.vel[0];
		}
	}
}
void drawHouse(int centerX, int centerY, Game &g)
{
	//need 3 walls
	drawVertWall((centerX - 120), centerY, g);
	drawVertWall(centerX +  120, centerY, g);
	drawHorzWall(centerX, centerY - 140, g);
	drawDoorWall(centerX, centerY + 140, g);
}
void drawSecondHouse(int x, int y, Game &g)
{
	drawVertWall((x - 120), y, g); //left
	drawDoorRight(x + 120, y, g); //Right bottom door
	drawHorzWall(x, y - 140, g);//Bottom
	drawHorzWall(x+20,y + 140, g);
}
void rockPlayerCollision(int x, int y, int px, int py, Game &g)
{
	if (abs(x-px) <= 30 && abs(y-py) <= 30) {
		g.ship.pos[1] -= g.ship.vel[1];
		g.ship.pos[0] -= g.ship.vel[0];
	}
}
void regulateSpeed(Game &g){
	g.ship.pos[0] += g.ship.vel[0];
	g.ship.pos[1] += g.ship.vel[1];
	for (int i = 0; i < 27; i++) {
		rockPlayerCollision(Rocks[i][0], Rocks[i][1], g.ship.pos[0], g.ship.pos[1], g);
	}
	drawHouse(1738, 1045, g); //Door Top Right
	drawSecondHouse(149, 1115, g);
	drawSecondHouse(723, 646, g);
}
void wallCollision(int x, int y, int i, Bullet *b, int check, Game &g){
	int w = 125;
	int h = 200;
	if((b->pos[0]>=x-w&&b->pos[0]<=x+(w+10))&&
			(b->pos[1]>=y-h&&b->pos[1]<=y+(h))){
		if (check == 1) {
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
			g.nbullets--;
		} else {
			memcpy(&g.barrAst[i], &g.barrAst[g.astBull-1],
					sizeof(Bullet));
			g.astBull--;
		}

	}
}

void rockCollision(int x, int y, int bx, int by, int i, Bullet *b, int check, Game &g)
{
	if (abs(x-bx) <= 45 && abs(y-by) <= 45) {
		if (check == 1) {
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
					sizeof(Bullet));
			g.nbullets--;
		} else {
			memcpy(&g.barrAst[i], &g.barrAst[g.astBull-1],
					sizeof(Bullet));
			g.astBull--;
		}
	}
}
void enemyRockCollision(int x, int y, int ex, int ey, Asteroid *a , int velSwitchCounter, vector<int> &flipVel){
	if (abs(x-ex) <= 45 && abs(y-ey) <= 45) {
		if (flipVel[velSwitchCounter] == 1) {
			flipVel[velSwitchCounter] = 0;
		} else {
			flipVel[velSwitchCounter] = 1;
		}
	}
}
void enemyWallCollision(int x, int y, Asteroid *a, int velSwitchCounter, vector<int> &flipVel){
	int w = 125;
	int h = 200;
	if((a->pos[0]>=x-w&&a->pos[0]<=x+(w+10))&&
			(a->pos[1]>=y-h&&a->pos[1]<=y+(h))) {
		if (flipVel[velSwitchCounter] == 1) {
			flipVel[velSwitchCounter] = 0;
		} else
			flipVel[velSwitchCounter] = 1;
	}

}
void flipPos(vector<int> &flipVel, int velSwitchCounter, Asteroid *a)
{
	if (flipVel[velSwitchCounter]==0){
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		a->angle += a->rotate;
	}
}
void flipNeg(vector<int> &flipVel, int velSwitchCounter, Asteroid *a)
{
	if (flipVel[velSwitchCounter]==1){
		a->pos[0] -= a->vel[0];
		a->pos[1] -= a->vel[1];
		a->angle += a->rotate;
	}
}
void bounceOffWall(vector<int> &flipVel, int velSwitchCounter)
{
	if (flipVel[velSwitchCounter] == 1) {
		flipVel[velSwitchCounter] = 0;
	} else {
		flipVel[velSwitchCounter] = 1;
	}
}
