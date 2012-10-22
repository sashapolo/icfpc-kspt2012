EXE = icfpc_solver
TESTEXE = tester

SRC_DIR = src
HDR_DIR = hdr
OBJ_DIR = obj
TST_DIR = tests

CC = g++
CFLAGS = -I $(HDR_DIR) -Wall -g

LD = g++
LFLAGS = -Wall
LIBS = -lcppunit

SRCS = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.cpp))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS)) 
TESTSRCS = $(foreach sdir, $(TST_DIR), $(wildcard $(sdir)/*.cpp))
TESTOBJS = $(patsubst $(TST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TESTSRCS)) 

DOXYGENCFG = doc/doxygen/doxygen-config


all: create_object_dir $(EXE) $(TESTEXE) 

create_object_dir:
	@mkdir -p $(OBJ_DIR)

# Main executable
$(EXE): $(OBJS)
	$(LD) $(LFLAGS) $^ -o $(EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Executable to run the tests
$(TESTEXE): $(TESTOBJS)
	$(LD) $(LFLAGS) $(LIBS) $^ -o $(TESTEXE)

$(OBJ_DIR)/%.o: $(TST_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Generate documentation
doxygen:
	doxygen $(DOXYGENCFG)
	
# Clean the project
clean:
	@rm -rf $(OBJ_DIR)
	@rm $(EXE)
	@rm $(TESTEXE) 
