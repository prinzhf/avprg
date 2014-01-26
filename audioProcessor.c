#include <audioProcessor.h>

CHANNEL* chan;
MAINWINDOW* window;
int upRunning = 0;
int songpos = 0;
Mix_Chunk* chunks[4];
Sint16* streams[4][44100];
int channels[4] = {0, 1, 2, 3};
int waveforms[4] = {0, 0, 0, 0};
float freqs[4] = {0.0, 0.0, 0.0, 0.0};
float amps[4] = {0.0, 0.0, 0.0, 0.0};
float pans[4] = {0.0, 0.0, 0.0, 0.0};
int atcs[4] = {0, 0, 0, 0};
int decs[4] = {0, 0, 0, 0};
int suss[4] = {1, 1, 1, 1};
int rels[4] = {0, 0, 0, 0};

//SIGNUM (c-plusplus.de), used for square-sample
int sgn(double x) { 
   if (x > 0.0L) 
     return 1.0L; 
   else if (x < 0.0L)   
     return -1.0L; 
   else   
     return 0.0L; 
}

//CALCULATE SINE SAMPLE
Uint8* generateSineChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec) {
	int i;
	double generated;
	float finalAmp = amp;
	for(i = 0; i < 44100; i++) {
		amp = handleADSR(i, num, amp, finalAmp, atc, decs[num], sus, rels[num]);
		generated = (32767*amp)*sin(TWO_PI*freq*i/44100);
		((Sint16*) streams[num])[i] = ((Sint16) generated);
	}
	return (Uint8*) streams[num];
}

//CALCULATE SQUARE SAMPLE
Uint8* generateSquareChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec) {
	int i;
	double generated;
	float finalAmp = amp;
	for(i = 0; i < (44100); i++) {
		amp = handleADSR(i, num, amp, finalAmp, atc, dec, sus, rel);
		generated = (32767*amp)*sgn(sin(TWO_PI*freq*i/44100));
		((Sint16*) streams[num])[i] = generated;
	}
	return (Uint8*) streams[num];
}

//CALCULATE TRIANGLE SAMPLE
Uint8* generateTriangleChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec) {
	int i;
	double generated;
	float finalAmp = amp;
	for(i = 0; i < 44100; i++) {
		amp = handleADSR(i, num, amp, finalAmp, atc, dec, sus, rel);
		generated = (32767*amp)*sin(TWO_PI*freq*i/44100);
		if(generated >= 0)
			((Sint16*) streams[num])[i] = ((Sint16) generated);
		else
			((Sint16*) streams[num])[i] = ((Sint16) (generated*(-1)));
	}
	return (Uint8*) streams[num];
}

//CALCULATE SAW SAMPLE
Uint8* generateSawChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec) {
	int i;//is
	double generated;
	float finalAmp = amp;
	for(i = 0; i < 44100; i++) {
		amp = handleADSR(i, num, amp, finalAmp, atc, dec, sus, rel);
		generated = (32767*amp)*sin(TWO_PI*freq*i/44100);
		((Sint16*) streams[num])[i] = generated;
	}
	return (Uint8*) streams[num];
}
// o-datei test
//CALCULATE NOISE SAMPLE
Uint8* generateNoiseChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec) {
	int i;
	float randval;
	float finalAmp = amp;
	for(i = 0; i < 44100; i++) {
		amp = handleADSR(i, num, amp, finalAmp, atc, dec, sus, rel);
		randval = rand() % 32;
		((Sint16*) streams[num])[i] = ((Sint16) ((amp * 32767)*sin(TWO_PI*(randval*freq)*i/44100)));
	}
	return (Uint8*) streams[num];
}

void sfx_init() {	
	//initiate SDL_AUDIO
	if(SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("failed to initialize SDL Audio\n");
		exit(1);
	} else {
		printf("SDL Audio sucessfully initialized\n");
	}
	//OPEN MIX
	if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 512) < 0) {
		printf("failed to Mix OpenAudio\n");
	} else {
		printf("successfully opened Audio\n");
	}
	//ALLOCATE 4 CHANNELS
	int channels = Mix_AllocateChannels(5);
	if(channels < 0) {
		printf("failed to allocate channel\n");
		exit(1);
	} else {
		printf("succesfully allocated 4 channels\n");
	}
}

//INITIATE AUDIO, ASSIGN CHANNEL-STRUCTURE, PREPARE MIX_CHUNKS
void setupAudio(CHANNEL* channel, MAINWINDOW* window) {
	sfx_init();
	chan = channel;
	window = window;
	//window = (MAINWINDOW*) malloc(sizeof(MAINWINDOW));
	int i;
	for(i=0;i<4;i++) {
		chunks[i] = malloc(sizeof(Mix_Chunk));
		chunks[i]->alen = (44100);
		chunks[i]->volume = 32;
		chunks[i]->allocated = 1;
	}
}

//PLAY EACH CHANNELS SAMPLE
void playChannels(float* freqs, float* amps, int* waveforms, CHANNEL* chan) {
	int i;
	setPan(pans);
	for(i = 0; i < 4; i++){
		if(freqs[i] != -1.0) {
			switch(waveforms[i]) {
				case 1: chunks[i]->abuf = generateSineChunk(freqs[i], i, amps[i], atcs[i], rels[i], suss[i], decs[i]); break;
				case 2: chunks[i]->abuf = generateSquareChunk(freqs[i], i, amps[i], atcs[i], rels[i], suss[i], decs[i]); break;
				case 3: chunks[i]->abuf = generateTriangleChunk(freqs[i], i, amps[i], atcs[i], rels[i], suss[i], decs[i]); break;
				case 4: chunks[i]->abuf = generateSawChunk(freqs[i], i, amps[i], atcs[i], rels[i], suss[i], decs[i]); break;
				case 5: chunks[i]->abuf = generateNoiseChunk(freqs[i], i, amps[i], atcs[i], rels[i], suss[i], decs[i]); break;
				default: chunks[i]->abuf = generateSquareChunk(freqs[i], i, amps[i], atcs[i], rels[i], suss[i], decs[i]); break;
			}
			if(suss[i] != 1)
				chunks[i]->alen = (4096*suss[i]);
			else
				chunks[i]->alen = 44100;
			Mix_PlayChannel(channels[i], chunks[i], 0);
		} else {
			Mix_ExpireChannel(channels[i], -1);
		}
	}
}

//TIMER CALLBACK
Uint32 timerCallback(Uint32 intervall, void *parameter) {
	int i;
	if(songpos >= chan->lines)
		songpos = 0;
	for(i = 0; i < 4; i++) {
		freqs[i] = chan->chanval[i][songpos].freq;
		amps[i] = chan->chanval[i][songpos].amp;
		waveforms[i] = chan->chanval[i][songpos].waveform;
		pans[i] = chan->chanval[i][songpos].pan;
		atcs[i] = chan->chanval[i][songpos].atc;
		suss[i] = chan->chanval[i][songpos].sus;
		rels[i] = chan->chanval[i][songpos].rel;
		decs[i] = chan->chanval[i][songpos].dec;
	}
	playChannels(freqs, amps, waveforms, chan);
	songpos++;
	moveSong(window, chan, songpos);
	return intervall;
}

int pauseChannels(SDL_TimerID timer, int songRun, int tempSec) {
	if(songRun = 0) {
		SDL_AddTimer(tempSec, timerCallback, NULL);
		Mix_Resume(-1);
		songRun = 1;
	} else {
		SDL_RemoveTimer(timer);
		Mix_Pause(-1);
		songRun = 0;
	}
	return songRun;
}
void resetChannels() {
	Mix_HaltChannel(-1);
	songpos = 0;
}

//SET PANNING VIA Mix_SetPanning
void setPan(float* pans) {
	int i;
	for(i = 0; i < 4; i++) {
		if(pans[i] < 0.0 && pans[i] >= -1.0) {
			if(!Mix_SetPanning(channels[i], (int)(255*(pans[i]*(-1))), (int)(255*(1-(pans[i]*(-1)))))) {
  		 		printf("Mix_SetPanning: %s\n", Mix_GetError());
	    	} 	
	    } else if(pans[i] > 0.0 && pans[i] <= 1.0){
	    	if(!Mix_SetPanning(channels[i], (int) (255*(1-pans[i])), 255*pans[i])) {
  		 		printf("Mix_SetPanning: %s\n", Mix_GetError());
  		 	}
	    } else {
	    	if(!Mix_SetPanning(channels[i], 255, 255)) {
  		 		printf("Mix_SetPanning: %s\n", Mix_GetError());
  		 	}
	    }
	}
}

//HANDLE ATTACK, !(DECAY), !(SUSTAIN) & RELEASE
float handleADSR(int i, int num, float amp, float finalAmp, int atc, int dec, int sus, int rel) {
	if(atc == 1 && rel == 1)
		return finalAmp;
	if(i == 0)
		amp = 0.001f;
		
	float adsr = 0.0f;
	if(atc != 1 && atc != 0 && amp <= finalAmp && i <= (int)(chunks[num]->alen/atc)) {
		amp += (float)((chunks[num]->alen/atc)*0.00000001);
		return amp;
	} else if(rel != 1 && rel != 0 && i > (chunks[num]->alen/rel)) {
		amp -= (float)((chunks[num]->alen/rel)*0.00000001);
		return amp;
	} else {
			return finalAmp;
	}
}
