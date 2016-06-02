#Makefile

CC=g++
LDFLAGS=-l GL -l GLU -l glut
CFLAGS=-g

.PHONY: clean

all: collision_detection

collision_detection: collision_detection.o mesh.o concrete.o enemy.h
	#$(CC) $(CFLAGS) -o $@ $(LDFLAGS)
 
collision_detection.o: mesh.o

mesh.o: concrete.o

clean:
	rm -fr *.o collision_detection
