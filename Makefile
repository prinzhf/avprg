INCLUDES = -I./include
LIBS = -lSDL -lSDL_ttf -lSDL_mixer -lm
PROGS = phalanx-tracker
CC = gcc
CFLAGS = -Wall -g -ansi -pedantic
 
all: $(PROGS)

phalanx-tracker: main.c mainWindow.c fileHandler.c audioProcessor.c
		$(CC) -o phalanx-tracker main.c mainWindow.c fileHandler.c audioProcessor.c $(INCLUDES) $(LIBS)
	
clean:
	rm -f $(PROGS)
	rm -f *.o
