

#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "ex2.h"

// ------------- SONGS ------------- // TODO: må enten slette funksjoner fra action filen i såfall! 
//functions that use the other modules to make the songs



/*
name: startUpSong
purpose: plays a song when the microcontrollers is turned on 
argument(s):
	wave:
		range: 0, 1, 2, 3
		purpose: determine which waveformat to play the song in
return value: none
*/
/*
void startUpSong(int wave)
{

	int sizeVectors = 5;
	int frequencies[5] = { Hl, A, Gh, D, E };
	int lengthPerfrequency[5] = { 80, 80, 80, 80, 80 };

	makeSong(frequencies, sizeVectors, lengthPerfrequency, wave);

}

/*
name: updatewave
purpose: changes the value of wave so the microcontroller uses another waveformat when playing songs. It also shows which wave that it uses by ligth up leds accordingly
argument(s):
	*wave:
		range: 0, 1, 2, 3
		purpose: determine which waveformat to play the song in
return value: none
*/
/*
void updatewave(int *wave)
{
	//*GPIO_PA_DOUT = 0xffff;
	*wave += 1;
	if (*wave == 4) {
		*wave = 0;
	}
}
/*
name: makeSong
purpose: makes a song of the frequencies and lengths given
argument(s):
	freqencyVector:
		range: contains 0 to infty frequencies
		purpose: hold the frequencies that will be played in spesified order
	sizeVectors:
		range: the size of the frequencyVector and lengthVector
		purpose: contains the size of the vectors so we will play all the frequencies, nothing more, nothing less.
	lengthVector:
		range: contains 0 to infty lengths
		purpose: hold the length of the frequencies that will be played in same order as the frequencies that are supposed to be played in that length.
	wave: 
		range: 0, 1, 2, 3
		purpose: determine which waveformat the sound will be played in
return value: none
*/
/*
void
makeSong(int *frecquencyVector, int sizeVectors, int *lengthFrequencyVector,
	 int wave)
{
	//play the song
	for (int i = 0; i < sizeVectors; i++) {
		if (i == sizeVectors - 1){
			makeSound(frecquencyVector[i], 2*lengthFrequencyVector[i], wave);
		} else{
			makeSound(frecquencyVector[i], lengthFrequencyVector[i], wave);
		}
	}
}

 /* TODO: Mangler wave variabelen!!
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
name: playSong
purpose: play a song. Which song is determined by the argument.
argument(s):
	buttonX:
		range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
		purpose: choose song to play
	wave:
		range: 0, 1, 2, 3
		purpose: determine which waveformat to play the song in
return value: none
*//*
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
}*/



