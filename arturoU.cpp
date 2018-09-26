//Arturo Urbina
//date:September 25, 2018
//defined variables and functions to toggle Credits
#include <iostream>
#include "fonts.h"
static int creditsState;
int getCreditState()
{
	return creditsState;
}
void toggleCredits()
{
	creditsState^=1;
}
void  art_credits(int x, int y)
{
	Rect r;
	r.bot =y;
	r.left=x;
	ggprint8b(&r,16,0x00FFFF00,"Arturo Urbina");
}
