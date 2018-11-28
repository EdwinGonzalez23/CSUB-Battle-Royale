//Resource file for Andrew Burt.
//This file will include the functions to display graphics to the program.

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
//#include "csub.h"

using namespace std;

void andrew_credits(int x, int y)
{
    Rect r;
    r.bot = x;
    r.left = y;
    ggprint8b(&r, 16, 0x00ff0000, "Andrew Burt");
}

void andrew_picture(int x, int y, GLuint texid)
{

    glColor3ub(255, 255, 255);
    int w=50;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix();
}


void character(int x, int y, int z, float angle, GLuint texid)
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    int w = 25; 
	glTranslatef(x, y, z);
	glRotatef(angle + 90, 0.0f, 0.0f, 1.0f);
	//+ 90 Accounts for starting angle facing wrong way.
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

void enemy(int x, int y, int z, float angle, GLuint texid)
{
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    int w = 25; 
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

void genRoadHorizontal(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=64;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix(); 
}
void genRoadVertical(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=64;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix(); 
}

void genWall(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=32;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix(); 
}
void genWallCorner(int x, int y, int angle, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=32;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix(); 
}


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

void genBush(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=64;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix(); 
}