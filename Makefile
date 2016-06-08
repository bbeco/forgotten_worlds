#Makefile

CC=g++
LDLIBS=-l GL -l GLU -l glut
CPPFLAGS=-g -Wall -Werror

.PHONY: clean

all: forgotten

forgotten: forgotten.o mesh.o concrete.o util.o game.o enemy.h hero.o loadppm.o
	$(CC) $(CPPFLAGS) -o forgotten forgotten.o mesh.o concrete.o util.o game.o enemy.h hero.o loadppm.o $(LDLIBS)
forgotten.o: mesh.h enemy.h util.h game.h hero.h

loadppm.o: loadppm.cpp loadppm.h

game.o: game.h enemy.h hero.o util.o

hero.o: hero.cpp hero.h

mesh.o: rotation.hpp

concrete.o: concrete.h mesh.h mesh.o

util.o: util.h

clean:
	rm -fr *.o forgotten
