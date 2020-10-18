#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

//------------ ex2 -------------//
//Here we have the main function the busy wait polling solution uses

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

		buttonStatus = *GPIO_PC_DIN;
		buttonPressed(buttonStatus);

	}
}
