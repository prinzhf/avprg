#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fileHandler.h>

typedef struct mainWindow {
	TTF_Font *font;
	TTF_Font *font2;
	TTF_Font *font3;
	SDL_Surface *surface;
	SDL_Surface *authored;
	SDL_Surface *blackened;
	SDL_Color black;
	SDL_Rect textDst2;
	SDL_Rect textDstOld;
	SDL_Rect mover;
	SDL_Rect prevMover;
	SDL_Surface *songVisible;
	SDL_Surface *songInvisible;
	SDL_Rect songPos;
	SDL_Rect songPosOld;
} MAINWINDOW;

MAINWINDOW createWindow(MAINWINDOW window, CHANNEL* chan);
void moveDescription(MAINWINDOW* window);

#endif
