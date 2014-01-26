#include <stdio.h>
#include <mainWindow.h>

MAINWINDOW* createWindow(MAINWINDOW* window) {
	int i;
	window = (MAINWINDOW*) malloc(sizeof(MAINWINDOW));
	window->font = NULL;
	window->font2 = NULL;
	window->surface = NULL;
	window->authored = NULL;
	window->blackened = NULL;
	window->text_color = WHITE;
	window->black = BLACK;
	window->green = GREEN;
	window->textDst2 = (SDL_Rect) {0, 565, 800, 50};
	window->textDstOld = (SDL_Rect) {0, 565, 0, 0};
	window->mover = (SDL_Rect) {10, 30, 20, 40};
	window->prevMover = (SDL_Rect) {10, 30, 20, 40};
	for(i = 0; i < 16; i++) {
		window->chanDisplay[i] = (SDL_Surface*) malloc(sizeof(SDL_Surface));
	}
	
	window->surface = SDL_SetVideoMode(800, 600, 16, SDL_SWSURFACE);
	if(window->surface == NULL) {
		printf("surface error");
	}
	
	//TITLE
	SDL_WM_SetCaption("Phalanx-Tracker", "Phalanx-Tracker");
	
	//WHITE LINES
	SDL_Rect rectTopLine = {0, 50, 800, 5};
	SDL_FillRect(window->surface, &rectTopLine, 0xFFFFFF);
	SDL_Rect rectBottomLine = {0, 550, 800, 5};
	SDL_FillRect(window->surface, &rectBottomLine, 0xFFFFFF);
	
	//FONTS
	window->font = TTF_OpenFont("fonts/slickerregular.ttf", 24);
	if(window->font == NULL) {
		printf("error in font\n");
	}
	window->font2 = TTF_OpenFont("fonts/slickerregular.ttf", 12);
	if(window->font2 == NULL) {
		printf("error in font\n");
	}
	window->font3 = TTF_OpenFont("fonts/slickerregular.ttf", 18);
	if(window->font3 == NULL) {
		printf("error in font\n");
	}
	
	//HEADLINE
	SDL_Surface *phalanx = TTF_RenderText_Blended(window->font, "phalanx-tracker", window->text_color);
	
	//INFO TEXT
	window->file = TTF_RenderText_Blended(window->font2, "Press 's' to start song", window->text_color);
	SDL_Rect textDst3 = {0, 0, 100, 15};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst3);
	window->file = TTF_RenderText_Blended(window->font2, "Press 'p' to pause/unpause song", window->text_color);
	SDL_Rect textDst4 = {0, 15, 100, 30};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst4);
	window->file = TTF_RenderText_Blended(window->font2, "Press 'r' to reset song", window->text_color);
	SDL_Rect textDst5 = {0, 30, 100, 45};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst5);
	window->file = TTF_RenderText_Blended(window->font2, "Press 'o' to open song", window->text_color);
	SDL_Rect textDst6 = {650, 0, 800, 15};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst6);
	window->file = TTF_RenderText_Blended(window->font2, "Press 't' to set tempo", window->text_color);
	SDL_Rect textDst7 = {650, 15, 800, 30};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst7);
	window->file = TTF_RenderText_Blended(window->font2, "Press 'esc' to exit", window->text_color);
	SDL_Rect textDst8 = {650, 30, 800, 45};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst8);
	
	SDL_Rect textDst = {300, 7.5, 0, 0};
	
	SDL_BlitSurface(phalanx, NULL, window->surface, &textDst);
	SDL_BlitSurface(window->authored, NULL, window->surface, &window->textDst2);
	
	SDL_Flip(window->surface);
	
	return window;
}

void displayInput(char* input, MAINWINDOW* window) {
	window->file = TTF_RenderText_Blended(window->font, "OPEN FILE: projects/", window->text_color);
	SDL_Rect textDst9 = {5, 570, 250, 600};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst9);
	window->file = TTF_RenderText_Blended(window->font, input, window->text_color);
	SDL_Rect textDst10 = {250, 570, 800, 600};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst10);
	SDL_Flip(window->surface);
}

void moveDescription(MAINWINDOW* window) {	
	SDL_BlitSurface(window->blackened, NULL, window->surface, &window->textDstOld);
	window->textDstOld.x = window->textDst2.x;
	
	if(window->textDst2.x < 800) {
		window->textDst2.x += 1;
	} else {
		window->textDst2.x = -100;
		//clearBottom(window);
	}
	SDL_BlitSurface(window->authored, NULL, window->surface, &window->textDst2);
	SDL_Flip(window->surface);
}

void setDescription(MAINWINDOW* window, CHANNEL* chan, char* phat) {
	window->authored = TTF_RenderText_Solid(window->font, phat, window->text_color);
	window->blackened = TTF_RenderText_Solid(window->font, phat, window->black);
	SDL_BlitSurface(window->authored, NULL, window->surface, &window->textDst2);
}

void setPhatText(MAINWINDOW* window, CHANNEL* chan) {
	int i, j;
	for(j = 0; j < 4; j++) {
		for(i = 0; i < 16; i++) {
			if(i == 7)
				window->chanDisplay[i] = TTF_RenderText_Shaded(window->font2, chan->chanval[j][i].wholeChan, window->black, window->green);
			else
				window->chanDisplay[i] = TTF_RenderText_Blended(window->font2, chan->chanval[j][i].wholeChan, window->text_color);
			//window->displayPos[i] = {0, 50+(i*20), 400, 20};
			SDL_Rect displayPos = {(j*200)+20, 65+(i*30), (j*200)+200, 40};
			SDL_BlitSurface(window->chanDisplay[i], NULL, window->surface, &displayPos);
			SDL_Flip(window->surface);
		}
	}
}

void moveSong(MAINWINDOW* window, CHANNEL* chan, int songpos) {
	/*int i, j;
	for(j = 0; j < 4; j++){
		window->chanDisplay[i] = TTF_RenderText_Blended(window->font2, chan->chanval[j][songpos].wholeChan, window->text_color);
	}*/
		
}

void clearBottom(MAINWINDOW* window) {
	SDL_Rect bottom = {0, 565, 800, 40};
	SDL_FillRect(window->surface, &bottom, 0);
	SDL_Flip(window->surface);
}
void clearCenter(MAINWINDOW* window) {
	SDL_Rect content = {0, 65, 800, 465};
	SDL_FillRect(window->surface, &content, 0);
	SDL_Flip(window->surface);
}
