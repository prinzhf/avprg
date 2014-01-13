#include <stdio.h>
#include <mainWindow.h>

int i;
FILE* fp;
SDL_Color text_color = {255, 255, 255};
int mRight = 1;

MAINWINDOW createWindow(MAINWINDOW window, CHANNEL* chan) {
	
	window.surface = SDL_SetVideoMode(800, 600, 16, SDL_SWSURFACE);
	if(window.surface == NULL) {
		printf("surface error");
	}
	
	MAINWINDOW *window_ptr;
	fp = fopen("projects/chan2.desc", "r");
	if(window_ptr != NULL)
		window_ptr = (MAINWINDOW *) malloc(sizeof(MAINWINDOW));
	
	//TITLE
	SDL_WM_SetCaption("Phalanx-Tracker", "Phalanx-Tracker");
	
	//WHITE LINES
	SDL_Rect rectTopLine = {0, 45, 800, 5};
	SDL_FillRect(window.surface, &rectTopLine, 0xFFFFFF);
	SDL_Rect rectBottomLine = {0, 555, 800, 5};
	SDL_FillRect(window.surface, &rectBottomLine, 0xFFFFFF);
	
	//TEXT
	window.font = TTF_OpenFont("fonts/AEH.TTF", 24);
	if(window.font == NULL) {
		printf("error in font\n");
	}
	window.font2 = TTF_OpenFont("fonts/AEH.TTF", 14);
	if(window.font2 == NULL) {
		printf("error in font\n");
	}
	window.font3 = TTF_OpenFont("fonts/AEH.TTF", 18);
	if(window.font3 == NULL) {
		printf("error in font\n");
	}
	
	//HEADLINE
	SDL_Surface *phalanx = TTF_RenderText_Solid(window.font, "phalanx-tracker", text_color);
	
	//SET THE MOVING DESCRIPTION	
	window.authored = TTF_RenderText_Solid(window.font, chan->description, text_color);
	window.blackened = TTF_RenderText_Solid(window.font, chan->description, window.black);
	SDL_Rect textDst = {300, 7.5, 0, 0};
	
	SDL_BlitSurface(phalanx, NULL, window.surface, &textDst);
	SDL_BlitSurface(window.authored, NULL, window.surface, &window.textDst2);
	
	SDL_BlitSurface(window.songVisible, NULL, window.surface, &window.songPos);
	
	return window;
}

void moveDescription(MAINWINDOW* window) {	
	SDL_BlitSurface(window->blackened, NULL, window->surface, &window->textDstOld);
	window->textDstOld.x = window->textDst2.x;
	
	if(window->textDst2.x < 800) {
		window->textDst2.x += 1;
	} else {
		window->textDst2.x = 0;
	}
	SDL_BlitSurface(window->authored, NULL, window->surface, &window->textDst2);
	SDL_Flip(window->surface);
}

void setPhatText(MAINWINDOW* window, CHANNEL* chan) {
	char phatLine[128];
	char currentString[10];
	//strcat(currentString, (const char*) chan->chanval[0][0].amp);
}

void moveSong(MAINWINDOW* window, CHANNEL* chan) {
	SDL_BlitSurface(window->songInvisible, NULL, window->surface, &window->songPosOld);
	window->songPosOld.y = window->songPos.y;
	if(window->songPos.y < 48) {
		window->songPos.y += 10;
		SDL_Delay(200);
	} else {
		window->songPos.y += 10;
		SDL_Delay(100);
	}
	
	SDL_BlitSurface(window->songVisible, NULL, window->surface, &window->songPos);
	SDL_Flip(window->surface);
}
