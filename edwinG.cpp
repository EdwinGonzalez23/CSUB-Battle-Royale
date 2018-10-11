/* Edwin Gonzalez
 * Special Functions to interact with main project
 *
 */

#include <iostream>
#include "fonts.h"
void edwin_credits(int x, int y){
	Rect r;
    r.bot = y;
	r.left = x;
	ggprint8b(&r, 16, 0x00ff0000, "Edwin Gonzalez");


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
