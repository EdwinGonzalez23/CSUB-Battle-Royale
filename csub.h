/*
 * =====================================================================================
 *
 *       Filename:  csub.h
 *
 *    Description:  ALL FUNCTIONS FROM CSUB.cpp to be used by others.
 *
 *        Version:  1.0
 *        Created:  10/28/2018 04:24:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arturo Urbina (), Arturo@Urbina.Email
 *   Organization:  CSUB
 *
 * =====================================================================================
 */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
//Audio library
#include <AL/alut.h>
#include <thread>
#include <vector>
#include "log.h"
#include "fonts.h"
class Global {
	public:
		GLuint artTexture;
		GLuint bryanTexture;
		GLuint joelTexture;
		GLuint andrewTexture;
		GLuint edwinTexture;
		GLuint rTexture;
		GLuint sTexture;
		GLuint mgTexture;
		GLuint bulletTexture;
		GLuint bgTexture;
		GLuint treeTexture;
		GLuint ydTexture;
		GLuint bhTexture;
		GLuint hpTexture;
		GLuint tileTexture;
		GLuint logoTexture;
		GLuint textTexture;
		GLuint goTexture;
		GLuint characterHandgun;
		GLuint characterRifle;
		GLuint characterShotgun;
		GLuint characterKnife;
		GLuint roadTexture;
		GLuint grassTexture;
		GLuint floorTexture;
		GLuint wallB;
		GLuint wallL;
		GLuint wallR;
		GLuint wallT;
		GLuint wallCorner;
		GLuint wallEmpty;
		GLuint rockTexture1;
		GLuint rockTexture2;
		GLuint bushTexture1;
		GLuint bushTexture2;
		//Store sound sources here.
		ALuint bulletSound;
		ALuint youDiedSound;
		ALuint playerHitSound;
		ALuint mgSound;
		ALuint sfSound;
		ALuint pSound;
		ALuint rSound;
		ALuint sSound;
		ALuint readySound;
		ALuint goSound;
		ALuint buffers[10];
		int xres, yres;
		int credits;
		char keys[65536];
		Global() {
			credits=0;
			xres = 1250;
			yres = 900;
			memset(keys, 0, 65536);
		}
}; //gl
