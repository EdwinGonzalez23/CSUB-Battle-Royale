//Resource file for Andrew Burt.
//This file will include the functions to display graphics to the program.

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include "game3.h"
Game g3;

using namespace std;

int Rocks [][2] = {
                639, 380,
                856, 352,
                786, 912,
                660, 1115,
                359, 1248,
                240, 772,
                191, 352,
                366, 366,
                366, 16,
                716, 35,
                1290, 28,
                2067, 0,
                2711, 28,
                3278, 56,
                3073, 483,
                2849, 609,
                2709, 896,
                2422, 1001,
                2163, 707,
                1883, 476,
                1449, 882,
                1260, 1610,
                1162, 1925,
                1771, 1990,
                2205, 1955,
                2660, 1717,
                2982, 2025,
                };

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
    int w = 150;
	//int h = 150;
	glPushMatrix();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, texid);
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

void genRock(int x, int y, GLuint texid)
{
    int w = 32;
	//int h = 150;
	glPushMatrix();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, texid);
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
	glBindTexture(GL_TEXTURE_2D, 0);;
}

void genHouse(GLuint &wallCorner, GLuint &wallT, GLuint &wallR, GLuint &wallL, GLuint &wallB, GLuint &wallEmpty, int num)
{
	if (num == 1) { 
		genWallCorner(1640, 768+412, 0, wallCorner);
		genWall(1704, 768+412, wallT);
		genWall(1768, 768+412, wallEmpty);
		genWallCorner(1832, 768+412, 270, wallCorner);
		genWall(1832, 704+412, wallR);
		genWall(1832, 640+412, wallR);
		genWall(1832, 576+412, wallR);
		genWallCorner(1832, 512+412, 180, wallCorner);
		genWall(1704, 512+412, wallB);
		genWall(1768, 512+412, wallB);
		genWallCorner(1640, 512+412, 90, wallCorner);
		genWall(1640, 704+412, wallL);
		genWall(1640, 640+412, wallL);
		genWall(1640, 576+412, wallL);

		genWall(1768, 576+412, wallEmpty);
		genWall(1704, 576+412, wallEmpty);
		genWall(1768, 640+412, wallEmpty);
		genWall(1704, 640+412, wallEmpty);
		genWall(1768, 704+412, wallEmpty);
		genWall(1704, 704+412, wallEmpty);
	}
	else if (num == 2) {
		genWallCorner(640, 768, 0, wallCorner);
		genWall(704, 768, wallT);
		genWall(768, 768, wallT);
		genWallCorner(832, 768, 270, wallCorner);
		genWall(832, 704, wallR);
		genWall(832, 640, wallR);
		genWall(832, 576, wallEmpty);
		genWallCorner(832, 512, 180, wallCorner);
		genWall(704, 512, wallB);
		genWall(768, 512, wallB);
		genWallCorner(640, 512, 90, wallCorner);
		genWall(640, 704, wallL);
		genWall(640, 640, wallL);
		genWall(640, 576, wallL);

		genWall(768, 576, wallEmpty);
		genWall(704, 576, wallEmpty);
		genWall(768, 640, wallEmpty);
		genWall(704, 640, wallEmpty);
		genWall(768, 704, wallEmpty);
		genWall(704, 704, wallEmpty);
	}
	else if (num == 3) {
		genWallCorner(640-576, 768+468, 0, wallCorner);
		genWall(704-576, 768+468, wallT);
		genWall(768-576, 768+468, wallT);
		genWallCorner(832-576, 768+468, 270, wallCorner);
		genWall(832-576, 704+468, wallR);
		genWall(832-576, 640+468, wallR);
		genWall(832-576, 576+468, wallEmpty);
		genWallCorner(832-576, 512+468, 180, wallCorner);
		genWall(704-576, 512+468, wallB);
		genWall(768-576, 512+468, wallB);
		genWallCorner(640-576, 512+468, 90, wallCorner);
		genWall(640-576, 704+468, wallL);
		genWall(640-576, 640+468, wallL);
		genWall(640-576, 576+468, wallL);

		genWall(768-576, 576+468, wallEmpty);
		genWall(704-576, 576+468, wallEmpty);
		genWall(768-576, 640+468, wallEmpty);
		genWall(704-576, 640+468, wallEmpty);
		genWall(768-576, 704+468, wallEmpty);
		genWall(704-576, 704+468, wallEmpty);
	}
}

