CC=gcc
TARGET_ARCH=
INCLUDES=-I/usr/local/include/chipmunk
CFLAGS=-g -Wall -pedantic -std=c99
LDLIBS=-lchipmunk -lSDLmain -lSDL
LOADLIBES=-L/usr/local/lib
EXEC_NAME=jeu
OBJ_FILES=SDL_Ex.o CreationEnv.o test-ligne.o 

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LOADLIBES) $(LDLIBS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@
