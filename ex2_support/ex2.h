#ifndef ex2
#define ex2

#define SAMPLE_PERIOD 318   		//The period between sound samples, in clock cycles 

/*dac*/
void setupDAC();
void stopDAC();
void startUpMelody();
void updatewave();
void ChooseWave(int *frecVec, int lengthFrecVec, int lengthnote);
void MakeSound(int freq, int length);
void MakeSong(int *frecVec, int lengthFrecVec, int lengthnote);
void MakeSoundT(int freq, int length);
void MakeSongT(int *frecVec, int lengthFrecVec, int lengthnote);
void MakeSoundS(int freq, int length);
void MakeSongS(int *frecVec, int lengthFrecVec, int lengthnote);
void updatewave();

/*GPIO*/
void setupGPIO();
void buttonPressed(int buttonX);

/*timer*/
void setupTimer(uint16_t period);

/*interrupt*/
void setupNVIC();
void Sleep();
void setUpDisableRam();

/*songs*/
#define Hl 123
#define Cl 131
#define Dl 147
#define E 165
#define F 175
#define G 196
#define A 220
#define H 267
#define C 262
#define D 294
#define Eh 330
#define Fh 349
#define Gh 392
#define Ah 440

#endif
