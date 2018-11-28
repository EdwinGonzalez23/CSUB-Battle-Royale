#
# Makefile for  csub BATTLE ROYALE game
#
# Enter one of the following
#
# make
# make all
# make csub
# make clean
#
CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm
LFLAGS = -lrt -lX11 -lGL

all: csub

csub: csub.cpp log.cpp timers.cpp arturoU.cpp edwinG.cpp andrewB.cpp joelS.cpp bryanS.cpp
	g++ $(CFLAGS) csub.cpp log.cpp timers.cpp arturoU.cpp edwinG.cpp andrewB.cpp joelS.cpp bryanS.cpp \
	libggfonts.a -Wall $(LFLAGS) -lopenal -lalut -lpthread -o csub

clean:
	rm -f csub
	rm -f *.o

