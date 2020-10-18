#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

// ------------- ACTIONS ------------- //
//functions that use the other modules to do some sort of action

int waveFormat_globalVariable = 0;

/*
	name: playSound
	purpose: play a sound of a certain waveformat determend of the parameter waveFormat
	argument(s):
		freqency:
			range: minimum frequency to maximum frequency
			purpose: the prosedyre plays a sound with this frequency
		length:
			range: 0 to infy
			purpose: determend how long the sound will play
		waveFormat: 
			range: SQUARE, SAWTOOTH, TRIANGLE, SINUS
			purpose: determend which waveformat the sound will be played in
	return value: none
*/
void playSound(int freqency, int length, int waveFormat)
{
	switch (waveFormat) {
	case SQUARE:
		makeSound_square(freqency, length);
		break;
	case SAWTOOTH:
		makeSound_sawtooth(freqency, length);
		break;
	case TRIANGLE:
		makeSound_triangle(freqency, length);
		break;
	case SINUS:
		makeSound_sinus(freqency, length);
		break;
	}
}

/*
	name: startUpSong
	purpose: play a song with wave format sinus when the microcontrollers is turned on 
	argument(s): none
	return value: none
*/

void startUpSong()
{
	//define the song
	int sizeArrays = 5;
	int frequencies[5] = { Hl, A, Gh, D, E };
	int lengths[5] = { 60, 60, 60, 60, 60 };

	//set waveformat
	int waveFormat = SINUS;
	
	//play the song
	makeSong(frequencies, sizeArrays, lengths, waveFormat);

}

/*
	name: updateWaveFormat
	purpose: change what wave format the microcontroller uses when playing songs.
	argument(s): none
	return value: none
*/
void updateWaveFormat()
{
	//Change the waveformat by increasing the waveformat held by the gloabal variable by one
	waveFormat_globalVariable += 1;
	
	//checks if the value of the wave is outside the range to the variabel, and if so set the value to SQUARE.
	if (waveFormat_globalVariable == 4) {
		waveFormat_globalVariable = SQUARE;
	}
}

/*
	name: makeSong
	purpose: makes a song of the frequencies and lengths given
	argument(s):
		frequencies:
			range: contains 0 to infty frequencies
			purpose: an array that hold the frequencies that will be played in cronological order
		sizeArrays:
			range: the size of the frequencies array and lengths array
			purpose: contains the size of the vectors so we will play all the frequencies, nothing more, nothing less.
		lengths:
			range: contains 0 to infty lengths
			purpose: an array that hold how long the corresponding frequency in the frequencies array will play
		wave: 
			range: SQUARE, SAWTOOTH, TRIANGLE, SINUS
			purpose: determend which waveformat the song will be played in
	return value: none
*/

void
makeSong(int *frequencies, int sizeArrays, int *lengths,
	 int waveFormat)
{

	//play a song by generating the sounds of certain frequencies and waveformat for a given amount of time
	for (int i = 0; i < sizeArrays; i++) {
			playSound(frequencies[i], lengths[i], waveFormat);
	}
}

 /*
    name: buttonPressed
    purpose: Run different prosedures. Either it changes the wave format used to play songs, or it plays a song with wave format determend by the gloabal variable that holdes the wave format. Set ligths to symbolize what waveformat that is currently in use. Turn off all LEDs when finished playing a song.
    argument(s):
    	buttonX:
    		range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
    		purpose: choose which kind of prosedure will run, change wave or play a song
    return value: none
  */

int waveTest = 3;

void buttonPressed(int buttonX)
{		
	//changing waveformat
	if (buttonX == BUTTON1) {
		updateWaveFormat();
		setLEDs_waveFormat(waveFormat_globalVariable);
				
		//delay so the waveformats dont change in a blink of an eye
		delayMicroSeconds(20);
		
		
	}
	
	//play a song
	else {
		setLEDs_waveFormat(waveFormat_globalVariable);
		playSong(buttonX, waveFormat_globalVariable);
	}
	
	turnOffLEDs();
}


/*
name: playSong
purpose: play a song. Which song is determined by the argument.
argument(s):
	buttonX:
		range: BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
		purpose: choose song to play
	wave:
		range: SQUARE, SAWTOOTH, TRIANGLE, SINUS
		purpose: determend which waveformat the song will be played in
return value: none
*/

void playSong(int buttonX, int waveFormat)
{
	//define songs
	int sizeVectors_fail = 3;
	int frequencies_fail[3] = { A, F, Hl };
	int lengths_fail[3] = { 70, 70, 200 };

	int sizeVectors_win = 9;
	int frequencies_win[9] = { Hl, Cl, Dl, E, F, G, H, A, D };
	int lengths_win[9] = { 130, 40, 40, 40, 40, 40, 130, 40, 200 };

	int sizeVectors_twinkleTwinkleLittleStar = 14;
	int frequencies_twinkleTwinkleLittleStar[14] =
	    { E, E, C, C, D, D, C, H, H, A, A, G, G, F };
	int lengths_twinkleTwinkleLittleStar[14] =
	    { 100, 100, 100, 100, 100, 100, 180, 100, 100, 100, 100, 100, 100, 200 };

	int sizeVectors_londonBridge = 24;
	int frequencies_londonBridge[24] =
	    { C, D, C, H, A, H, C, G, A, H, A, H, C, C, D, C, H, A, H, C, G, C, A, F };
	int lengths_londonBridge[24] =
	    { 65, 60, 60, 60, 60, 60, 90, 60, 60, 90, 60, 60, 90, 60, 60, 60, 60,
		60, 60, 90, 65, 60, 60, 150 };

	int sizeVectors_zelda = 11;
	int frequencies_zelda[11] = { E, G, Dl, E, G, Dl, E, G, D, C, G };
	int lengths_zelda[11] = { 120, 120, 180, 120, 120, 180, 120, 120, 120, 100, 200 };

	int sizeVectors_mikkelRev = 24;
	int frequencies_mikkelRev[24] =
	    { E, G, Cl, E, G, Cl, F, A, C, A, A, G, E, G, Cl, E, G, Cl, Dl, E,
		F, Hl, Dl, Cl};
	int lengths_mikkelRev[24] =
	    { 70, 70, 140, 70, 70, 140, 70, 70, 70, 70, 140, 140, 70, 70, 140,
		70, 70, 140, 70, 70, 70, 70, 140, 200};

	int sizeVectors_harryPotter = 30;
	int frequencies_harryPotter[30] = {E, A, C, H, A, Eh, D, H, A, C, H, G, H, E, A, C, H, A, Eh, Gh, Fh, Fh, C, Fh, Eh, Eh, E, C, A, A};
	int lengths_harryPotter[30] ={80, 90, 80, 80, 90, 80, 120, 120, 80, 80, 80, 80, 80, 120, 80, 80, 80, 80, 80, 80, 80, 80, 100, 80, 80, 80, 80, 80, 80, 200};

	//Chooses which song to play
	switch (buttonX) {
	case BUTTON2:
		makeSong(frequencies_fail, sizeVectors_fail, lengths_fail,
			 waveFormat);
		break;
		
	case BUTTON3:
		makeSong(frequencies_win, sizeVectors_win, lengths_win, waveFormat);
		break;
		
	case BUTTON4:
		makeSong(frequencies_twinkleTwinkleLittleStar,
			 sizeVectors_twinkleTwinkleLittleStar,
			 lengths_twinkleTwinkleLittleStar, waveFormat);
		break;
		
	case BUTTON5:
		makeSong(frequencies_londonBridge, sizeVectors_londonBridge,
			 lengths_londonBridge, waveFormat);
		break;
		
	case BUTTON6:
		makeSong(frequencies_zelda, sizeVectors_zelda, lengths_zelda,
			 waveFormat);
		break;
		
	case BUTTON7:
		makeSong(frequencies_harryPotter, sizeVectors_harryPotter, lengths_harryPotter,
			 waveFormat);
		break;
		
	case BUTTON8:
		makeSong(frequencies_mikkelRev, sizeVectors_mikkelRev,
			 lengths_mikkelRev, waveFormat);
		break;
	}
}



