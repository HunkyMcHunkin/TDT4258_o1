#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "efm32gg.h"

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

void MakeSound(int freq, int length)   //square wave
{ 
	int count = 1;    //tell antall tiks
	int dacvolt = 0;  //defines strength of sound
	length=length*1000; //tonens lengde=spesifisert lengde * 1ms
	while(count < length){
		int countperiod = 44100/freq;
		if (count % (countperiod/2) == 0){   
			if(dacvolt == 100){
				dacvolt = 0;
				*GPIO_PA_DOUT = 0xefff;
			} else{
				dacvolt = 100;
				*GPIO_PA_DOUT = 0xffff;
			}
			*DAC0_CH0DATA = dacvolt;  //skriver voltverdien til utgangen
			*DAC0_CH1DATA = dacvolt;
		}
		
		count++;
	}
}

void MakeSong(int *frecVec, int lengthFrecVec, int lengthnote){
	for (int i = 0; i<lengthFrecVec; i++){
		if(i==lengthFrecVec-1){
			MakeSound(frecVec[i],lengthnote*2);
		}
		MakeSound(frecVec[i],lengthnote);
	}
}

void MakeSoundS(int freq, int length)  //saberthoot wave - does not work yet
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
			*DAC0_CH0DATA = dacvolt;  
			*DAC0_CH1DATA = dacvolt;
		}
		if (dacUpCount==dacUpTime){
			dacvolt=dacvolt+(1*rate);
			dacUpCount=0;
		}
		count++;
		dacUpCount++;
	}
}

void MakeSongS(int *frecVec, int lengthFrecVec, int lengthnote){
	for (int i = 0; i<lengthFrecVec; i++){
		if(i==lengthFrecVec-1){
			MakeSoundS(frecVec[i],lengthnote*2);
		}
		MakeSoundS(frecVec[i],lengthnote);
	}
}

void MakeSoundT(int freq, int length)  //triangle wave  - warning verry LOUD
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
	int dacUpCount = 1;
	length=length*1000; 
	while(count < length){
		if (count % (countperiod/2) == 0){   
			dacdir=dacdir*(-1);
			*DAC0_CH0DATA = dacVolt;  
			*DAC0_CH1DATA = dacVolt;
		}
		if (dacUpCount==dacUpTime){
			dacVolt=dacVolt+dacdir*rate;
			dacUpCount=0;
		}
		count++;
		dacUpCount++;
	}
}

void MakeSongT(int *frecVec, int lengthFrecVec, int lengthnote){
	for (int i = 0; i<lengthFrecVec; i++){
		if(i==lengthFrecVec-1){
			MakeSoundT(frecVec[i],lengthnote*2);
		}
		MakeSoundT(frecVec[i],lengthnote);
	}
}



