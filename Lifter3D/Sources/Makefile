EXE = lifter3d

SRC_DIR = src
HDR_DIR = hdr
OBJ_DIR = obj

CC = g++
CFLAGS = -I $(HDR_DIR) -I irrIncludes -Wall -g 

LD = g++
LFLAGS = -Wall -lGL -lXxf86vm -lXext -lX11

# Link Libraries and Options
LDLIBSOPTIONS=-Llib -lIrrlicht


SRCS = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.cpp))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS)) 
SRCS_WITHOUT_MAIN = $(shell find src -not -name main.cpp -type f)
OBJS_WITHOUT_MAIN = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_WITHOUT_MAIN)) 


all: create_object_dir $(EXE)

create_object_dir:
	@mkdir -p $(OBJ_DIR)

# Main executable
$(EXE): $(OBJS)
	$(LD) $(LFLAGS)  $^ -o $(EXE) ${LDLIBSOPTIONS}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@


	
# Clean the project
clean:
	@rm -rf $(OBJ_DIR) $(EXE) 2>/dev/null
