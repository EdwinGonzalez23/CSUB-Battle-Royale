/* Edwin Gonzalez
 * Special Functions to interact with main project
 *
 */

#include <iostream>
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

void edwin_credits(int x, int y){
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

int checkDistApart(int X1, int X2, int Y1, int Y2)
{
		//int X1, X2, Y1, Y2;
		int X = abs(X2 - X1);
		int Y = abs(Y2 - Y1);
		return sqrt((pow(X, 2)) - (pow(Y, 2)));
}

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

float unitLock(float angle)
{
	if (angle >= 360.0f)
		return angle - 360.0f;
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
