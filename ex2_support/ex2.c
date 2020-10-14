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
int
main (void)
{
  int *wave;
  *wave = 3;
  uint32_t button_in;
<<<<<<< HEAD
=======
  //bool past_button_state = 1;
>>>>>>> 78a3ffa37ba7d598b83ada504c76e08c7e588801

  //setup configurations
  setup_GPIO ();
  setup_DAC ();
  startDAC ();
  setup_timer (SAMPLE_PERIOD);
  startUpSong (*wave);

  //operate on buttonpress
  while (1)
    {

      button_in = *GPIO_PC_DIN;
<<<<<<< HEAD
      buttonPressed (button_in, wave);

=======
      // Enable the buttonPressed() every time a button is pressed.
      // If the button haven't been pressed before, and it is being pressed..
      // .. enable buttonPressed() and set past_button_state = 0.
      //if ((past_button_state == 1) && (button_in == BUTTON1))
      //{
      //past_button_state = 0;
      buttonPressed(button_in, wave);
      //}
      // Reset the past state when the button is released
      //if (button_in == BUTTON0)
      //{
      //  past_button_state = 1;
      //}
>>>>>>> 78a3ffa37ba7d598b83ada504c76e08c7e588801
    }
}
