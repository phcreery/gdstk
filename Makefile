#  Makefile template for Static library. 
# 1. Compile every *.cpp in the folder 
# 2. All obj files under obj folder
# 3. static library .a at lib folder
# 4. run 'make dirmake' before calling 'make'

CC = g++
OUT_FILE_NAME = gdstk.a

CFLAGS= -Wall -Wextra -Wshadow -Wvla -Wformat -Wno-missing-field-initializers -Wno-missing-braces

INC = -I./include -I./external -I./external/qhull/src

SRC_DIR=./src
OBJ_DIR=./obj
OUT_DIR=./lib

#SRCS := $(wildcard src/*.cpp)
SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(addsuffix .o,$(basename $(SRCS))))

# Enumerating of every *.cpp as *.o and using that as dependency.	
# filter list of .c files in a directory.
# FILES =dump_l.c \
#	kter.c \
#
# $(OUT_FILE_NAME): $(patsubst %.c,$(OBJ_DIR)/%.o,$(wildcard $(FILES))) 


# Enumerating of every *.cpp as *.o and using that as dependency
# $(OUT_FILE_NAME): $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard *.cpp))
$(OUT_FILE_NAME): $(OBJS)
	ar -r -o $(OUT_DIR)/$@ $^

#Compiling every *.cpp to *.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp dirmake
	$(CC) -c $(INC) $(CFLAGS) -o $@  $<
	
dirmake:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OBJ_DIR)
	
clean:
	rm -f $(OBJ_DIR)/*.o $(OUT_DIR)/$(OUT_FILE_NAME) Makefile.bak

rebuild: clean build
