//Arturo Urbina
//date:September 25, 2018
//defined variables and functions to toggle Credits
//TO DO LIST:
//	Add functions to  create random objects
//	Add function for player health.
#include <iostream>
#include <GL/glu.h>
#include "fonts.h"
#include<math.h>
#include "csub.h"
typedef float Flt;
static int creditsState;
static int menuState=1;
//struct Vec{
//	Flt x, y, z;
//};
struct HP{
	int life=100;
	int shield=0;
};
int Get_HP_life(HP value)
{
	return value.life;
}
int Get_HP_shield(HP value)
{
	return value.shield;
}

struct Shape{
	Flt width, height;
	Vec center;
};
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0

int getCreditState()
{
	return creditsState;
}
int getMenuState()
{
	return menuState;
}
void toggleCredits()
{
	creditsState^=1;
}
void toggleMenu()
{
	menuState^=1;
}
void  art_credits(int x, int y)
{
	Rect r;
	r.bot =y;
	r.left=x;
	ggprint8b(&r,16,0x00FFFF00,"Arturo Urbina");
}
/* 
 * ===  FUNCTION  ==============================================================
 *         Name:  main_menu
 *  Description:Main menu to configure options for game. 
 * =============================================================================
 */
	void
main_menu (int x,int y )
{
	Rect r;
	r.bot =y;
	r.left=x;
	ggprint16(&r,16,0x0001FF00,"MAIN MENU");
	ggprint16(&r,16,0x0001FF00,"");
	ggprint16(&r,16,0x00FFFF00,"In future here you will be able to enable options for game");
	ggprint16(&r,16,0x00FF0F00,"Press M to start game");
}		/* -----  end of function main_menu  ----- */
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
	glTranslatef(fx,fy,0);
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  death_circle
 *  Description:  
 * =====================================================================================
 */
	void death_circle ( int x, int y, int time )
{
	return;
}		/* -----  end of function death_circle  ----- */
