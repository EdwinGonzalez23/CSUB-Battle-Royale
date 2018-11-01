//Resource file for Andrew Burt.
//This file will include the functions to display graphics to the program.

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
//#include "game.h"

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
/*
int numberofwalls = 0;
//Structures for Shapes
struct Vect {
    float x;
    float y;
    float z;
};

    struct Color {
    int r;
    int g;
    int b;
};

    struct Shape {
    float height;
    float width;
    float radius;
    Vect center;
    Color color;
};

void draw_wall(struct Vect walls, struct Color walls, struct Shape Walls) {
    Shape *walls;
    float w = width;
    float h = height;
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3ub(1.0,0.0,0.0);
    glPushMatrix();
    glTranslatef(s->center.x, s->center.y, s->center.z);
    glBegin(GL_QUADS);
    glVertex2i(-w, -h);
    glVertex2i(-w,  h);
    glVertex2i( w,  h);
    glVertex2i( w, -h);
    glEnd();
    glPopMatrix();
    walls = &g.walls[]
    }
};

void draw_wall()
{
    Shape *s;
    float w, h;
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3ub(1.0,0.0,0.0);
    glPushMatrix();
    glTranslatef(s->center.x, s->center.y, s->center.z);
    w = s->width;
    h = s->height;
    glBegin(GL_QUADS);
    glVertex2i(-w, -h);
    glVertex2i(-w,  h);
    glVertex2i( w,  h);
    glVertex2i( w, -h);
    glEnd();
    glPopMatrix();
    s = &g.wall[0];
}

void setup_house()
{
    wall[0].width = 60;
    wall[0].height = 5;
    wall[0].center.x = gl.xres * 0.20;
    wall[0].center.y = gl.yres * 0.80;
    wall[0].color.r = 1.0;
    wall[0].color.g = 0.0;
    wall[0].color.b = 0.0;
    
 }
*/

