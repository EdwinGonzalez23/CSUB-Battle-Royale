//defined types
#include "csub.h"
Global gl2;
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];
//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
							 (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float TIMESLICE = 1.0f;
const float GRAVITY = -0.6f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 1000;
const Flt MINIMUM_ASTEROID_SIZE = 10.0;
//-----------------------------------------------------------------------------
//Joel's stuff
#include <thread>
extern void play_sound(ALuint a);
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
class Image {
	public:
		int width, height;
		unsigned char *data;
		~Image() { delete [] data; }
		Image(const char *fname) {
			if (fname[0] == '\0')
				return;
			//printf("fname **%s**\n", fname);
			int ppmFlag = 0;
			char name[40];
			strcpy(name, fname);
			int slen = strlen(name);
			char ppmname[80];
			if (strncmp(name+(slen-4), ".ppm", 4) == 0)
				ppmFlag = 1;
			if (ppmFlag) {
				strcpy(ppmname, name);
			} else {
				name[slen-4] = '\0';
				//printf("name **%s**\n", name);
				sprintf(ppmname,"%s.ppm", name);
				//printf("ppmname **%s**\n", ppmname);
				char ts[100];
				//system("convert eball.jpg eball.ppm");
				sprintf(ts, "convert %s %s", fname, ppmname);
				system(ts);
			}
			//sprintf(ts, "%s", name);
			FILE *fpi = fopen(ppmname, "r");
			if (fpi) {
				char line[200];
				fgets(line, 200, fpi);
				fgets(line, 200, fpi);
				//skip comments and blank lines
				while (line[0] == '#' || strlen(line) < 2)
					fgets(line, 200, fpi);
				sscanf(line, "%i %i", &width, &height);
				fgets(line, 200, fpi);
				//get pixel data
				int n = width * height * 3;
				data = new unsigned char[n];
				for (int i=0; i<n; i++)
					data[i] = fgetc(fpi);
				fclose(fpi);
			} else {
				printf("ERROR opening image: %s\n",ppmname);
				exit(0);
			}
			if (!ppmFlag)
				unlink(ppmname);
		}
};
class Ship {
	public:
		Vec dir;
		Vec pos;
		Vec vel;
		float angle;
		float color[3];
	public:
		Ship() {
			VecZero(dir);
			pos[0] = (Flt)(gl2.xres/2);
			pos[1] = (Flt)(gl2.yres/2);
			pos[2] = 0.0f;
			VecZero(vel);
			angle = 0.0;
			color[0] = color[1] = color[2] = 1.0;
		}
};
class Bullet {
	public:
		Vec pos;
		Vec vel;
		float color[3];
		struct timespec time;
	public:
		Bullet() { }
};
class Asteroid {
	public:
		Vec pos;
		Vec vel;
		int health = 50;
		int maxHp = 50;
		int hpMissing = maxHp-health;
		bool invuln = 0;
		struct timespec invulnTimer;
		int nverts;
		int gunNum;
		Flt radius;
		Vec vert[8];
		float angle;
		float rotate;
		float color[3];
		struct Asteroid *prev;
		struct Asteroid *next;
	public:
		Asteroid() {
			prev = NULL;
			next = NULL;
		}
		void drawHealthBar(int x, int y){
			glColor3f(0,1,0.5);
			glPushMatrix();
			glTranslatef(0, 0, 0);
			glBegin(GL_QUAD_STRIP);
			glVertex2f(x,y);
			glVertex2f(x, y-10);
			glVertex2f(x+50, y);
			glVertex2f(x+50,y-10);
			glEnd();
			glPopMatrix();
			//Current MISSING player HP
			glColor3f(0.5f,0,0);
			glPushMatrix();
			glTranslatef(0, 0, 0);
			glBegin(GL_QUAD_STRIP);
			glVertex2f(x+50-hpMissing,y);
			glVertex2f(x+50-hpMissing, y-10);
			glVertex2f(x+50, y);
			glVertex2f(x+50,y-10);
			glEnd();
			glPopMatrix();
		}
		void checkInvuln(){
			struct timespec invulnComparison;
			clock_gettime(CLOCK_REALTIME, &invulnComparison);
			double timeDifference = timeDiff(&invulnTimer, &invulnComparison);
			//std::cout<<timeDifference<<std::endl;
			if(timeDifference>0.5){
				invuln=0;
			}
		}
};
class Game {
	public:
		int astr_destroyed;
		Ship ship;
		Asteroid *ahead;
		Bullet *barr;
		Bullet *barrAst;
		int nasteroids;
		int nbullets;
		int astBull;
		struct timespec bulletTimer;
		struct timespec enemyBulletTimer[3];
		struct timespec itemTimer;
		struct timespec mouseThrustTimer;
		bool mouseThrustOn;
	public:
		Game() {
			astr_destroyed=0;
			ahead = NULL;
			barr = new Bullet[MAX_BULLETS];
			barrAst = new Bullet[MAX_BULLETS];
			nasteroids = 0;
			nbullets = 0;
			astBull = 0;
			mouseThrustOn = false;
			//build 10 asteroids...
			for (int j=0; j<2; j++) { //Change num asteroid
				Asteroid *a = new Asteroid;
				a->nverts = 8;
				a->radius = rnd()*80.0 + 40.0;
				//a->radius=10.0;
				Flt r2 = a->radius / 2.0;
				Flt angle = 0.0f;
				Flt inc = (PI * 2.0) / (Flt)a->nverts;
				for (int i=0; i<a->nverts; i++) {
					a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
					a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
					angle += inc;
				}
				a->pos[0] = (Flt)(rand() % gl2.xres);
				a->pos[1] = (Flt)(rand() % gl2.yres);
				a->pos[2] = 0.0f;
				a->angle = 0.0;
				a->rotate = rnd() * 4.0 - 2.0;
				a->color[0] = 0.8;
				//a->color[1] = 0.8;
				a->color[1] = 0.8;
				a->color[2] = 0.7;
				a->vel[0] = (Flt)(rnd()*2.0-1.0);
				a->vel[1] = (Flt)(rnd()*2.0-1.0);
				//std::cout << "asteroid" << std::endl;
				//add to front of linked list
				a->next = ahead;
				if (ahead != NULL)
					ahead->prev = a;
				ahead = a;
				++nasteroids;
			}
			clock_gettime(CLOCK_REALTIME, &bulletTimer);
			clock_gettime(CLOCK_REALTIME, &enemyBulletTimer[3]);
		}
		~Game() {
			delete [] barr;
			delete [] barrAst;
		}
};
//
// static int colors = 255;
// int getColors(){
//         return colors;
// }
//
// static bool faded = 0;
// static bool fadeOutBegin = 0;
// extern void toggleMenu();
// bool menuFadedOut(){
//         return faded;
// }
//
// void menuSound(){
//         play_sound(gl2.mgSound);
//         sleep(1);
//         play_sound(gl2.sfSound);
// }
//
// void beginFade(){
//         fadeOutBegin=1;
// 	std::thread td(menuSound);
// 	td.detach();
// }
//
// void genBH(GLuint texture, int x, int y, int size){
//         int w = size;
//         //int h = 150;
//         glPushMatrix();
//         glTranslatef(x,y,0);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glEnable(GL_ALPHA_TEST);
//         glAlphaFunc(GL_GREATER, 0.0f);
//         glColor4ub(colors,colors,colors,255);
//         glBegin(GL_QUADS);
//         glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
//         glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
//         glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
//         glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
//         glEnd();
//         glPopMatrix();
//         glBindTexture(GL_TEXTURE_2D, 0);
//
// }
//
// bool fadeBegin(){
//         return fadeOutBegin;
// }
//
// void setColors(int x){
//         colors=x;
// }
// void genTitleScreen(GLuint texture,GLuint texture2, int x, int y){
//         int w = 800;
//         //int h = 150;
//         glPushMatrix();
//         glTranslatef(x,y+500,0);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glEnable(GL_ALPHA_TEST);
//         glAlphaFunc(GL_GREATER, 0.0f);
//         glColor4ub(colors,colors,colors,255);
//         glBegin(GL_QUADS);
//         glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
//         glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
//         glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
//         glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
//         glEnd();
//         glPopMatrix();
//         glBindTexture(GL_TEXTURE_2D, 0);
//
//
//         w = 350;
//         glPushMatrix();
//         glTranslatef(x,y,0);
//         glBindTexture(GL_TEXTURE_2D, texture2);
//         glEnable(GL_ALPHA_TEST);
//         glAlphaFunc(GL_GREATER, 0.0f);
//         glColor4ub(colors,colors,colors,255);
//         glBegin(GL_QUADS);
//         glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
//         glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w);
//         glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w);
//         glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
//         glEnd();
//         glPopMatrix();
//         glBindTexture(GL_TEXTURE_2D, 0);
//
//         if(fadeOutBegin==1&&colors>0){
//                 colors-=1;
// 		genBH(gl2.bhTexture, 1200,1200,350);
// 		genBH(gl2.bhTexture, 2000,1600,350);
// 		genBH(gl2.bhTexture, 1800,1900,350);
// 		genBH(gl2.bhTexture, 1300,2000,350);
// 		genBH(gl2.bhTexture, 1500,2500,350);
//         }
//         if(colors==0){
//                 faded =1;
//                 toggleMenu();
//                 colors=255;
//
//         }
// }
