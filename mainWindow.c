#include <stdio.h>
#include <mainWindow.h>

MAINWINDOW createWindow(MAINWINDOW window) {
	
	window.surface = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_SWSURFACE);
	if(window.surface == NULL) {
		printf("surface error");
	}
	
	//TITLE
	SDL_WM_SetCaption("Phalanx-Tracker", "Phalanx-Tracker");
	
	//WHITE LINES
	SDL_Rect rectTopLine = {0, 45, 800, 5};
	SDL_FillRect(window.surface, &rectTopLine, 0xFFFFFF);
	SDL_Rect rectBottomLine = {0, 555, 800, 5};
	SDL_FillRect(window.surface, &rectBottomLine, 0xFFFFFF);
	
	//FONTS
	window.font = TTF_OpenFont("fonts/slickerregular.ttf", 24);
	if(window.font == NULL) {
		printf("error in font\n");
	}
	window.font2 = TTF_OpenFont("fonts/slickerregular.ttf", 12);
	if(window.font2 == NULL) {
		printf("error in font\n");
	}
	window.font3 = TTF_OpenFont("fonts/slickerregular.ttf", 18);
	if(window.font3 == NULL) {
		printf("error in font\n");
	}
	
	//HEADLINE
	SDL_Surface *phalanx = TTF_RenderText_Blended(window.font, "phalanx-tracker", window.text_color);
	
	//INFO TEXT
	window.file = TTF_RenderText_Blended(window.font2, "Press 's' to start song", window.text_color);
	SDL_Rect textDst3 = {0, 0, 100, 15};
	SDL_BlitSurface(window.file, NULL, window.surface, &textDst3);
	window.file = TTF_RenderText_Blended(window.font2, "Press 'p' to pause/unpause song", window.text_color);
	SDL_Rect textDst4 = {0, 15, 100, 30};
	SDL_BlitSurface(window.file, NULL, window.surface, &textDst4);
	window.file = TTF_RenderText_Blended(window.font2, "Press 'r' to reset song", window.text_color);
	SDL_Rect textDst5 = {0, 30, 100, 45};
	SDL_BlitSurface(window.file, NULL, window.surface, &textDst5);
	window.file = TTF_RenderText_Blended(window.font2, "Press 'o' to open song", window.text_color);
	SDL_Rect textDst6 = {670, 0, 700, 15};
	SDL_BlitSurface(window.file, NULL, window.surface, &textDst6);
	window.file = TTF_RenderText_Blended(window.font2, "Press 't' to set tempo", window.text_color);
	SDL_Rect textDst7 = {670, 15, 700, 30};
	SDL_BlitSurface(window.file, NULL, window.surface, &textDst7);
	window.file = TTF_RenderText_Blended(window.font2, "Press 'esc' to exit", window.text_color);
	SDL_Rect textDst8 = {670, 30, 700, 45};
	SDL_BlitSurface(window.file, NULL, window.surface, &textDst8);
	
	SDL_Rect textDst = {300, 7.5, 0, 0};
	
	SDL_BlitSurface(phalanx, NULL, window.surface, &textDst);
	SDL_BlitSurface(window.authored, NULL, window.surface, &window.textDst2);
	
	SDL_Flip(window.surface);
	
	return window;
}

void displayInput(char* input, MAINWINDOW* window) {
	window->file = TTF_RenderText_Blended(window->font, "OPEN FILE: projects/", window->text_color);
	SDL_Rect textDst9 = {5, 562.5, 0, 0};
	SDL_BlitSurface(window->file, NULL, window->surface, &textDst9);
	window->file = TTF_RenderText_Blended(window->font, input, window->text_color);
	SDL_Rect textDst10 = {250, 562.5, 0, 0};
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
	//SDL_BlitSurface(window->authored, NULL, window->surface, &window->textDst2);
}

void setPhatText(MAINWINDOW* window, CHANNEL* chan) {
	int i;
	for(i = 0; i < chan->lines; i++) {
		if(window->channel1Pos.y < 550) {
			if(i == 0) {
				window->channel1Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[0][i].wholeChan, window->black, window->green);
				window->channel2Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[1][i].wholeChan, window->black, window->green);
				window->channel3Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[2][i].wholeChan, window->black, window->green);
				window->channel4Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[3][i].wholeChan, window->black, window->green);
				SDL_BlitSurface(window->channel1Visible, NULL, window->surface, &window->channel1Pos);
				SDL_BlitSurface(window->channel2Visible, NULL, window->surface, &window->channel2Pos);
				SDL_BlitSurface(window->channel3Visible, NULL, window->surface, &window->channel3Pos);
				SDL_BlitSurface(window->channel4Visible, NULL, window->surface, &window->channel4Pos);
				
			} else {
				window->channel1Visible = TTF_RenderText_Blended(window->font2, chan->chanval[0][i].wholeChan, window->text_color);
				window->channel2Visible = TTF_RenderText_Blended(window->font2, chan->chanval[1][i].wholeChan, window->text_color);
				window->channel3Visible = TTF_RenderText_Blended(window->font2, chan->chanval[2][i].wholeChan, window->text_color);
				window->channel4Visible = TTF_RenderText_Blended(window->font2, chan->chanval[3][i].wholeChan, window->text_color);
				SDL_BlitSurface(window->channel1Visible, NULL, window->surface, &window->channel1Pos);
				SDL_BlitSurface(window->channel2Visible, NULL, window->surface, &window->channel2Pos);
				SDL_BlitSurface(window->channel3Visible, NULL, window->surface, &window->channel3Pos);
				SDL_BlitSurface(window->channel4Visible, NULL, window->surface, &window->channel4Pos);
			}
			window->channel1Pos.y += 20;
			window->channel2Pos.y += 20;
			window->channel3Pos.y += 20;
			window->channel4Pos.y += 20;
			SDL_Flip(window->surface);
		}
	}
}

void moveSong(MAINWINDOW* window, CHANNEL* chan) {
	int i;
	if(window->channel1Pos.y > 60) {
		window->channel1Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[0][i].wholeChan, window->black, window->green);
		window->channel2Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[1][i].wholeChan, window->black, window->green);
		window->channel3Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[2][i].wholeChan, window->black, window->green);
		window->channel4Visible = TTF_RenderText_Shaded(window->font2, chan->chanval[3][i].wholeChan, window->black, window->green);
		SDL_BlitSurface(window->channel1Visible, NULL, window->surface, &window->channel1Pos);
		SDL_BlitSurface(window->channel2Visible, NULL, window->surface, &window->channel2Pos);
		SDL_BlitSurface(window->channel3Visible, NULL, window->surface, &window->channel3Pos);
		SDL_BlitSurface(window->channel4Visible, NULL, window->surface, &window->channel4Pos);
	}
	SDL_Flip(window->surface);
}

void clearBottom(MAINWINDOW* window) {
	SDL_Rect bottom = {0, 562.5, WIDTH, HEIGHT};
	SDL_FillRect(window->surface, &bottom, 0);
	SDL_Flip(window->surface);
}
void clearContent(MAINWINDOW* window) {
	/*SDL_Rect content = {0, 70, 800, 80};
	SDL_FillRect(window->surface, &content, 0);
	SDL_Flip(window->surface);*/
	free(window->channel1Visible);
	SDL_Flip(window->surface);
}
