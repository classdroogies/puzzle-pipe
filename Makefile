DSRC = src/
DBIN = bin/

CC = gcc
CFLAGS = -Wall 
LDFLAGS =`sdl-config --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_mixer
BIN = puzzlepipe

SRC = $(wildcard $(DSRC)*.c)
OBJ = $(SRC:.c=.o)

all : $(BIN)

$(BIN) : $(OBJ)
	$(CC) -o $(DBIN)$(BIN) $(OBJ) $(LDFLAGS)

clean :
	rm -rf $(DSRC)*.o
	
clean-all : clean
	rm -rf $(DBIN)$(BIN)
