#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

 /*
    name: main
    purpose: program the microcontroller runs. Set up nessesary settings, checks for buttonpresses, and operate on according to the status of the buttons
    argument(s): none
    return value: none
  */
int main(void)
{

	uint32_t buttonStatus;

	//setup configurations
	setup_GPIO();
	setup_DAC();
	startDAC();
	setup_timer(SAMPLE_PERIOD);
	startUpSong();

	//operate on buttonpress
	while (1) {

		button_in = *GPIO_PC_DIN;
		buttonPressed(buttonStatus);

	}
}
