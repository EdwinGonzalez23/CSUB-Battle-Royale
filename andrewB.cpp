//Resource file for Andrew Burt.
#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
using namespace std;
void andrew_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    ggprint8b(&r, 16, 0x00ff0000, "Andrew Burt");
}
void andrew_credits_picture(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=40;
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
