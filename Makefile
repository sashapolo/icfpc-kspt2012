LIBS=

CC=g++
CFLAGS=-I hdr -c

LD=g++
LFLAGS=-o

SRC=src/*.cpp
OBJ=*.o

all: icfpc_solver

icfpc_solver:
	$(CC) $(CFLAGS) $(SRC)
	$(LD) $(LFLAGS) "icfpc_solver" $(OBJ) $(LIBS)

clean:
	rm -rf *.o
