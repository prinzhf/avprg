#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fileHandler.h>

#define WHITE (SDL_Color){255, 255, 255}
#define BLACK (SDL_Color){0, 0, 0}
#define GREEN (SDL_Color){0, 255, 0}
#define WIDTH (800)
#define HEIGHT (600)

typedef struct mainWindow {
	TTF_Font *font;
	TTF_Font *font2;
	TTF_Font *font3;
	SDL_Surface *surface;
	SDL_Surface *authored;
	SDL_Surface *blackened;
	SDL_Color black;
	SDL_Color green;
	SDL_Color text_color;
	SDL_Rect textDst2;
	SDL_Rect textDstOld;
	SDL_Rect mover;
	SDL_Rect prevMover;
	SDL_Surface *channel1Visible;
	SDL_Surface *channel2Visible;
	SDL_Surface *channel3Visible;
	SDL_Surface *channel4Visible;
	SDL_Surface *songInvisible;
	SDL_Rect channel1Pos;
	SDL_Rect channel2Pos;
	SDL_Rect channel3Pos;
	SDL_Rect channel4Pos;
	SDL_Rect songPosOld;
	SDL_Surface *file;
	SDL_Surface *tempo;
	SDL_Surface* chanDisplay[16];
	SDL_Rect* displayPos[16];
} MAINWINDOW;

MAINWINDOW* createWindow(MAINWINDOW* window);
void moveDescription(MAINWINDOW* window);
void setPhatText(MAINWINDOW* window, CHANNEL* chan);
void clearBottom(MAINWINDOW* window);
void clearCenter(MAINWINDOW* window);

#endif
