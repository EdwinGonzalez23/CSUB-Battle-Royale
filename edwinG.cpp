/* Edwin Gonzalez
 * Special Functions to interact with main project
 *
 */

#include <iostream>
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glu.h>

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
