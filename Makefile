#Makefile

#OBJS specifies which files to compile as part of the project
OBJS = pictrl.c

#CC specifies which compiler we're using
CC = gcc

#Preprocessor flags
#CPPFLAGS = -D__LINUX_ALSA__

#COMPILER_FLAGS specifies the additional compilation options we're using
# -g debug, -Wall suppress warnings
COMPILER_FLAGS = -g -O0 -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lssd1306_i2c

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = pictrl

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

