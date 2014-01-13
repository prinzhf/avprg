#ifndef __AUDIOPROCESSOR_H_INCLUDED
#define __AUDIOPROCESSOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <fileHandler.h>

typedef struct adsr {
	float atc[4];
	float dec[4];
	int sus[4];
	float rel[4];
} ADSR;

Uint32 timerCallback(Uint32 intervall, void *parameter);

float setAttack(float val, float amp, float curAmp);
float setDecay();
float setSustain();
float setRelease(float adsr, float amp);

Uint8* generateSineChunk(float freq, int num, float amp, float atc);
Uint8* generateSquareChunk(float freq, int num, float amp);
Uint8* generateTriangleChunk(float freq, int num, float amp);
Uint8* generateSawChunk(float freq, int num, float amp);
Uint8* generateNoiseChunk(float freq, int num, float amp);

void sfx_init();
void setupAudio(CHANNEL* channel);
void playChannels(float* freqs, float* amps, int* waveforms, CHANNEL* chan);
void setPan(float* pans);

#define M_PI  3.14159265358979323846
#define TWO_PI (M_PI*2.0)

#endif
