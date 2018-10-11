/* Edwin Gonzalez
 * Personal Function Work
 * Image name : edwinImg.png (located in image folder)
 */
#include <iostream>
#include "fonts.h"
#include <GL/gl.h>
#include <GL/glu.h>
void edwin_credits(int x, int y)
{
	Rect r;
    	r.bot = y;
	r.left = x;
	ggprint8b(&r, 16, 0x00ff0000, "Edwin Gonzalez");
}

void edwin_picture(int x, int y, GLuint texid) 
{
    	//Show my image 
    	glColor3ub(255, 255, 255);
    	int w = 50;
    	glPushMatrix();
    	glTranslatef(x + 100, y, 0);
    	//glBindTexture(GL_TEXTURE_2D, gl.randomTexture);
    	glBindTexture(GL_TEXTURE_2D, texid);
    	glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
	glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
	glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);

    	glEnd();
    	glPopMatrix();
}
