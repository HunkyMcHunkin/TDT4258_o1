#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "efm32gg.h"
#include "ex2.h"


/*------------SET UP-------------*/
int wave = 0;  //global variable

void setupDAC()
{
	 
	 *CMU_HFPERCLKEN0 |= (1<<17);
	 *DAC0_CTRL = 0x50010;
	 *DAC0_CH0CTRL = 1;
	 *DAC0_CH1CTRL = 1;
}

void stopDAC()
{
	*DAC0_CTRL = 0x00000;
	*DAC0_CH0CTRL = 0;
	*DAC0_CH1CTRL = 0;
}

void startUpMelody()
{
	wave=0;
	int song[] = {E, F, G, A, H, C, D, D, C, H, A, G, F, E};
	ChooseWave(song, 14, 100);
}


/*------------WAVE SELECT---------------*/

void updatewave()
{
	wave += 1;
	*GPIO_PA_DOUT = 0xefff;
	if (wave==3){
		wave = 0;
	}
}

void ChooseWave(int *frecVec, int lengthFrecVec, int lengthnote)
{
	if(wave==0){
		MakeSong(frecVec, lengthFrecVec, lengthnote);
	} else if(wave==1){
		MakeSongS(frecVec, lengthFrecVec, lengthnote);
	} else if(wave==2){
		MakeSongT(frecVec, lengthFrecVec, lengthnote);
	}
	else {
		*GPIO_PA_DOUT = 0x00ff;
	}
}


/*-------------SQARE WAVE-------------------*/


void MakeSound(int freq, int length)   
{ 
	int count = 1;    //tell antall tiks
	int dacvolt = 0;  //defines strength of sound
	length=length*1000; //tonens lengde=spesifisert lengde * 1ms
	while(count < length){
		int countperiod = 44100/freq;
		if (count % (countperiod/2) == 0){   
			if(dacvolt == 100){
				dacvolt = 0;
			} else{
				dacvolt = 100;
			}
			*DAC0_CH0DATA = dacvolt;  //skriver voltverdien til utgangen
			*DAC0_CH1DATA = dacvolt;
		}
		
		count++;
	}
}

void MakeSong(int *frecVec, int lengthFrecVec, int lengthnote){
	*GPIO_PA_DOUT = 0xc3ff;
	for (int i = 0; i<lengthFrecVec; i++){
		MakeSound(frecVec[i],lengthnote);
	}
	MakeSound(frecVec[lengthFrecVec-1], lengthnote*2);
	*GPIO_PA_DOUT = 0xffff;
}


/*---------------SABERTHOOT-------------------*/

void MakeSoundS(int freq, int length) 
{
	int count = 1;    
	int dacvolt = 0;
	int countperiod = 44100/freq;
	int dacUpTime = countperiod/100;   //calculates how often i must update voltage to swing between 0 and 100
	int rate=1;
	if (dacUpTime<1){
		dacUpTime=1;
		if (freq>1323){
			rate=3;
		} else if (freq>882){
			rate=2;
		}
	}
	int dacUpCount = 1; 
	length=length*1000; 
	while(count < length){
		if (count % countperiod == 0){   
			dacvolt=0;
		}
		*DAC0_CH0DATA = dacvolt;  
		*DAC0_CH1DATA = dacvolt;
		if (dacUpCount>=dacUpTime){
			dacvolt=dacvolt+(1*rate);
			dacUpCount=0;
		}
		count++;
		dacUpCount++;
	}
}

void MakeSongS(int *frecVec, int lengthFrecVec, int lengthnote){
	*GPIO_PA_DOUT = 0x55ff;
	for (int i = 0; i<lengthFrecVec; i++){
		MakeSoundS(frecVec[i],lengthnote);
	}
	MakeSoundS(frecVec[lengthFrecVec-1], lengthnote*2);
	*GPIO_PA_DOUT = 0xffff;
}



/*------------------TRIANGLE WAVE---------------------*/

void MakeSoundT(int freq, int length) 
{
	int count = 1;
	int dacVolt = 0;          
	int dacdir = 1;
	int countperiod = 44100/freq; 
	int dacUpTime=countperiod/200;
	int rate=1;
	if (dacUpTime<1){
		dacUpTime=1;
		if (freq>1102){
			rate=5;
		} else if (freq>882){
			rate=4;
		} else if (freq>662){
			rate=3;
		} else if (freq>441){
			rate=2;
		}
	}
	int dacUpCount = 0;
	length=length*1000; 
	while(count < length){
		if (count % (countperiod/2) == 0){   
			dacdir=dacdir*(-1);
		}
		*DAC0_CH0DATA = dacVolt;  
		*DAC0_CH1DATA = dacVolt;
		if (dacUpCount>=dacUpTime){
			dacUpCount=0;
			dacVolt=dacVolt+(dacdir*rate);
			if (dacVolt>100){
				dacVolt=100;
			} else if (dacVolt<0){
				dacVolt=0;
			}
		}
		count++;
		dacUpCount++;
	}
}

void MakeSongT(int *frecVec, int lengthFrecVec, int lengthnote){
	*GPIO_PA_DOUT = 0x1fff;
	for (int i = 0; i<lengthFrecVec; i++){
		MakeSoundT(frecVec[i],lengthnote);
	}
	MakeSoundT(frecVec[lengthFrecVec-1], lengthnote*2);
	*GPIO_PA_DOUT = 0xffff;
}



