#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

/*
name: setup_timer
purpose: sets up timer by configure registers. 
argument(s):
	periode:
		range: 0 to 65535
		purpose: set how far the counter counts untill it resets to 0, and if configures set an interrupt flag.
return value: none
*/
void
setup_timer (uint16_t period)
{
  //enable high frequency peripheral clock for the timer
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;

  //set value the timer counts to
  *TIMER1_TOP = period;

  //enable overlfow interrupt
  *TIMER1_IEN = 1;

  //start timer
  *TIMER1_CMD = 1;
}
