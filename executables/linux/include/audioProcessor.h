#ifndef __AUDIOPROCESSOR_H_INCLUDED
#define __AUDIOPROCESSOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <fileHandler.h>
#include <mainWindow.h>

typedef struct adsr {
	unsigned char atc[4];
	unsigned char dec[4];
	int sus[4];
	unsigned char rel[4];
} ADSR;

Uint32 timerCallback(Uint32 intervall, void *parameter);

float handleADSR(int i, int num, float amp, float finalAmp, int atc, int dec, int sus, int rel);

Uint8* generateSineChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec);
Uint8* generateSquareChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec);
Uint8* generateTriangleChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec);
Uint8* generateSawChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec);
Uint8* generateNoiseChunk(float freq, int num, float amp, int atc, int rel, int sus, int dec);

void sfx_init();
void setupAudio(CHANNEL* channel, MAINWINDOW* window);
void playChannels(float* freqs, float* amps, int* waveforms, CHANNEL* chan);
void setPan(float* pans);

#define M_PI  3.14159265358979323846
#define TWO_PI (M_PI*2.0)

#endif
