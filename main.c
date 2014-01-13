#include <stdio.h>
//#include <SDL/SDL.h>
//#include "include/SDL.h"
//#include <SDL/SDL_ttf.h>
#include <mainWindow.h>
#include <fileHandler.h>
#include <audioProcessor.h>

int i, j, k, running = 1;
FILE* phatFile = NULL;
FILE* descFile = NULL;
SDL_Event event;
//MAINWINDOW* window;
CHANNEL* chan;

//SETUP MAINWINDOW
MAINWINDOW window = { 
	.font = NULL, 
	.font2 = NULL,
	.font3 = NULL,
	.surface = NULL,
	.authored = NULL,
	.blackened = NULL,
	.black = {0, 0, 0},
	.textDst2 = {0, 562.5, 0, 0},
	.textDstOld = {0, 562.5, 0, 0},
	.mover = {10, 30, 20, 40},
	.prevMover = {10, 30, 20, 40},
	.songVisible = NULL,
	.songInvisible = NULL,
	.songPos = {0, 300, 800, 20},
	.songPosOld = {50, 300, 750, 20}
};

int main(int argc, char* argv[]) {
	SDL_TimerID timer;
	
	/*if(window)
		window = (MAINWINDOW) malloc(sizeof(MAINWINDOW));
	*/
	//not needed
	
	//initiate SDL_VIDEO, SDL_TTF (fonts) & SDL_TIMER
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL init error\n");
		return 1;
	}
	if(TTF_Init() == -1) {
   		printf("TTF_Init: %s\n", TTF_GetError());
    	return 1;
	}
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("SDL timer init error\n");
		return 1;
	}
	
	
	descFile = fopen("projects/glutchie.desc", "r");
	phatFile = fopen("projects/glutchie.phat", "r");
	
	chan = getChannelData(descFile, phatFile);
	fclose(descFile);
	fclose(phatFile);
	
	window = createWindow(window, chan);
	window.textDst2.x = 0;
	
	timer = SDL_AddTimer(200, timerCallback, NULL);
	
	setPhatText(window, chan);
	
	setupAudio(chan);
	
	//main loop
	while(running) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				break;
		}
		else if(SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = 0;
					break;
				case SDLK_o:
					printf("o pressed\n");
					break;
				default:
					break;
			}
		}
		moveDescription(&window);
		//moveSong(window);
		SDL_Delay(1);
	}
	
	SDL_Quit();
	TTF_Quit();
	
	return 0;
}
