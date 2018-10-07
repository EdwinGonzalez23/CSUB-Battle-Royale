//joelS.cpp
//Author: Joel Staggs

#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
//Audio library
//#include <AL/alut.h>
#include <unistd.h>

/*
 *These audio functions work on my personal computer
 *But I'd like to make sure everyone can run them before uncommenting.
 *(Changes were made on older branch).
 * /

//Declare these globally.
/*
ALuint bulletSound;
ALuint impactSound;
ALuint bgm;
ALuint buffers[3];
*/

//Declare this in main()
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
void playSound(ALuint src){
        alSourcePlay (src);
        sleep (1);

}

void playBGM(ALuint bgmSrc){
        while(1){
                alSourcePlay(bgmSrc);
                sleep(268);
        }
}
*/
//Put these in global scope with other prototypes.
//extern void playBGM(ALuint bgmSrc);
//extern void playSound(ALuint src);



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
