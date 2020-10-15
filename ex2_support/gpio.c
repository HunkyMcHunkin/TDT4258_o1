#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

 /*
    name: setup_GPIO
    purpose: sets up GPIO mode and interrupts by configure registers. 
    argument(s): none
    return value: none
  */
void setup_GPIO()
{
	//enable high frequency peripheral clock for the timer
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	//set lowest drive strength
	*GPIO_PA_CTRL = 1;

	//set pins A8-15 as output
	*GPIO_PA_MODEH = 0x55555555;

	//turn all LEDs off
	*GPIO_PA_DOUT = 0xffff;

	//set buttons as output
	*GPIO_PC_MODEL = 0x33333333;

	//enable internal pull up
	*GPIO_PC_DOUT = 0xff;
}

 /*
    name: buttonPressed
    purpose: Run different procedures. Either it changes the wave used to play songs, or it plays a song.
    argument(s):
    buttonX:
    range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
    purpose: choose which kind of procedure will run, change wave or play a song

    return value: none
  */
/*
void buttonPressed(int buttonX, int *wave)
{
	//changing waveformat
	if (buttonX == BUTTON1) {
		updatewave(wave);
		setLEDs_waveFormat(*wave);
		Delay_C(10);
	}
	//playing a song
	else {
		//setLEDs_songs(buttonX);
		setLEDs_waveFormat(*wave);
		playSong(buttonX, *wave);
	}
}

/*
name: setLEDs_waveFormat
purpose: Lighting LEDs to indicate which waveform is being used.
argument(s):
	wave:
		range: 0, 1, 2, 3
		purpose: define which waveform is being used
return value: none
*/

void setLEDs_waveFormat(int wave)
{
	// choosing which LEDs to light, and lighting them
	switch (wave) {
	case 3:
		*GPIO_PA_DOUT = (0x00ff);
		break;
	case 2:
		*GPIO_PA_DOUT = (0x03ff);
		break;
	case 1:
		*GPIO_PA_DOUT = (0x0fff);
		break;
	case 0:
		*GPIO_PA_DOUT = (0x3fff);
		break;
	case 4:
		*GPIO_PA_DOUT = (0x3fff);
		break;
	}
}

/*
name: setLEDs_songs
purpose: Lighting a LED to indicate which song is being played.
argument(s):
		buttonX:
			range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
			purpose: Identify which song is being played so that the corresponding LED can be lit.
return value: none
*/
void setLEDs_songs(int buttonX)
{
	// choosing which LED to light, and lighting it
	switch (buttonX) {
	case BUTTON2:
		*GPIO_PA_DOUT = LED2;
		break;
	case BUTTON3:
		*GPIO_PA_DOUT = LED3;
		break;
	case BUTTON4:
		*GPIO_PA_DOUT = LED4;
		break;
	case BUTTON5:
		*GPIO_PA_DOUT = LED5;
		break;
	case BUTTON6:
		*GPIO_PA_DOUT = LED6;
		break;
	case BUTTON7:
		*GPIO_PA_DOUT = LED7;
		break;
	case BUTTON8:
		*GPIO_PA_DOUT = LED8;
		break;
	}
}

/*
name: turnOffLEDs
purpose: Turn off all the LEDs.
argument(s): none
return value: none
*/
void turnOffLEDs()
{
	*GPIO_PA_DOUT = 0xffff;
}

/*
name: playSong
purpose: play a song. Which song is determined by the argument.
argument(s):
	buttonX:
		range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
		purpose: choose song to play
return value: none
*/
/*
void playSong(int buttonX, int wave)
{
	int sizeVectors_fail = 3;
	int frequencies_fail[] = { A, F, Hl };
	int lengths_fail[] = { 70, 70, 100 };

	int sizeVectors_win = 9;
	int frequencies_win[] = { Hl, Cl, Dl, E, F, G, H, A, D };
	int lengths_win[] = { 130, 40, 40, 40, 40, 40, 130, 40, 120 };

	int sizeVectors_twinkleTwinkleLittleStar = 14;
	int frequencies_twinkleTwinkleLittleStar[] =
	    { E, E, C, C, D, D, C, H, H, A, A, G, G, F };
	int lengths_twinkleTwinkleLittleStar[] =
	    { 100, 100, 100, 100, 100, 100, 180, 100, 100, 100, 100, 100, 100, 100 };

	int sizeVectors_londonBridge = 24;
	int frequencies_londonBridge[] =
	    { C, D, C, H, A, H, C, G, A, H, A, H, C, C, D, C, H, A, H, C, G, C, A, F };
	int lengths_londonBridge[] =
	    { 85, 80, 80, 80, 80, 80, 110, 80, 80, 110, 80, 80, 110, 80, 80, 80, 80,
		80, 80, 110, 85, 80, 80, 80 };

	int sizeVectors_zelda = 11;
	int frequencies_zelda[] = { E, G, Dl, E, G, Dl, E, G, D, C, G };
	int lengths_zelda[] = { 120, 120, 180, 120, 120, 180, 120, 120, 120, 100, 100 };

	int sizeVectors_mikkelRev = 24;
	int frequencies_mikkelRev[] =
	    { E, G, Cl, E, G, Cl, F, A, C, A, A, G, E, G, Cl, E, G, Cl, Dl, E,
		F, Hl, Dl, Cl};
	int lengths_mikkelRev[] =
	    { 70, 70, 140, 70, 70, 140, 70, 70, 70, 70, 140, 140, 70, 70, 140,
		70, 70, 140, 70, 70, 70, 70, 140, 140};

	int sizeVectors_fullStep = 22;
	int frequencies_fullStep[] =
	    { E, F, G, A, H, C, D, Eh, Fh, Gh, Ah, Ah, Gh, Fh, Eh, D, C, H, A,
		G, F, E };
	int lengths_fullStep[] =
	    { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40
		};
	int sizeVectors_harry = 30;
	int frequencies_harry[] = {E, A, C, H, A, Eh, D, H, A, C, H, G, H, E, A, C, H, A, Eh, Gh, Fh, Fh, C, Fh, Eh, Eh, E, C, A, A};
	int lengths_harry[] ={80, 90, 80, 80, 90, 80, 120, 120, 80, 80, 80, 80, 80, 120, 80, 80, 80, 80, 80, 80, 80, 80, 100, 80, 80, 80, 80, 80, 80, 80};

	//Chooses which song to play
	switch (buttonX) {
	case BUTTON2:
		makeSong(frequencies_harry, sizeVectors_harry, lengths_harry,
			 wave);
		turnOffLEDs();
		break;
	case BUTTON3:
		makeSong(frequencies_win, sizeVectors_win, lengths_win, wave);
		turnOffLEDs();
		break;
	case BUTTON4:
		makeSong(frequencies_twinkleTwinkleLittleStar,
			 sizeVectors_twinkleTwinkleLittleStar,
			 lengths_twinkleTwinkleLittleStar, wave);
		turnOffLEDs();
		break;
	case BUTTON5:
		makeSong(frequencies_londonBridge, sizeVectors_londonBridge,
			 lengths_londonBridge, wave);
		turnOffLEDs();
		break;
	case BUTTON6:
		makeSong(frequencies_zelda, sizeVectors_zelda, lengths_zelda,
			 wave);
		turnOffLEDs();
		break;
	case BUTTON7:
		makeSong(frequencies_fullStep, sizeVectors_fullStep,
			 lengths_fullStep, wave);
		turnOffLEDs();
		break;
	case BUTTON8:
		makeSong(frequencies_mikkelRev, sizeVectors_mikkelRev,
			 lengths_mikkelRev, wave);
		turnOffLEDs();
		break;
	}
}

 /*
    name: buttonPressed
    purpose: Run different procedures. Either it changes the wave used to play songs, or it plays a song.
    argument(s):
    	buttonX:
    		range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
    		purpose: choose which kind of procedure will run, change wave or play a song
  	  wave:
		range: 0, 1, 2, 3
		purpose: determend which waveformat to play the song in
    

    return value: none
  */

void buttonPressed(int buttonX, int *wave)
{
	//changing waveformat
	if (buttonX == BUTTON1) {
		updatewave(wave);
		setLEDs_waveFormat(*wave);
		Delay_C(10);
	}
	//playing a song
	else {
		//setLEDs_songs(buttonX);
		setLEDs_waveFormat(*wave);
		playSong(buttonX, *wave);
	}
}

/*
name: Time
purpose: Count the time specified by uS
argument(s):
		uS:
			range: 0 to 4294967296
			purpose: Specify the value to count up to
	
return value: none
*/
void Time(uint32_t uS)
{
	uint32_t i, s = 0;

	for (i = 0; i < uS; i++)
		s++;
}

/*
name: Delay_C
purpose: Delay by mS times 10 milliseconds.
argument(s):
		mS:
			range: 0 to 4294967296
			purpose: Specify the delay in milliseconds.
return value: none
*/
void Delay_C(uint32_t mS)
{
	uint32_t i;
	for (i = 0; i < mS; i++) {
		Time(10000);
	};
}
