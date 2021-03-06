#include <fileHandler.h>

static const char *namedNotes[] = {"c-2", "c#2", "d-2", "d#2", "e-2", "f-2", "f#2", "g-2", "g#2", "a-2", "a#2", "b-2",
									"c-3", "c#3", "d-3", "d#3", "e-3", "f-3", "f#3", "g-3", "g#3", "a-3", "a#3", "b-3",
									"c-4", "c#4", "d-4", "d#4", "e-4", "f-4", "f#4", "g-4", "g#4", "a-4", "a#4", "b-4", 
									"c-5", "c#5", "d-5", "d#5", "e-5", "f-5", "f#5", "g-5", "g#5", "a-5", "a#5", "b-5", "c-6", "000", NULL};
static const float frequencedNotes[] = {65.4064, 69.2957, 73.4162, 77.7817, 82.4069, 87.3071, 92.4986, 97.9989, 103.826, 110.000, 116.541, 123.471,
										130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220.0, 233.082, 246.942,
										261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440.000, 466.164, 493.883, 
										523.251, 554.365, 587.330, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880.000, 932.328, 987.767, 1046.50, -1.0, 0.0};

//STORE NOTE-RELATED FREQUENCY
float getNoteFrequency(char noteName[]) { 
     int i; 
     float frequency = 0.0; 
     for(i = 0;  namedNotes[i]; i++) { 
             if(strcmp(namedNotes[i], noteName) == 0) { 
             frequency = frequencedNotes[i]; 
             return frequency;
         } 
     }
     return 0.0; 
}

//PARSE .PHAT-FILE & .DESC-FILE, STORE IN CHANNEL-STRUCTURE
CHANNEL* getChannelData(FILE* chanFile) {
	
	int i, j;
	CHANNEL* chan = (CHANNEL*) chanFile;
	if(chan != NULL)
		chan = (CHANNEL*) malloc(sizeof(CHANNEL));
	CHANVAL* chanval1 = (CHANVAL*) chanFile;
	if(chanval1 != NULL)
		chanval1 = (CHANVAL*) malloc(sizeof(CHANVAL)*1024);
	CHANVAL* chanval2 = (CHANVAL*) chanFile;
	if(chanval2 != NULL)
		chanval2 = (CHANVAL*) malloc(sizeof(CHANVAL)*1024);
	CHANVAL* chanval3 = (CHANVAL*) chanFile;
	if(chanval3 != NULL)
		chanval3 = (CHANVAL*) malloc(sizeof(CHANVAL)*1024);
	CHANVAL* chanval4 = (CHANVAL*) chanFile;
	if(chanval4 != NULL)
		chanval4 = (CHANVAL*) malloc(sizeof(CHANVAL)*1024);
		
	int chanCount = 0, wCount = 0;
	char chanLine[128];
	
	int nvals, dvals, wvals;
	
	while(fgets(chanLine, sizeof(chanLine), chanFile)) {
		nvals = sscanf(chanLine, "%d %f %f %s %d %d %d %d %d %f %f %s %d %d %d %d %d %f %f %s %d %d %d %d %d %f %f %s %d %d %d %d", &chanval1[chanCount].waveform, &chanval1[chanCount].amp, &chanval1[chanCount].pan, &chanval1[chanCount].tone[0], &chanval1[chanCount].atc, &chanval1[chanCount].dec, &chanval1[chanCount].sus, &chanval1[chanCount].rel, &chanval2[chanCount].waveform, &chanval2[chanCount].amp, &chanval2[chanCount].pan, &chanval2[chanCount].tone[0], &chanval2[chanCount].atc, &chanval2[chanCount].dec, &chanval2[chanCount].sus, &chanval2[chanCount].rel, &chanval3[chanCount].waveform, &chanval3[chanCount].amp, &chanval3[chanCount].pan, &chanval3[chanCount].tone[0], &chanval3[chanCount].atc, &chanval3[chanCount].dec, &chanval3[chanCount].sus, &chanval3[chanCount].rel, &chanval4[chanCount].waveform, &chanval4[chanCount].amp, &chanval4[chanCount].pan, &chanval4[chanCount].tone[0], &chanval4[chanCount].atc, &chanval4[chanCount].dec, &chanval4[chanCount].sus, &chanval4[chanCount].rel);
		//printf("waveform: %d, amp: %.1f, pan: %.1f, note: %s atc: %d, dec: %d\n", chanval2[chanCount].waveform, chanval2[chanCount].amp, chanval2[chanCount].pan, chanval2[chanCount].tone, (int)chanval2[chanCount].atc, (int)chanval2[chanCount].dec);
		if(nvals == 0)
			continue;
		chanCount++;
	}
	
	chan->chanval[0] = chanval1;
	chan->chanval[1] = chanval2;
	chan->chanval[2] = chanval3;
	chan->chanval[3] = chanval4;
	
	for(i = 0; i <= chanCount; i++) {
		chan->chanval[0][i].freq = getNoteFrequency(chanval1[i].tone);
		chan->chanval[1][i].freq = getNoteFrequency(chanval2[i].tone);
		chan->chanval[2][i].freq = getNoteFrequency(chanval3[i].tone);
		chan->chanval[3][i].freq = getNoteFrequency(chanval4[i].tone);
	}
	
	for(j = 0; j < 4; j++) {
		int x;
		char waveform[sizeof(int)];
		char amp[sizeof(float)];
		char pan[sizeof(float)];
		char tone[4];
		char atc[sizeof(int)];
		char dec[sizeof(int)];
		char sus[sizeof(int)];
		char rel[sizeof(int)];
			
		for(x = 0; x <= chanCount; x++) {
			snprintf(waveform, sizeof(int), "%d", chan->chanval[j][x].waveform);
			snprintf(amp, sizeof(float), "%f", chan->chanval[j][x].amp);
			snprintf(pan, sizeof(float), "%f", chan->chanval[j][x].pan);
			snprintf(tone, 4, "%s", chan->chanval[j][x].tone);
			snprintf(atc, sizeof(int), "%d", chan->chanval[j][x].atc);
			snprintf(dec, sizeof(int), "%d", chan->chanval[j][x].dec);
			snprintf(sus, sizeof(int), "%d", chan->chanval[j][x].sus);
			snprintf(rel, sizeof(int), "%d", chan->chanval[j][x].rel);
			strcat(chan->chanval[j][x].wholeChan, waveform);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, amp);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, pan);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, tone);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, atc);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, dec);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, sus);
			strcat(chan->chanval[j][x].wholeChan, "  ");
			strcat(chan->chanval[j][x].wholeChan, rel);
		}
	}
	
	printf("lines in chanFile: %d\n", chanCount);
	/*free(chanval1);
	free(chanval2);
	free(chanval3);
	free(chanval4);
	*/
	chan->lines = chanCount;
	
	return chan;
}
