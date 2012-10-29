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
SRCS_WITHOUT_MAIN = $(shell find src -not -name main.cpp -type f)
OBJS_WITHOUT_MAIN = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_WITHOUT_MAIN)) 

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
$(TESTEXE): $(TESTOBJS) $(OBJS_WITHOUT_MAIN)
	$(LD) $(LFLAGS) $(LIBS) $^ -o $(TESTEXE)


$(OBJ_DIR)/%.o: $(TST_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Generate documentation
doxygen:
	doxygen $(DOXYGENCFG)
	
# Clean the project
clean:
	@rm -rf $(OBJ_DIR) $(EXE) $(TESTEXE) 2>/dev/null
