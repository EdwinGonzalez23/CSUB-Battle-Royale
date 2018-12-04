//Resource file for Andrew Burt.
//This file will include the functions to display graphics to the program.

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include "game3.h"
#include <cstdlib>
Game g3;

using namespace std;

int Rocks [][2] = {
                639, 380,
                856, 352,
                786, 912,
                660, 1115,
                359, 1248,
                240, 772,
                191, 352,
                366, 366,
                366, 16,
                716, 35,
                1290, 28,
                2067, 0,
                2711, 28,
                3278, 56,
                3073, 483,
                2849, 609,
                2709, 896,
                2422, 1001,
                2163, 707,
                1883, 476,
                1449, 882,
                1260, 1610,
                1162, 1925,
                1771, 1990,
                2205, 1955,
                2660, 1717,
                2982, 2025,
                };

int Bushes [][2] = {
				737, 366,
				394, 471,
				107, 786,
				240, 898,
				338, 996,
				380, 1283,
				149, 1521,
				240, 1619,
				863, 1745,
				688, 1598,
				884, 1934,
				1157, 1780,
				1304, 1556,
				1220, 1241,
				1199, 1031,
				1297, 821,
				1157, 597,
				1262, 401,
				1178, 380,
				1430, 303,
				1626, 422,
				1696, 695,
				1927, 583,
				2067, 401,
				2312, 359,
				2564, 499,
				2627, 961,
				2452, 1178,
				2795, 1304,
				3040, 1255,
				3369, 1178,
				3493, 695,
				3297, 632,
				2884, 51,
				2982, 93,
				2695, 91,
				2555, 0,
				2478, 63,
				2324, 21,
				2121, 84,
				1876, 0,
				1764, 56,
				1470, 84,
				1155, 35
				};

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
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    int w = 25; 
	glTranslatef(x, y, z);
	glRotatef(angle + 90, 0.0f, 0.0f, 1.0f);
	//+ 90 Accounts for starting angle facing wrong way.
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
    glBindTexture(GL_TEXTURE_2D, 0);
}

void enemy(int x, int y, int z, float angle, GLuint texid)
{
    glColor3f(1.0, 0.0, 0.0);
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
	glBindTexture(GL_TEXTURE_2D, 0);
}

void genRoadHorizontal(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=64;
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
void genRoadVertical(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=64;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texid);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-w,  -w);
    glTexCoord2f(0.0f, 0.0f); glVertex2f( -w,  w);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( w, w);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
    glEnd();
    glPopMatrix(); 
}

void genWall(int x, int y, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=32;
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
void genWallCorner(int x, int y, int angle, GLuint texid)
{
    glColor3ub(255, 255, 255);
    int w=32;
    glPushMatrix();
    glTranslatef(x, y, 0);
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


void genTreeTexture(GLuint texture, int x, int y, int angle)
{
	int w = 150;
	//int h = 150;
	glPushMatrix();
	glTranslatef(x,y,0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

}

void genBushTexture(int x, int y, GLuint texid)
{
    int w = 150;
	//int h = 150;
	glPushMatrix();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void genRockTexture(int x, int y, GLuint texid)
{
    int w = 32;
	//int h = 150;
	glPushMatrix();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void genRocks(GLuint &rockTexture1, GLuint &rockTexture2)
{
	for (int i = 0; i < 27; i++) {
		for (int j = 0; j < 2; j++){
			if (j == 0) {
				genRockTexture(Rocks[i][j], Rocks[i][j+1], rockTexture1);
			}
			else {
				genRockTexture(Rocks[i][j],Rocks[i][j+1], rockTexture2);
			}
		}
	}
}

void genHouse(GLuint &wallCorner, GLuint &wallT, GLuint &wallR, GLuint &wallL, GLuint &wallB, GLuint &wallEmpty, int num)
{
	if (num == 1) {

		genWall(1768, 576+412, wallEmpty);
		genWall(1704, 576+412, wallEmpty);
		genWall(1768, 640+412, wallEmpty);
		genWall(1704, 640+412, wallEmpty);
		genWall(1768, 704+412, wallEmpty);
		genWall(1704, 704+412, wallEmpty);
		genWall(1768, 768+412, wallEmpty);

		genWall(1832, 768+412, wallEmpty);
		genWall(1832, 640+412, wallEmpty);
		genWall(1832, 704+412, wallEmpty);
		genWall(1832, 576+412, wallEmpty);

		genWall(1704, 512+412, wallEmpty);
		genWall(1768, 512+412, wallEmpty);
		genWall(1832, 512+412, wallEmpty);

		genWallCorner(1640, 768+412, 0, wallCorner);
		genWall(1704, 768+412, wallT);
		genWallCorner(1832, 768+412, 270, wallCorner);
		genWall(1832, 704+412, wallR);
		genWall(1832, 640+412, wallR);
		genWall(1832, 576+412, wallR);
		genWallCorner(1832, 512+412, 180, wallCorner);
		genWall(1704, 512+412, wallB);
		genWall(1768, 512+412, wallB);
		genWallCorner(1640, 512+412, 90, wallCorner);
		genWall(1640, 704+412, wallL);
		genWall(1640, 640+412, wallL);
		genWall(1640, 576+412, wallL);

	}
	else if (num == 2) {
		genWall(645, 512, wallEmpty);
		genWall(704, 512, wallEmpty);
		genWall(768, 512, wallEmpty);
		genWall(645, 576, wallEmpty);
		genWall(704, 576, wallEmpty);
		genWall(704, 640, wallEmpty);
		genWall(704, 704, wallEmpty);
		genWall(768, 640, wallEmpty);
		genWall(768, 704, wallEmpty);
		genWall(768, 576, wallEmpty);
		genWall(832, 576, wallEmpty);
		genWall(832, 640, wallEmpty);
		genWall(832, 704, wallEmpty);

		genWallCorner(640, 768, 0, wallCorner);
		genWall(704, 768, wallT);
		genWall(768, 768, wallT);
		genWallCorner(832, 768, 270, wallCorner);
		genWall(832, 704, wallR);
		genWall(832, 640, wallR);
		
		genWallCorner(832, 512, 180, wallCorner);
		genWall(704, 512, wallB);
		genWall(768, 512, wallB);
		genWallCorner(640, 512, 90, wallCorner);
		genWall(640, 704, wallL);
		genWall(640, 640, wallL);
		genWall(640, 576, wallL);
		
	}
	else if (num == 3) {

		genWall(832-576, 576+468, wallEmpty);
		genWall(832-576, 640+468, wallEmpty);
		genWall(832-576, 704+468, wallEmpty);
		genWall(768-576, 512+468, wallEmpty);
		genWall(704-576, 512+468, wallEmpty);
		genWall(768-576, 640+468, wallEmpty);
		genWall(768-576, 640+468, wallEmpty);
		genWall(704-576, 640+468, wallEmpty);
		genWall(704-576, 576+468, wallEmpty);
        genWall(645-576, 576+468, wallEmpty);
        genWall(645-576, 576+468, wallEmpty);
        genWall(768-576, 576+468, wallEmpty);
		genWall(768-576, 704+468, wallEmpty);
		genWall(704-576, 704+468, wallEmpty);
        genWall(768-576, 704+468, wallEmpty);



		genWallCorner(640-576, 768+468, 0, wallCorner);
		genWall(704-576, 768+468, wallT);
		genWall(768-576, 768+468, wallT);
		genWallCorner(832-576, 768+468, 270, wallCorner);
		genWall(832-576, 704+468, wallR);
		genWall(832-576, 640+468, wallR);
		


		genWallCorner(832-576, 512+468, 180, wallCorner);
		genWall(704-576, 512+468, wallB);
		genWall(768-576, 512+468, wallB);
		genWallCorner(640-576, 512+468, 90, wallCorner);
		genWall(640-576, 704+468, wallL);
		genWall(640-576, 640+468, wallL);
		genWall(640-576, 576+468, wallL);



	}
}

void genTrees(GLuint &treeTexture)
{
	genTreeTexture(treeTexture,1318,1085, rand()%2);
	genTreeTexture(treeTexture,1444,532, rand()%2);
	genTreeTexture(treeTexture,1906,1701, rand()%2);
	genTreeTexture(treeTexture,1325,2107, rand()%2);
	genTreeTexture(treeTexture,72,569, rand()%2);
	genTreeTexture(treeTexture,191,-60, rand()%2);
	genTreeTexture(treeTexture,3110,-60, rand()%2);
	genTreeTexture(treeTexture,3250,1043, rand()%2);
	genTreeTexture(treeTexture,3026,1743, rand()%2);
	genTreeTexture(treeTexture,2501,2205, rand()%2);
	genTreeTexture(treeTexture,1577,2359, rand()%2);
}

void genRoads(GLuint &roadTexture)
{
	for (int i = -7; i < 35; i++) {
		genRoadHorizontal((i*128), 192, roadTexture);
	}
	for (int i = -7; i < 35; i++) {
		genRoadHorizontal((i*128), 1408, roadTexture);
	}
	for (int i = -7; i < 35; i++) {
		genRoadVertical(512, (i*128), roadTexture);
	}
	for (int i = -7; i < 35; i++) {
		genRoadVertical(1024, (i*128), roadTexture);
	}
	for (int i = -7; i < 35; i++) {
		genRoadVertical(512, (i*128), roadTexture);
	}
}

void genBushes(GLuint &bushTexture1, GLuint &bushTexture2)
{
	for (int i = 0; i < 27; i++) {
		for (int j = 0; j < 2; j++){
			if (j == 0) {
				genBushTexture(Bushes[i][j], Bushes[i][j+1], bushTexture1);
			}
			else {
				genBushTexture(Bushes[i][j],Bushes[i][j+1], bushTexture2);
			}
		}
	}
}
