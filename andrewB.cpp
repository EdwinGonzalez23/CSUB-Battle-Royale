//Resource file for Andrew Burt.
//This file will include the functions to display graphics to the program.

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include "game.h"

using namespace std;
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
    glColor3f(1.0,1.0,1.0);
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
}

void enemy(int x, int y, int z, float angle, GLuint texid)
{
    glPushMatrix();
    glColor3ub(255, 255, 255);
    int w = 50; 
	glTranslatef(x, y, z);
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
/*
GLuint makeTransparentTexture(GLuint texid, Ppmimage * img)
{

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texid);

	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
							img->width, img->height,
							0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	GLuint transparentTexture;
	glGenTextures(1, &transparentTexture);

	glBindTexture(GL_TEXTURE_2D, transparentTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *transparentData = buildAlphaData(img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, transparentData);
	free(transparentData);
	//return transparentTexture;
}
void initAndrewTextures()
{
    int w = 20;
    int h = 20;
    glBindTexture(GL_TEXTURE_2D, gl.character);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *characterData = buildAlphaData(&img[9]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, characterData);
free(characterData);
}
*/
unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
return newdata;
}
