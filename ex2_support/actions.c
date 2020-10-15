

#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

// ------------- ACTIONS ------------- //
//functions that use the other modules to do some sort of action

/*
name: startUpSong
purpose: play a song when the microcontrollers is turned on 
argument(s):
	wave:
		range:0, 1, 2, 3
		purpose: determend which waveformat to play the song in
return value: none
*/

void startUpSong(int wave)
{
	//define the song
	int sizeArrays = 5;
	int frequencies[5] = { Hl, A, Gh, D, E };
	int lengths[5] = { 80, 80, 80, 80, 80 };

	//play the song
	makeSong(frequencies, sizeArrays, lengths, wave);

}

/*
name: updatewave
purpose: changes the value of wave so the microcontroller uses another waveformat when playing songs.
argument(s):
	*wave:
		range: the adress of the wave variable that holdes information of whiche waveformat that is currently in use
		prupose: make it possible to store a new value to the wave variabel so the microcontroller changes waveformat globaly
return value: none
*/
void updatewave(int *wave)
{
	//Change the waveformat by increasing by one
	*wave += 1;
	
	//checks if the value of the wave is outside the range to the variabel, and if so set the value to 0.
	if (*wave == 4) {
		*wave = 0;
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
		range: 0, 1, 2, 3
		purpose: determend which waveformat the song will be played in
return value: none
*/

void
makeSong(int *frequencies, int sizeArrays, int *lengths,
	 int wave)
{

	//play a song by generating the sounds/frequencies for a given amount of time/length.
	for (int i = 0; i < sizeArrays; i++) {
			makeSound(frequencies[i], lengths[i], wave);
	}
}

 /*
    name: buttonPressed
    purpose: Run different prosedures. Either it changes the wave used to play songs, or it plays a song.
    argument(s):
    	buttonX:
    		range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
    		purpose: choose which kind of prosedure will run, change wave or play a song. Set ligth to symbolize what waveformat that is currently in use. Turn off all LEDs when finished playing a song.
    	*wave:
			range: the adress of the wave variable that holdes information of whiche waveformat that is currently in use
			prupose: depending on which button that is pressed either change waveformat, or determend which waveformat a song will be played in
    return value: none
  */

void buttonPressed(int buttonX, int *wave)
{
	//set LEDs according to which waveformat that is currentliy in use
	setLEDs_waveFormat(*wave);
	
	//changing waveformat
	if (buttonX == BUTTON1) {
		updatewave(wave);
		
		//update LEDs to show the new waveformat
		setLEDs_waveFormat(*wave);
		
		//delay so the waveformats dont change in a blink of an eye
		Delay_C(10);
	}
	
	//play a song and turn off all LEDs when finished
	else {
		playSong(buttonX, *wave);
		turnOffLEDs();
	}
}


/*
name: playSong
purpose: play a song. Which song is determined by the argument.
argument(s):
	buttonX:
		range: BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
		purpose: choose song to play
	wave:
		range: 0, 1, 2, 3
		purpose: determend which waveformat the song will be played in
return value: none
*/

void playSong(int buttonX, int wave)
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
	    { 85, 80, 80, 80, 80, 80, 110, 80, 80, 110, 80, 80, 110, 80, 80, 80, 80,
		80, 80, 110, 85, 80, 80, 200 };

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

	int sizeVectors_fullStep = 22;
	int frequencies_fullStep[22] =
	    { E, F, G, A, H, C, D, Eh, Fh, Gh, Ah, Ah, Gh, Fh, Eh, D, C, H, A,
		G, F, E };
	int lengths_fullStep[22] =
	    { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
		40, 40, 40, 40, 40, 40
		};
	int sizeVectors_harry = 30;
	int frequencies_harry[30] = {E, A, C, H, A, Eh, D, H, A, C, H, G, H, E, A, C, H, A, Eh, Gh, Fh, Fh, C, Fh, Eh, Eh, E, C, A, A};
	int lengths_harry[30] ={80, 90, 80, 80, 90, 80, 120, 120, 80, 80, 80, 80, 80, 120, 80, 80, 80, 80, 80, 80, 80, 80, 100, 80, 80, 80, 80, 80, 80, 200};

	//Chooses which song to play
	switch (buttonX) {
	case BUTTON2:
		makeSong(frequencies_harry, sizeVectors_harry, lengths_harry,
			 wave);
		break;
		
	case BUTTON3:
		makeSong(frequencies_win, sizeVectors_win, lengths_win, wave);
		break;
		
	case BUTTON4:
		makeSong(frequencies_twinkleTwinkleLittleStar,
			 sizeVectors_twinkleTwinkleLittleStar,
			 lengths_twinkleTwinkleLittleStar, wave);
		break;
		
	case BUTTON5:
		makeSong(frequencies_londonBridge, sizeVectors_londonBridge,
			 lengths_londonBridge, wave);
		break;
		
	case BUTTON6:
		makeSong(frequencies_zelda, sizeVectors_zelda, lengths_zelda,
			 wave);
		break;
		
	case BUTTON7:
		makeSong(frequencies_fullStep, sizeVectors_fullStep,
			 lengths_fullStep, wave);
		break;
		
	case BUTTON8:
		makeSong(frequencies_mikkelRev, sizeVectors_mikkelRev,
			 lengths_mikkelRev, wave);
		break;
	}
}



