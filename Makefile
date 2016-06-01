#Makefile

CC=g++
LDFLAGS=-l GL -l GLU -l glut
CFLAGS=-g

.PHONY: clean

all: collision_detection

collision_detection: collision_detection.o concrete.o
	$(CC) $(CFLAGS) -o $@ mesh.o $< $(LDFLAGS)
 
collision_detection.o: mesh.o mesh.h

clean:
	rm -fr *.o collision_detection
