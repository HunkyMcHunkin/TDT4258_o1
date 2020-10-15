#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "efm32gg.h"
#include "ex2.h"

/*------------SET UP-------------*/

/*
name: setup_DAC
purpose: sets up a digial to analog converter (DAC) by configure registers. 
argument(s): none
return value: none
*/

void setup_DAC()
{
	//enable high frequency peripheral clock for the timer
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;

	//set clock frequency to DAC as high frequency peripheral clock frecuency divided by 2^5
	*DAC0_CTRL |= 0x50000;
}

/*
name: startDAC
purpose: start the digital to analog converter (DAC) by configure registers. 
argument(s): none
return value: none
*/

void startDAC()
{
	//enable DAC output pin, disable DAC output to analog to digital converter (ADC) and ACMP TODO: ACMP?
	*DAC0_CTRL |= 0x00010;

	//enable channel 0
	*DAC0_CH0CTRL = 1;

	//enable channel 1
	*DAC0_CH1CTRL = 1;
}

/*
name: stopDAC
purpose: stop the digital to analog converter (DAC) from operating by configure registers. 
argument(s): none
return value: none
*/
void stopDAC()
{
	//disable DAC output to pin and ADC, and set the frequency to high frequency peripheral clock frecuency 
	*DAC0_CTRL &= ~(1 << 1);

	//disable channel 0
	*DAC0_CH0CTRL = 0;

	//disable channel 1
	*DAC0_CH1CTRL = 0;
}

/*
name: startUpSong
purpose: plays a song when the microcontrollers is turned on 
argument(s): none
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

/*------------WAVE SELECT---------------*/

/*
name: updatewave
purpose: changes the value of wave so the microcontroller uses another waveformat when playing songs. It also shows which wave that it uses by ligth up leds accordingly
argument(s): none
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
name: makeSound
purpose: make sound, but chooses what kind of sound to make depending on the value of wave
argument(s):
	freqency:
		range: min frequency to max frequency
		purpose: the prosedyre makes sound this frequency
	length:
		range: 0, infy
		purpose: determend how long the sound will play
	wave: 
		range: 0, 1, 2, 3
		purpose: determend which waveformat the sound will be played in
return value: none
*/
void makeSound(int freqency, int length, int wave)
{
	switch (wave) {
	case 0:
		makeSound_square(freqency, length);
		break;
	case 1:
		makeSound_saberthoot(freqency, length);
		break;
	case 2:
		makeSound_triangle(freqency, length);
		break;
	case 3:
		makeSound_sinus(freqency, length);
		break;
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
		purpose: determend which waveformat the sound will be played in
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

/*-------------SQARE WAVE-------------------*/
/*
name: makeSound_square
purpose: generate sound from a square wave
argument(s):
	freq:
		range: 20 to 2000 (which will translate to 20 Hz to 2000 Hz)??????
		purpose: determend the harmonic frequency for the wave we want to ganerate
	length:
		range: 0 to infty. Specified in mili seconds.
		purpose: determend how long the generated sound wave schould last 
return value: none
*/
void makeSound_square(int freq, int length)
{
	//holds the count of number of ticks
	int count = 1;

	//holds the strength of the sound 
	int dacvolt = 0;

	//Number of samples the microcontroller generates per second.
	int samplesPerSecond = 44100;

	//Number of tics per periode
	int countperiod = samplesPerSecond / freq;

	//turn length into length in milli seconds. length = {determed length} * 1ms
	length = length * 2000;

	//Does this loop as long as the wave havent reaches its length.
	while (count < length) {
		//Checks if a half periode is reached
		if (count % (countperiod / 2) == 0) {

			//Change output from high to low or low to high
			if (dacvolt == 100) {
				dacvolt = 0;
			} else {
				dacvolt = 100;
			}

			//Give a sample to the DAC 
			*DAC0_CH0DATA = dacvolt;
			*DAC0_CH1DATA = dacvolt;
		}
		//Write a sample to DAC 
		count++;
	}
}

/*---------------SABERTHOOT-------------------*/

void makeSound_saberthoot(int freq, int length)
{
	int count = 1;
	int dacvolt = 0;
	int samplesPerSecond = 44100;
	int countperiod = samplesPerSecond / freq;
	int dacUpTime = countperiod / 100;	//calculates how often i must update voltage to swing between 0 and 100
	int rate = 1;
	if (dacUpTime < 1) {
		dacUpTime = 1;
		if (freq > 1323) {
			rate = 3;
		} else if (freq > 882) {
			rate = 2;
		}
	}
	int dacUpCount = 1;
	length = length * 1100;
	while (count < length) {
		if (count % countperiod == 0) {
			dacvolt = 0;
		}
		*DAC0_CH0DATA = dacvolt;
		*DAC0_CH1DATA = dacvolt;
		if (dacUpCount >= dacUpTime) {
			dacvolt = dacvolt + (1 * rate);
			dacUpCount = 0;
		}
		count++;
		dacUpCount++;
	}
}

/*------------------TRIANGLE WAVE---------------------*/
/*
name: makeSound_triangle
purpose: generate sound from a triangle wave
argument(s):
	freq:
		range: 20 to 2000 (which will translate to 20 Hz to 2000 Hz)??????
		purpose: determend the harmonic frequency for the wave we want to ganerate
	length:
		range: 0 to infty. Specified in mili seconds.
		purpose: determend how long the generated sound wave schould last 
return value: none
*/
void makeSound_triangle(int freq, int length)
{

	//holds the count of number of ticks
	int count = 1;

	//holds the strength of the sound 
	int dacVolt = 0;

	//Number of samples the microcontroller generates per second.
	int samplesPerSecond = 44100;

	//Number of tics per periode
	int countperiod = samplesPerSecond / freq;

	//turn length into length in milli seconds. length = {determed length} * 1ms
	length = length * 1000;

	//variable for increacing/decresing dacVolt
	int dacdir = 1;
	
	//deside how often the dacVolt shuld be updated to get a value between 0 and 100 for each half period
	int dacUpTime = countperiod / 200;
	
	//
	//int dacUpCount = 0;
	int rate = 1;

	//Decides the amplitude of how much the samples change from one sample value to next depending on the frequency  
	if (dacUpTime < 1) {
		dacUpTime = 1;
		if (freq > 1102) {
			rate = 5;
		} else if (freq > 882) {
			rate = 4;
		} else if (freq > 662) {
			rate = 3;
		} else if (freq > 441) {
			rate = 2;
		}
	}
	//Does this loop as long as the wave havent reaches its length.
	while (count < length) {	
		if (count % (countperiod / 2) == 0) { //for each half period, change direction of DacVoltage
			dacdir = dacdir * (-1);
		}
		//Give a sample to the DAC 
		*DAC0_CH0DATA = dacVolt;
		*DAC0_CH1DATA = dacVolt;
		if ((count % dacUpTime) == 0) {  //update the value at correct time to swing between 0 and 100 in half of a period
			//dacUpCount = 0;
			dacVolt = dacVolt + (dacdir * rate);
			if (dacVolt>100){   		//checks if value have gone outside range, and cut the triangle signal if it have
			      dacVolt=100;
			} else 
			if (dacVolt < 0) {
				dacVolt = 0;
			}
			count++;
			//dacUpCount++;
		}
	}
}

/*---------------SINUS WAVE-------------------*/
/*
name: makeSound_sinus
purpose: generate sound from a sinus wave by using a lookup table. The periode to the wave will be divided into 28 parts with equal amount of ticks, where the first part will generate samples of the lookup tables first cell, the second from the lookup table from the second cell, and so on.
argument(s):
	freq:
		range: 20??? to 2000??? (which will translate to 20 Hz to 2000 Hz)??????
		purpose: determend the harmonic frequency for the wave we want to ganerate 
	length:
		range: 0 to infty. Specified in mili seconds.
		purpose: determend how long the generated sound wave schould last
return value: none
*/
	void makeSound_sinus(int freq, int length) {
		//aThe size and the array for a lookup table used to generate the sinus wave
		int sinusVecLength = 28;
		int sinusVec[28] =
		    { 100, 98, 95, 89, 81, 71, 61, 50, 38, 28, 18, 10, 4, 1, 0,
	     1, 4,
			10, 18, 28, 38, 49, 61, 71, 81, 89, 95, 98
		};

		//To keep track of the index in the sinus lookup table
		int sinus_table_index = 0;

		//holds the count of number of ticks
		int count = 1;

		//holds the strength of the sound 
		int dacvolt = 0;

		//Number of samples the microcontroller generates per second.
		int samplesPerSecond = 44100;

		//Number of tics per periode
		int countperiod = samplesPerSecond / freq;
		
		length = length * 1300;

		//Does this loop as long as the wave havent reaches its length
		while (count < length) {
			//checks if current ticks is big enought to change output sample value to the next value in the sinus look up table.
			if (count % (countperiod / sinusVecLength) == 0) {
				//Checks if we have reached the end of the look up table
				if (sinus_table_index == sinusVecLength) {
					//If so start form the beginning.
					sinus_table_index = 0;
				}
				//set sample value to the value in the given location in the look up table
				dacvolt = sinusVec[sinus_table_index];

				//Change to next cell in the lookup table
				sinus_table_index++;

			}
			//Write a sample to DAC 
			*DAC0_CH0DATA = dacvolt;	//skriver voltverdien til utgangen
			*DAC0_CH1DATA = dacvolt;

			//increace the count, to ceep track of note-length
			count++;
		}
	}
