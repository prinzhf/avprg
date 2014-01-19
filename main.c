#include <stdio.h>
//#include <SDL/SDL.h>
//#include "include/SDL.h"
//#include <SDL/SDL_ttf.h>
#include <mainWindow.h>
#include <fileHandler.h>
#include <audioProcessor.h>

void getFileName(SDL_Event event, char* phat, char* desc, char* extension1, char* extension2, MAINWINDOW* window) {
	int file_given = 0;
	int i = 0;
	char c;
	SDL_EnableUNICODE(1);
	while(file_given == 0) {
		while(SDL_PollEvent(&event)){
			switch(event.type) {
				case SDL_QUIT:
					file_given = 1;
				case SDL_KEYDOWN:
					case SDL_KEYUP:
						if(event.key.keysym.sym == SDLK_RETURN) {
							strcpy(phat, desc);
							strcat(desc, extension1);
							strcat(phat, extension2);
							undisplayInput(desc, window);
							file_given = 1;
							break;	
						} else if(event.key.keysym.sym == SDLK_ESCAPE) {
							file_given = 1;
							break;
						} else {
							c = event.key.keysym.unicode;
							strcat(desc, &c);
							displayInput(desc, window);
							printf("%s %d\n", desc, i);
							break;
						}
			}
		}
	}
	SDL_EnableUNICODE(SDL_DISABLE);
}

int main(int argc, char* argv[]) {
	
	char desc[128] = "projects/";
	char phat[128];
	char* extension1 = ".desc";
	char* extension2 = ".phat";
	char tempo[4];
	int tempSec = 200;
	SDL_TimerID timer;

	int i, j, k, running = 1;
	FILE* phatFile = NULL;
	FILE* descFile = NULL;
	SDL_Event event;
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
		.green = {0, 255, 0},
		.text_color = {255, 255, 255},
		.textDst2 = {0, 562.5, 0, 0},
		.textDstOld = {0, 562.5, 0, 0},
		.mover = {10, 30, 20, 40},
		.prevMover = {10, 30, 20, 40},
		.channel1Visible = NULL,
		.channel2Visible = NULL,
		.channel3Visible = NULL,
		.channel4Visible = NULL,
		.songInvisible = NULL,
		.channel1Pos = {10, 50, 200, 550},
		.channel2Pos = {210, 50, 400, 550},
		.channel3Pos = {410, 50, 600, 550},
		.channel4Pos = {610, 50, 800, 550},
		.songPosOld = {50, 300, 750, 20},
		.file = NULL,
		.tempo = NULL
	};

	//SDL_TimerID timer;
	
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
	
	window = createWindow(window);
	window.textDst2.x = 0;
	
	//displayInput(desc, window);
	getFileName(event, phat, desc, extension1, extension2, &window);
	
	descFile = fopen(desc, "r");
	phatFile = fopen(phat, "r");
	if(!(descFile) || !(phatFile)) {
		printf("file error!\n");
		printf("%s\n", desc);
		printf("%s\n", phat);
		return 1;
	}
	
	chan = getChannelData(descFile, phatFile);
	if(chan == NULL) {
		printf("coudn't extract channel-data\n");
		return 1;
	}
	
	fclose(descFile);
	fclose(phatFile);
	
	setPhatText(&window, chan);
	setDescription(&window, chan, phat);
	
	setupAudio(chan);
	
	timer = SDL_AddTimer(tempSec, timerCallback, NULL);
	
	//main loop
	while(running) {
		if(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				break;
		}
		switch(event.type) {
			//case SDL_KEYDOWN:
				case SDL_KEYUP: 
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_o:
							break;
						case SDLK_t:
							//getTempo();
							break;
						case SDLK_s:
							//createTimer();
							break;
						case SDLK_x:
							//SDL_RemoveTimer(timer);
						default:
							break;
					}
				
		}
		moveDescription(&window);
		//moveSong(window);
		
		SDL_Delay(1);
	}
	
	SDL_CloseAudio();
	SDL_Quit();
	TTF_Quit();
	
	return 0;
}
