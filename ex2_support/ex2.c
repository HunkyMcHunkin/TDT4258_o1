#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

 /*
    name: main
    purpose: program the microcontroller runs. Sets up nessesary settings, checks for buttonpresses
    argument(s): none
    return value: none
  */
int main(void)
{
	//set up a pointer to a variabel that will hold what waveformat the microcontroller use to play songs
	int *wave;
	
	//set to waveformat 2, which correspond to triangle sound waves
	*wave = 2;
	
	//holds the status of the GPIO buttons
	uint32_t button_in;

	//setup configurations
	setup_GPIO();
	setup_DAC();
	startDAC();
	setup_timer(SAMPLE_PERIOD);
	startUpSong(*wave);

	//operate on buttonpress
	while (1) {

		button_in = *GPIO_PC_DIN;
		buttonPressed(button_in, wave);

	}
}
