#include <audioProcessor.h>

CHANNEL* chan;
int songpos = 0;
Mix_Chunk* chunks[4];
Sint16* streams[4][44100];
int channels[4] = {0, 1, 2, 3};
int waveforms[4] = {0, 0, 0, 0};
float freqs[4] = {0.0, 0.0, 0.0, 0.0};
float amps[4] = {0.0, 0.0, 0.0, 0.0};
float pans[4] = {0.0, 0.0, 0.0, 0.0};
float atcs[4] = {0.0, 0.0, 0.0, 0.0};
float decs[4] = {0.0, 0.0, 0.0, 0.0};
float suss[4] = {0.0, 0.0, 0.0, 0.0};
float rels[4] = {0.0, 0.0, 0.0, 0.0};

ADSR* adsr;

//SIGNUM (c-plusplus.de)
int sgn(double x) { 
   if (x > 0.0L) 
     return 1.0L; 
   else if (x < 0.0L)   
     return -1.0L; 
   else   
     return 0.0L; 
}

//CALCULATE SINE SAMPLE
Uint8* generateSineChunk(float freq, int num, float amp, float atc) {
	int i;
	double generated;
	float attack = 1.0f;
	for(i = 0; i < 44100; i++) {
		attack = setAttack(1.0, amp, attack);
		//printf("%f - ", attack);
		generated = (32767*amp)*sin(TWO_PI*freq*i/44100);
		//generated += setAttack(1.0, 1);
		//printf("%f\n", setAttack(1.0, 0));
		((Sint16*) streams[num])[i] = ((Sint16) generated);
	}
	return (Uint8*) streams[num];
}

//CALCULATE SQUARE SAMPLE
Uint8* generateSquareChunk(float freq, int num, float amp) {
	int i;
	double generated;
	for(i = 0; i < 22050; i++) {
		generated = (32767*amp)*sgn(sin(TWO_PI*freq*i/44100));
		((Sint16*) streams[num])[i] = generated;
	}
	return (Uint8*) streams[num];
}

//CALCULATE TRIANGLE SAMPLE
Uint8* generateTriangleChunk(float freq, int num, float amp) {
	int i;
	double generated;
	for(i = 0; i < 44100; i++) {
		generated = (32767*amp)*sin(TWO_PI*freq*i/44100);
		if(generated >= 0)
			((Sint16*) streams[num])[i] = ((Sint16) generated);
		else
			((Sint16*) streams[num])[i] = ((Sint16) (generated*(-1)));
	}
	return (Uint8*) streams[num];
}//this

//CALCULATE SAW SAMPLE
Uint8* generateSawChunk(float freq, int num, float amp) {
	int i;//is
	double generated;
	for(i = 0; i < 44100; i++) {
		//((Sint16*) streams[num])[i] = ((Sint16) ((32767 * amp)*(sin(TWO_PI*freq*tm1/44100)/fabs(sin(TWO_PI*freq*tm1/44100)))));
		
		generated = (32767*amp)*sgn(sin(TWO_PI*freq*i/44100));
		/*if(((32767*amp)*sin(TWO_PI*freq*(i+1)/44100)) >= ((32767*amp)*sin(TWO_PI*freq*i/44100)))
			((Sint16*) streams[num])[i] = generated;
		else
			((Sint16*) streams[num])[i] = 0;
		*/
		((Sint16*) streams[num])[i] = generated;
	}
	return (Uint8*) streams[num];
}

//CALCULATE NOISE SAMPLE
Uint8* generateNoiseChunk(float freq, int num, float amp) {
	int i;
	float randval;
	float aAmp = 0.7;
	for(i = 0; i < 44100/2; i++) {
		aAmp -= 0.002;
		randval = rand() % 32;
		((Sint16*) streams[num])[i] = ((Sint16) ((aAmp * 32767)*sin(TWO_PI*(randval*freq)*i/44100)));
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
	if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024) < 0) {
		printf("failed to Mix OpenAudio\n");
	} else {
		printf("successfully opened Audio\n");
	}
	//ALLOCATE 4 CHANNELS
	int channels = Mix_AllocateChannels(4);
	if(channels < 0) {
		printf("failed to allocate channel\n");
		exit(1);
	} else {
		printf("succesfully allocated 4 channels\n");
	}
}

//INITIATE AUDIO, ASSIGN CHANNEL-STRUCTURE, PREPARE MIX_CHUNKS
void setupAudio(CHANNEL* channel) {
	sfx_init();
	chan = channel;
	adsr = (ADSR*) malloc(sizeof(ADSR));
	
	int i;
	for(i=0;i<4;i++) {
		chunks[i] = malloc(sizeof(Mix_Chunk));
		chunks[i]->alen = 44100;
		chunks[i]->volume = 32;
		chunks[i]->allocated = 1;
	}
}

//PLAY EACH CHANNELS SAMPLE
void playChannels(float* freqs, float* amps, int* waveforms, CHANNEL* chan) {
	int i;
	for(i = 0; i < 4; i++){
		setPan(pans);
		switch(waveforms[i]) {
			case 1: chunks[i]->abuf = generateSineChunk(freqs[i], i, amps[i], atcs[i]); break;
			case 2: chunks[i]->abuf = generateSquareChunk(freqs[i], i, amps[i]); break;
			case 3: chunks[i]->abuf = generateTriangleChunk(freqs[i], i, amps[i]); break;
			case 4: chunks[i]->abuf = generateSawChunk(freqs[i], i, amps[i]); break;
			case 5: chunks[i]->abuf = generateNoiseChunk(freqs[i], i, amps[i]); break;
			default: chunks[i]->abuf = generateSquareChunk(freqs[i], i, amps[i]); break;
		}
		channels[i] = Mix_PlayChannel(channels[i], chunks[i], 0);
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
		adsr->atc[i] = chan->chanval[i][songpos].atc;
		adsr->dec[i] = chan->chanval[i][songpos].dec;
		adsr->sus[i] = chan->chanval[i][songpos].sus;
		adsr->rel[i] = chan->chanval[i][songpos].rel;
	}
	playChannels(freqs, amps, waveforms, chan);
	songpos++;
	return intervall;
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

//HANDLE ATTACK, DECAY, SUSTAIN & RELEASE
float setAttack(float val, float amp, float curAmp) {
	float atcAmp = 0.0;
	
	return atcAmp;
}
float setDecay() {
	
}
float setSustain() {
	
}
float setRelease(float adsr, float amp) {
	float relAmp = 0.0;
	if(adsr <= amp) {
		relAmp -= 0.01;
	}
	return relAmp;
}
