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

void MakeSound(int freq, int length)
{ 
	int count = 1;
	int dacvolt = 0;
	while(count < length){
		if (count % freq == 0){
			if(dacvolt== 100){
				dacvolt = 0;
			} else{
				dacvolt = 100;
			}
			*DAC0_CH0DATA = dacvolt;
			*DAC0_CH1DATA = dacvolt;
		}
		count++;
	}
}

/*void wait(time){
	counter=0;
	while(counter < time){
		counter++
	}
}*/

