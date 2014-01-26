ifdef __MINGW32__
LIBS = lmingw32 -lSDL -lSDL_mixer -lSDL_ttf -lm
ALL=phalanx-tracker.exe
else
LIBS = -lSDL -lSDL_mixer -lSDL_ttf -lm
ALL=phalanx-tracker
endif

INCLUDES = /include/
CC=gcc
CFLAGS = -I.$(INCLUDES)

DEPS = fileHandler.h mainWindow.h audioProcessor.h
OBJS = main.o fileHandler.o mainWindow.o audioProcessor.o
PROJS= /projects/

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ALL): $(OBJS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(INCLUDES)*~ $(PROJS)*~ phalanx-tracker
