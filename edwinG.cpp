/* Edwin Gonzalez
 * Special Functions to interact with main project
 *
 */
#include <iostream>
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
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

// bool isInSector(float xLen, float yLen, int tracker, float xPos, float yPos)
// {
//     int numSections = 3;
//     float xSectLen = (xLen/numSections);
//     float ySectLen = (yLen/numSections);
//     switch (playerIndex) {
//         case 1:
//             if ((xPos < xSectLen) && (xPos > 1.0f) &&
//             (yPos < yLen) && (yPos > ((ySectLen*2)+1))) {
//                 return true;
//             }
//         case 2:
//         case 3:
//         case 4:
//         case 5:
//         case 6:
//         case 7:
//         case 8:
//         case 9:
//
//}

// void updateAsteroidPos()
// {
//     int j = 0;
// 	while (j < g.astBull) {
// 		Bullet *bAst = &g.barrAst[j];
// 		double ts = timeDiff(&bAst->time, &bt);
// 		if (ts > 2.5) {
// 			memcpy(&g.barrAst[j], &g.barrAst[g.astBull-1],
// 					sizeof(Bullet));
// 			g.astBull--;
// 			continue;
// 		}
// 		bAst->pos[0] += bAst->vel[0];
// 		bAst->pos[1] += bAst->vel[1];
// 		j++;
// 	// }
// }
// }




//Testing Console Log (ignore)
// cout << "\n";
// 		cout << "Asteroid PosX: " << a->pos[0] << endl;
// 		cout << "Asteroid PosY: " << a->pos[1] << endl;
// 		cout << "SHIP X: " << g.ship.pos[0] << endl;
// 		cout << "SHIP Y: "<< g.ship.pos[1] << endl;
// 		cout << "xTan: " << xTan << endl;
// 		cout << "yTan: "<< yTan << endl;
// 		//cout << "divTan: "<< divTan << endl;
// 		//cout << "divTanLow: "<< divTanLow << endl;
// 		cout << "\ndivTanAngle: "<< divTanAngle << endl;
// 		cout << "\nAsteroid rot: " << a->rotate << endl;
// 		cout << "Asteroid angle: " << a->angle << endl;
// 		cout << "Ship angle: " << g.ship.angle << endl;
// 		Flt radss = ((a->angle+90.0) / 360.0f) * PI * 2.0;
// 		cout << "Rad Ast Angle: " << radss << endl;
//
//
// 		cout << "\n";
