//joelS.cpp
//Author: Joel Staggs

#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
//Audio library
//#include <AL/alut.h>
#include <unistd.h>

/*
###READ THIS###
These audio functions work on my personal computer
but I'd like to make sure everyone in the group
can run them before further implementation/uncommenting.
This code is being tested on another branch.
I also use several variables for sources instead of
using an array as it makes the function calls easier
to read (play_sound(bulletSound) vs. play_sound(src[i])).
 */

//Declare these globally.
/*
ALuint bulletSound;
ALuint impactSound;
ALuint bgm;
ALuint buffers[3];
*/

//Declare this in main(), easy to implement but cluttered.
/*
alutInit (NULL, NULL);
buffers[0] = alutCreateBufferFromFile ("./audio/gunshot.wav");
buffers[1] = alutCreateBufferFromFile ("./audio/impact.wav");
buffers[2] = alutCreateBufferFromFile ("./audio/RaveOnRevised.wav");
alGenSources (1, &impactSound);
alGenSources (1, &bulletSound);
alGenSources (1,&bgm);
alSourcei (bulletSound, AL_BUFFER, buffers[0]);
alSourcei (impactSound, AL_BUFFER, buffers[1]);
alSourcei (bgm, AL_BUFFER, buffers[2]);
thread tbgm(playBGM,bgm);
tbgm.detach();
*/
/*
OR use this cleaner solution,
it requires adding the ALuints into Global gl
but results in a much more organized main.
Put this in csub.cpp and run it in main().
void setup_sound(Global &gl)
{
        alutInit (NULL, NULL);
        gl.buffers[0] = alutCreateBufferFromFile ("./audio/gunshot.wav");
        alGenSources (1, &gl.bulletSound);
        alSourcei (gl.bulletSound, AL_BUFFER, gl.buffers[0]);
}

*/
/*
void play_sound(ALuint src)
{
        alSourcePlay (src);
        sleep (1);
}

void play_BGM(ALuint bgmSrc)
{
        while(1){
                alSourcePlay(bgmSrc);
                sleep(268);
        }
}
*/
//Put these in global scope with other prototypes.
//extern void play_BGM(ALuint bgmSrc);
//extern void play_sound(ALuint src);



//Function required for lab 5
void joel_credits(int x, int y)
{
        Rect r;
        r.bot = y;
        r.left = x;
        ggprint8b(&r, 16, 0x00ff0000, "Joel Staggs");
}

void joel_picture(int x, int y, GLuint texture)
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
