LIBS=-lcppunit

CC=g++
CFLAGS=-I hdr/ -c

LD=g++
LFLAGS=-o

SRC=src/*.cpp
TSTSRC=tests/*.cpp
OBJ=*.o
OBJ_DIR=dist/

TGTOBJ=-o obj/icfpc_solver
TSTOBJ=-o obj/test_runner

TGTELF=-o dist/icfpc_solver
TSTELF=-o dist/test_runner

DOXYGENCFG=doc/doxygen/doxygen-config

all: icfpc_solver test_runner

icfpc_solver:
	$(CC) $(CFLAGS) $(SRC) 
	$(LD) $(LFLAGS) "icfpc_solver" $(OBJ) $(LIBS)

# Executable to run the tests
test_runner:
#	$(CC) $(CFLAGS) $(TSTOBJ) $(TST)
#	$(LD) $(TSTELF) $(TSTOBJ) $(LIBS)
	$(CC) $(LIBS) -I hdr $(TSTELF) $(TSTSRC)

# Generate documentation
doxygen:
	doxygen $(DOXYGENCFG)

clean:
	rm -rf *.o
	rm -rf icfpc_solver
