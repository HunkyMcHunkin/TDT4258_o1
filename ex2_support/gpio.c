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
purpose: Run different prosedures. Either it changes the wave used to play songs, or it plays a song.
argument(s):
	buttonX:
		range: BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8
		purpose: choose which kind of prosedure will run, change wave or play a song

return value: none
*/ 
void buttonPressed(int buttonX, int *wave)
{
	if(buttonX == BUTTON1){
		updatewave(&wave);
	}else{
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
return value: none
*/
void playSong(int buttonX, int wave){

	//songs with frequency defines and how long each frequency in the songs will play
	
	int sizeVectors_pokemon = 17;
	//int frequency_pokemon_old[17] = { A, A, A, A, 1, 1, A, G, E, Cl, 1, Cl, A, A, G, F, G };
	int frequencies_pokemon[17] = { G, G, C, D, E, Ah, G, Ah, G, F, C, C, D, E, F, G, Ah };
	int lengthPerfrequency_pokemon [17] = { 80, 80, 80, 40, 40, 80, 80, 80, 80, 80, 80, 45, 45, 70, 80, 80, 80 }; //FEIL!!
	
	int sizeVectors_twinkleTwinkleLittleStar = 14;
	int frequencies_twinkleTwinkleLittleStar[] = { E, E, C, C, D, D, C, H, H, A, A, G, G, F };
	int lengthPerfrequency_twinkleTwinkleLittleStar[] = { 80, 80, 80, 80, 80, 80, 160, 80, 80, 80, 80, 80, 80, 160 };
	
	int sizeVectors_londonBridge = 24;
	int frequencies_londonBridge[24] = { C, D, C, H, A, H, C, G, A, H, A, H, C, C, D, C, H, A, H, C, G, C, A, F };
	int lengthPerfrequency_londonBridge [24] = { 70, 70, 70, 70, 70, 70, 140, 70, 70, 140, 70, 70, 140, 70, 70, 70, 70, 70, 70, 140, 95, 95, 95};
	
	int sizeVectors_song = 35;
	int frequencies_song[35] = { D, F, 1, 1, F, E, 1, 1, G, A, H, A, G, F, 1, D, E, G, D, D, E, F, G, C, H, 1, G, A, 1, D, E, F, G, F, E};
	int lengthPerfrequency_song[35] = {70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70}; //?
	
	int sizeVectors_zelda = 11;
	int frequencies_zelda[11] = { E, G, Dl, E, G, Dl, E, G, D, C, G };
	int lengthPerfrequency_zelda[11] = { 70, 70, 140, 70, 70, 140, 70, 70, 70, 70, 70 };
	
	int sizeVectors_mikkelRev = 24;
	int frequencies_mikkelRev[24] = { E, G, Cl, E, G, Cl, F, A, C, A, A, G, E, G, Cl, E, G, Cl, Dl, E, F, Hl, Dl, Cl, };
	int lengthPerfrequency_mikkelRev[24] = { 70, 70, 140, 70, 70, 140, 70, 70, 70, 70, 140, 140, 70, 70, 140, 70, 70, 140, 70, 70, 70, 70, 140, 140 };
	
	//int sizeVectors_fullStep = 14;
	//int frequencies_fullStep[] = { E, F, G, A, H, C, D, Eh, Fh, Gh, Ah, Ah, Gh, Fh, Eh, D, C, H, A, G, F, E };
	//int lengthPerfrequency_pokemon [
	
	int sizeVectors_harryPotter = 14;
	int frequencies_harryPotter[14] = {Hl, E, G, F, E, Hl, A, F, E, G, F, E, F, Hl};
	int lengthPerfrequency_harryPotter[14] = {40, 80, 30, 30, 80, 50, 100, 100, 40, 80, 30, 30, 80, 100};
	
	//Chooses which song to play
	switch(buttonX){
		case BUTTON2:
			makeSong(frequencies_pokemon, sizeVectors_pokemon, lengthPerfrequency_pokemon, wave);
			break;
		case BUTTON3:
			makeSong(frequencies_twinkleTwinkleLittleStar, sizeVectors_twinkleTwinkleLittleStar, lengthPerfrequency_twinkleTwinkleLittleStar, wave);
			break;
		case BUTTON4:
			makeSong(frequencies_londonBridge, sizeVectors_londonBridge, lengthPerfrequency_londonBridge, wave);
			break;
		case BUTTON5:
			makeSong(frequencies_song, sizeVectors_song, lengthPerfrequency_song, wave);
			break;
		case BUTTON6:
			makeSong(frequencies_zelda, sizeVectors_zelda , lengthPerfrequency_zelda, wave);
			break;
		case BUTTON7:
			makeSong(frequencies_mikkelRev, sizeVectors_mikkelRev, lengthPerfrequency_mikkelRev, wave);
			break;
		case BUTTON8:
			makeSong(frequencies_harryPotter, sizeVectors_harryPotter, lengthPerfrequency_harryPotter, wave);
			break;
	}
}
