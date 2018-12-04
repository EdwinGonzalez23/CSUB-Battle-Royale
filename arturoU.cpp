//Arturo Urbina
//date:September 25, 2018
//defined variables and functions to toggle Credits
//TO DO LIST:
//	Add functions to  create random objects
//	Add function for player health.
#include <iostream>
#include <GL/glu.h>
#include <GL/glx.h>
#include "fonts.h"
#include<math.h>
typedef float Flt;
static int creditsState;
static int menuState=1;
static float cx=1638;
static float cy=182;
static float circle_radius=1000;
static struct timespec circletimer;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
static float circle_stage=0;
float getcircleRadius()
{
	return circle_radius;
}
float getCircleX()
{
	return cx;
}
float getCircley()
{
	return cy;
}
struct Vec{
	Flt x, y, z;
};
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
float getCircleStage()
{
	return circle_stage;
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
	//Rect r;
	//r.bot =y;
	//r.left=x;
	/*
	   ggprint16(&r,16,0x0001FF00,"MAIN MENU");
	   ggprint16(&r,16,0x0001FF00,"");
	   ggprint16(&r,16,0x00FFFF00,"In future here you will be able to enable options for game");
	   ggprint16(&r,16,0x00FF0F00,"Press M to start game");
	   */
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
void bigBoss(int x, int y, int z, float angle, GLuint texid)
{
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	int w = 300;
	glTranslatef(x, y, z);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texid);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
	glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
	glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
	glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  death_circle
 *  Description:  
 * =====================================================================================
 */
void death_circle(float ex, float ey)
{
	float theta = 2 * 3.1415926 / float(1000); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
	float radial_factor = cosf(theta);//calculate the radial factor 
	struct timespec difference;
	clock_gettime(CLOCK_REALTIME, &difference);
	double ts = timeDiff(&circletimer, &difference);
	if(ts>10)
		circle_radius=750;
	float x = circle_radius;//we start at angle = 0 
	float y = 0; 


	glLineWidth(30);
	glBegin(GL_LINE_LOOP); 
	
	for(int ii = 0; ii < 1000; ii++) 
	{ 
		glVertex2f(x + ex, y + ey);//output vertex 

		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 
		float tx = -y; 
		float ty = x; 

		//add the tangential vector 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 

		//correct using the radial factor 
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}
bool isInsideDeath(float circlex, float circley, float rad,float x, float y)
{
	// Compare radius of circle with distance  
	// of its center from given point 
	if ((x - circlex) * (x - circlex) +
			(y - circley) * (y - circley) <= rad * rad)
		return true;
	else
	{
		void extern damagePlayer();
		damagePlayer();
		return false;
	}
}
