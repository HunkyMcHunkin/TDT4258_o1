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
void setup_timer(uint16_t period)
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

/*
	name: time_microseconds
	purpose: count tics specified by the time specified by uS TODO: uS ???? Skjønner at det er en parameter, men hva står uS for? er det mulig med et bedre forklart navn eller forklaring til variablene?
	JEG TROR DET STÅR FOR TID I MIKRO SEKUNDER (10^-6 sekunder =\MU sekunder)
	argument(s):
			uS:
				range: 0 to 4294967296
				purpose: Specify the value to count up to
	
	return value: none
*//*
void delayMicroSeconds(uint32_t uS)
{
	uint32_t i, s = 0;

	for (i = 0; i < uS; i++)
		s++;
}

/*
	name: Delay_C TODO: hvorfor delay_C og ikke bare delay? Konvensjonen er at funksjoner starter med små bokstaver
	purpose: Delay by mS times 10 milliseconds.
	argument(s):
			mS:
				range: 0 to 4294967296
				purpose: Specify the delay in milliseconds.
	return value: none
*//*
void delayMilliSeconds(uint32_t mS)
{
	uint32_t i;
	for (i = 0; i < mS; i++) {
		delayMicroSeconds(10000);
	};
}

/////////////////
/*
	name: delayMicroSeconds
	purpose: give a delay in micro seconds specified by the argument by counting where each count takes approximatly 1 micro second
	argument(s):
			microSeconds:
				range: 0 to 4294967296
				purpose: Specify how long the delay in micro seconds is
	return value: none
*//*
void delayMicroSeconds(uint32_t microSeconds) 
{
	uint32_t count = 0;
	while (count < microSeconds){
		count++;
	}
}

/*
	name: delayMilliSeconds
	purpose: give a delay in milli seconds specified by the argument
	argument(s):
			mS:
				range: 0 to 4294967296
				purpose: Specify the delay in milli seconds.
	return value: none
*//*
void delayMilliSeconds(uint32_t milliSeconds)
{
	uint32_t i;
	for (i = 0; i < milliSeconds; i++) {
		delayMicroSeconds(1000*10);
	};
}*/
