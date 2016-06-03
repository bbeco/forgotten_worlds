#Makefile

CC=g++
LDLIBS=-l GL -l GLU -l glut
CFLAGS=-g -Wall -Werror

.PHONY: clean

all: collision_detection

collision_detection: collision_detection.o mesh.o concrete.o enemy.h
 
collision_detection.o: mesh.h enemy.h

mesh.o: rotation.hpp

concrete.o: mesh.h

clean:
	rm -fr *.o collision_detection
