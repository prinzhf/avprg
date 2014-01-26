#include <stdio.h>
#include <mainWindow.h>
#include <fileHandler.h>
#include <audioProcessor.h>

void getFileName(SDL_Event event, CHANNEL* chan, MAINWINDOW* window, char* filename);
void addTimer(SDL_TimerID timer, int tempo);
int setTempo(SDL_Event event, MAINWINDOW* window);
void setFiles(char* phat, CHANNEL* chan, MAINWINDOW* window);

int main(int argc, char* argv[]) {
	
	FILE* fp;
	CHANNEL* chan;
	MAINWINDOW* window;
	SDL_Event event;
	SDL_TimerID timer;
	char filename[128];
	char tempo[4];
	int tempSec = 200;
	int songRun = 0;
	int i, j, k, running = 1;
	
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
	
	//SETUP MAINWINDOW
	window = createWindow(window);
	window->textDst2.x = 0;
	
	SDL_EnableKeyRepeat(0, 0);
	
	//main loop
	while(running) {
		if(SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					case SDL_KEYUP: 
						switch(event.key.keysym.sym) {
							case SDLK_ESCAPE:
								running = 0;
								break;
							case SDLK_o:
								songRun = 0;
								resetChannels();
								SDL_RemoveTimer(timer);
								clearBottom(window);
								clearCenter(window);
								getFileName(event, chan, window, filename);
								break;
							case SDLK_t:
								songRun = 0;
								resetChannels();
								SDL_RemoveTimer(timer);
								tempSec = setTempo(event, window);
								break;
							case SDLK_s:
								if(filename != NULL && songRun != 1) {
									songRun = 1;
									clearBottom(window);
									FILE* fp = fopen(filename, "r");
									chan = getChannelData(fp);
									fclose(fp);
									setupAudio(chan, window);
									timer = SDL_AddTimer(tempSec, timerCallback, NULL);
								}
								break;
							case SDLK_p:
								songRun = pauseChannels(timer, songRun, tempSec);
								break;
							case SDLK_r:
								resetChannels();
							default:
								break;
						}
			}
		}
		moveDescription(window);
		//moveSong(window);
		
		SDL_Delay(1);
	}
	SDL_FreeSurface(window->surface);
	SDL_FreeSurface(window->blackened);
	SDL_FreeSurface(window->authored);
	free(window);
	free(chan);
	SDL_CloseAudio();
	SDL_Quit();
	TTF_Quit();
	
	return 0;
}

void setFiles(char* phat, CHANNEL* chan, MAINWINDOW* window) {
	FILE* phatFile = NULL;
	phatFile = fopen(phat, "r");
	if(!(phatFile)) {
		phat = NULL;
		printf("file error!\n");
		printf("try again\n");
	} else {
		chan = getChannelData(phatFile);
		if(chan == NULL) {
			printf("coudn't extract channel-data\n");
			exit;
		}
		fclose(phatFile);
		setPhatText(window, chan);
		//setDescription(window, chan, phat);
		//setupAudio(chan);
	}
}

void getFileName(SDL_Event event, CHANNEL* chan, MAINWINDOW* window, char* filename) {
	int file_given = 0;
	char c;
	char phatPath[128] = "projects/";
	char phat[128] = "";
	char phatClear[128] = "";
	char* extension = ".phat";
	CHANNEL* channel;
	SDL_EnableUNICODE(1);
	//displayInput("", window);
	while(file_given == 0) {
		while(SDL_PollEvent(&event)){
			switch(event.type) {
				case SDL_QUIT:
					file_given = 1;
					break;
				case SDL_KEYDOWN:
					case SDL_KEYUP:
						if(event.key.keysym.sym == SDLK_RETURN) {
							strcpy(phat, phatPath);
							strcat(phat, extension);
							clearBottom(window);
							setFiles(phat, channel, window);
							strcpy(filename, phat);
							file_given = 1;
							break;	
						} else if(event.key.keysym.sym == SDLK_ESCAPE) {
							file_given = 1;
							break;
						} else {
							c = event.key.keysym.unicode;
							strcat(phatPath, &c);
							strcat(phatClear, &c);
							displayInput(phatClear, window);
							setDescription(window, chan, phatClear);
							break;
						}
			}
		}
	}
	SDL_EnableUNICODE(SDL_DISABLE);
}

int setTempo(SDL_Event event, MAINWINDOW* window) {
	char c;
	int tempo_given = 0;
	int tempo = 0;
	char ts[4];
	char tempoText[16] = "Set Tempo: ";
	SDL_EnableUNICODE(1);
	displayInput(tempoText, window);
	while(tempo_given == 0) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					tempo_given = 1;
					break;
				case SDL_KEYDOWN:
					case SDL_KEYUP:
						if(event.key.keysym.sym ==SDLK_RETURN) {
							tempo_given = 1;
							break;
						} else if(event.key.keysym.sym == SDLK_ESCAPE){
							tempo_given = 1;
							break;
						} else {
							c = event.key.keysym.unicode;
							displayInput(ts, window);
							strcat(ts, &c);
							break;
						}
				default: break;
			}
		}
	}
	SDL_EnableUNICODE(SDL_DISABLE);
	tempo = atoi(ts);
	printf("Tempo: %d\n", tempo);
	clearBottom(window);
	return tempo;
}
