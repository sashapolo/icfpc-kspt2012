EXE = icfpc_solver
TESTEXE = tester
3DEXE = lifter3d
SHEXE = score_harness

SRC_DIR = src
HDR_DIR = hdr
OBJ_DIR = obj

TST_DIR = tests
TST_SRC_DIR = $(TST_DIR)/src
TST_OBJ_DIR = $(TST_DIR)/obj
TST_HDR_DIR = $(TST_DIR)/hdr

3D_DIR = 3D
3D_SRC_DIR = $(3D_DIR)/src
3D_OBJ_DIR = $(3D_DIR)/obj
3D_HDR_DIR = $(3D_DIR)/hdr

SH_DIR = scoreharness
SH_SRC_DIR = $(SH_DIR)/src
SH_OBJ_DIR = $(SH_DIR)/obj
SH_HDR_DIR = $(SH_DIR)/hdr

CC = g++
CFLAGS = -Wall -g

LD = g++
LFLAGS = -Wall
TEST_LIBS = -lcppunit
3D_LIBS = -lGL -lXxf86vm -lXext -lX11 -lrt

SRCS = $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TESTSRCS = $(foreach sdir, $(TST_SRC_DIR), $(wildcard $(sdir)/*.cpp))
TESTOBJS = $(patsubst $(TST_SRC_DIR)/%.cpp, $(TST_OBJ_DIR)/%.o, $(TESTSRCS))
SRCS_WITHOUT_MAIN = $(shell find $(SRC_DIR) -not -name main.cpp -type f)
OBJS_WITHOUT_MAIN = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_WITHOUT_MAIN))

3D_SRCS = $(foreach sdir, $(3D_SRC_DIR), $(wildcard $(sdir)/*.cpp))
3D_OBJS = $(patsubst $(3D_SRC_DIR)/%.cpp, $(3D_OBJ_DIR)/%.o, $(3D_SRCS))
3D_DEP_OBJS = $(OBJ_DIR)/base/Field.o $(OBJ_DIR)/base/Point.o\
			  $(OBJ_DIR)/base/FieldSim.o $(OBJ_DIR)/Logger.o \
			  $(OBJ_DIR)/base/Path.o

SH_SRCS = $(foreach sdir, $(SH_SRC_DIR), $(wildcard $(sdir)/*.cpp))
SH_OBJS = $(patsubst $(SH_SRC_DIR)/%.cpp, $(SH_OBJ_DIR)/%.o, $(SH_SRCS))
SH_DEP_OBJS = $(OBJ_DIR)/HTMLLogger.o $(OBJ_DIR)/Logger.o $(OBJ_DIR)/SignalHandler.o \
	      $(OBJ_DIR)/algo/AStar.o $(OBJ_DIR)/algo/AStarPoint.o \
	      $(OBJ_DIR)/algo/ManhattanHeuristic.o $(OBJ_DIR)/algo/NearestNeighbour.o \
	      $(OBJ_DIR)/algo/Solver.o $(OBJ_DIR)/algo/TwoOptOptimizer.o \
	      $(OBJ_DIR)/base/Field.o $(OBJ_DIR)/base/FieldSim.o \
	      $(OBJ_DIR)/base/Point.o $(OBJ_DIR)/base/Path.o
SH_REPORT_DIR = scoreharnessingreports

DOXYGENCFG = doc/doxygen/doxygen-config


all: create_object_dir create_test_object_dir create_3d_object_dir create_sh_object_dir $(TESTEXE) $(EXE) $(3DEXE) $(SHEXE) run_tests

create_object_dir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/base
	@mkdir -p $(OBJ_DIR)/algo

create_test_object_dir:
	@mkdir -p $(TST_OBJ_DIR)

create_3d_object_dir:
	@mkdir -p $(3D_OBJ_DIR)

create_sh_object_dir:
	@mkdir -p $(SH_OBJ_DIR)
	@mkdir -p $(TST_OBJ_DIR)

run_tests:
	@echo
	@echo "****************************************"
	@echo "            RUNNING TESTS"
	@echo "****************************************"
	@echo
	@./$(TESTEXE)
	@echo

solver: create_object_dir $(EXE)
tests: create_object_dir create_test_object_dir $(TESTEXE)
3dsolver: create_object_dir create_3d_object_dir $(3DEXE)
scoreharness: create_object_dir create_sh_object_dir $(SHEXE)


# Main executable
$(EXE): $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -I $(HDR_DIR) $(CFLAGS) -c $< -o $@


# Executable to run the tests
$(TESTEXE): $(TESTOBJS) $(OBJS_WITHOUT_MAIN)
	$(LD) $(LFLAGS) $^ -o $@ $(TEST_LIBS)

$(TST_OBJ_DIR)/%.o: $(TST_SRC_DIR)/%.cpp
	$(CC) -I $(TST_HDR_DIR) -I $(HDR_DIR) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.cpp
	$(CC) -I $(HDR_DIR) $(CFLAGS) -c $< -o $@


# 3D launcher
$(3DEXE): $(3D_OBJS) $(3D_DEP_OBJS)
	$(LD) $(LFLAGS) $^ -o $@ -L/usr/local/lib -lIrrlicht $(3D_LIBS)

$(3D_OBJ_DIR)/%.o: $(3D_SRC_DIR)/%.cpp
	$(CC) -I $(3D_HDR_DIR) -I $(HDR_DIR) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -I $(HDR_DIR) $(CFLAGS) -c $< -o $@

# Score Harnessing
$(SHEXE): $(SH_OBJS) $(SH_DEP_OBJS)
	@mkdir -p $(SH_REPORT_DIR)
	$(LD) $(LFLAGS) $^ -o $@ -L/usr/local/lib

$(SH_OBJ_DIR)/%.o: $(SH_SRC_DIR)/%.cpp
	$(CC) -I $(SH_HDR_DIR) -I $(HDR_DIR) $(CFLAGS) -std=c++11 -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -I $(HDR_DIR) $(CFLAGS) -c $< -o $@

# Generate documentation
doxygen:
	doxygen $(DOXYGENCFG)

# Clean the project
clean:
	@rm -rf $(OBJ_DIR) $(3D_OBJ_DIR) $(TST_OBJ_DIR) $(SH_OBJ_DIR) $(SH_REPORT_DIR)\
	        $(EXE) $(TESTEXE) $(3DEXE) $(SHEXE) 2>/dev/null

