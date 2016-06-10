#Makefile

CC=g++
LDLIBS=-l GL -l GLU -l glut
CPPFLAGS=-g -Wall -Werror

.PHONY: clean

all: forgotten

forgotten: forgotten.o mesh.o concrete.o util.o game.o enemy.o hero.o loadppm.o boss.h grid.o
	$(CC) $(CPPFLAGS) -o forgotten forgotten.o mesh.o concrete.o util.o game.o enemy.o hero.o loadppm.o boss.h bullet.o grid.o $(LDLIBS)

collision_detection: collision_detection.o enemy.o concrete.o util.o
	$(CC) $(CPPFLAGS) -o collision_detection collision_detection.o mesh.o concrete.o util.o enemy.o $(LDLIBS)
	
rotation_test: rotation_test.o
	
rotation_test.o: rotation.hpp
	
collision_detection.o: collision_detection.cpp enemy.h util.h

forgotten.o: mesh.h enemy.h util.h game.h hero.h

loadppm.o: loadppm.cpp loadppm.h

game.o: game.h enemy.h hero.o util.o boss.h bullet.o

hero.o: hero.cpp hero.h bullet.o

mesh.o: mesh.h rotation.hpp grid.h

enemy.o : enemy.h enemy.cpp

concrete.o: concrete.h mesh.h mesh.o

util.o: util.h

bullet.o: bullet.h

grid.o: grid.h

clean:
	rm -fr *.o forgotten collision_detection rotation_test
