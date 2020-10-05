#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"


int main(void)
{
	uint32_t button_in;

	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	startUpMelody();

	while (1){

		button_in=*GPIO_PC_DIN;
		buttonPressed(button_in);
		
	}
}
