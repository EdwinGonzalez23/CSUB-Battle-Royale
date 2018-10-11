//Arturo Urbina
//date:September 25, 2018
//defined variables and functions to toggle Credits
#include <iostream>
#include <GL/glu.h>
#include "fonts.h"
#include<math.h>
static int creditsState;
int getCreditState()
{
	return creditsState;
}
void toggleCredits()
{
	creditsState^=1;
}
void  art_credits(int x, int y)
{
	Rect r;
	r.bot =y;
	r.left=x;
	ggprint8b(&r,16,0x00FFFF00,"Arturo Urbina");
}

void art_picture(int x,int y,GLuint textid)
{
	static float angle=0.0f;
	float fx=(float)x;
	float fy=(float)y;
	fx+=sin(angle)*10.0f;
	fy+=cos(angle)*10.0f;
	angle+=5.0f;
	glColor3ub(255,255,255);
	int wid=50;
	glPushMatrix();
	glTranslatef(x,y,0);
	glRotatef(angle, 0.0f,0.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D,textid);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(1.0f,0.0f); glVertex2i(-wid,wid);
	glTexCoord2f(1.0f,1.0f); glVertex2i(wid,wid);
	glTexCoord2f(0.0f,1.0f); glVertex2i(wid,-wid);
	glEnd();
	glPopMatrix();
	return;
}
