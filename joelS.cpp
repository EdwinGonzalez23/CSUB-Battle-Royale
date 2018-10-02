//joelS.cpp
//Author: Joel Staggs

#include <iostream>
#include <GL/glx.h>
#include "fonts.h"


//Function required for lab 5
void joel_credits(int x, int y){
        Rect r;
        r.bot = y;
        r.left = x;

        ggprint8b(&r, 16, 0x00ff0000, "Joel Staggs");
}

void joel_picture(int x, int y, GLuint texture){


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
