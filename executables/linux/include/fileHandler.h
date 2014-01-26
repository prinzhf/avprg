#ifndef __FILEHANDLER_H_INCLUDED
#define __FILEHANDLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct channel_values {
	unsigned int waveform;
	float amp;
	float pan;
	char tone[3];
	float freq;
	int atc;
	int dec;
	int sus;
	int rel;
	char wholeChan[128];
} CHANVAL;

typedef struct channel {
	unsigned char line;
	int tempo;
	char name[128];
	char description[128];
	CHANVAL* chanval[4];
	int lines;
} CHANNEL;

CHANNEL* getChannelData(FILE* chanFile);
float getNoteFrequency(char noteName[]);

#define LINELEN 128;

#endif
