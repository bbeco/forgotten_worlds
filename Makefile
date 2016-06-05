#Makefile

CC=g++
LDLIBS=-l GL -l GLU -l glut
CFLAGS=-g -Wall -Werror

.PHONY: clean

all: collision_detection

collision_detection: collision_detection.o mesh.o concrete.o util.o enemy.h
 
collision_detection.o: mesh.h enemy.h util.h

mesh.o: rotation.hpp

concrete.o: concrete.h mesh.h mesh.o

util.o: util.h

clean:
	rm -fr *.o collision_detection
