#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "efm32gg.h"
#include "ex2.h"

//------------ DAC -------------//
//Here we have functions related to the DAC. In addition to function operating on the DACs registers, this will include sound generating functions

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
	//enable DAC output pin, disable DAC output to analog to digital converter (ADC)
	*DAC0_CTRL |= 0x00010;

	//enable channel 0
	*DAC0_CH0CTRL = 1;

	//enable channel 1
	*DAC0_CH1CTRL = 1;
}

/*
	name: stopDAC
	purpose: stop the digital to analog converter (DAC) from operating 
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
	name: makeSound_square
	purpose: generate sound from a square wave
	argument(s):
		frequency:
			range: minimum to maximum hearable frequency
			purpose: determend the harmonic frequency for the wave we want to ganerate
		length:
			range: 0 to infty. Specified in mili seconds.
			purpose: determend how long the generated sound wave schould last 
	return value: none
*/
void makeSound_square(int frequency, int length)
{
	int count = 1;
	int dacVoltage = 0;
	int samplesPerSecond = 44100;
	int countsPerPeriod = samplesPerSecond / frequency;
	int cmuClockFrequencyInMHz = 14000;
	
	// the average clock periods needed to compleat one while iteration
	int clockPeriodsPerWhileLoopIteration = 6;
	
	//turn length into length in ms into corresponding while itterations. 
	//length = {determed length in milli second} / {average while loop time in milli seconds}
	length = length * (cmuClockFrequencyInMHz / (clockPeriodsPerWhileLoopIteration));

	while (count < length) {
		//Checks if a half periode is reached
		if (count % (countsPerPeriod / 2) == 0) {

			if (dacVoltage == 100) {
				dacVoltage = 0;
			} else {
				dacVoltage = 100;
			}


		}
		//Write the voltage to the DAC 
		*DAC0_CH0DATA = dacVoltage;
		*DAC0_CH1DATA = dacVoltage;
		
		//increace the count, to ceep track of note-length
		count++;
	}
}

/*
	name: makeSound_sawtooth
	purpose: generate sound from a sawtooth wave
	argument(s):
		frequency:
			range:minimum to maximum hearable frequency
			purpose: determend the harmonic frequency for the wave we want to ganerate
		length:
			range: 0 to infty. Specified in mili seconds.
			purpose: determend how long the generated sound wave schould last 
	return value: none
*/
void makeSound_sawtooth(int frequency, int length)
{
	int count = 1;
	int dacVoltage = 0;
	int samplesPerSecond = 44100;
	int countsPerPeriod = samplesPerSecond / frequency;
	int cmuClockFrequencyInMHz = 14000;
	int clockPeriodsPerWhileLoopIteration = 10;
	int incrementRate = 1;
	
	//turn length into length into corresponding while itterations. 
	//length = {determed length in second} / {average while loop time}
	length = length * (cmuClockFrequencyInMHz / clockPeriodsPerWhileLoopIteration); 
	
	//calculates how often i must update voltage to swing between 0 and 100
	int dacUpTime = countsPerPeriod / 100;
	
	
	if (dacUpTime < 1) {
		dacUpTime = 1;
		if (frequency > 1323) {
			incrementRate = 3;
		} else if (frequency > 882) {
			incrementRate = 2;
		}
	}

	while (count < length) {
		//Checks if a periode is reached
		if (count % countsPerPeriod == 0) {
			dacVoltage = 0;
		}
		
		//checks if we should update voltage
		if ((count % dacUpTime) == 0) {
			dacVoltage = dacVoltage + (1 * incrementRate);
		}
		
		//Write the voltage to the DAC 
		*DAC0_CH0DATA = dacVoltage;
		*DAC0_CH1DATA = dacVoltage;
		
		//increace the count, to ceep track of note-length
		count++;
	}
}


/*
	name: makeSound_triangle
	purpose: generate sound from a triangle wave
	argument(s):
		frequency:
			range: minimum to maximum hearable frequency
			purpose: determend the harmonic frequency for the wave we want to ganerate
		length:
			range: 0 to infty. Specified in mili seconds.
			purpose: determend how long the generated sound wave schould last 
	return value: none
*/
void makeSound_triangle(int frequency, int length)
{

	int count = 1;
	int dacVoltage = 0;
	int samplesPerSecond = 44100;
	int countsPerPeriod = samplesPerSecond / frequency;
	int cmuClockFrequencyInMHz = 14000;
	int clockPeriodsPerWhileLoopIteration = 11;
	int incrementRate = 1;
	
	//turn length into length into corresponding while itterations. 
	//length = {determed length in second} / {average while loop time}
	length = length * (cmuClockFrequencyInMHz / clockPeriodsPerWhileLoopIteration); 

	//variable that determends dacVoltage should increacing or decresing
	int dacDirection = 1;
	
	//deside how often the dacVolt shuld be updated to get a value between 0 and 100 for each half period
	int dacUpTime = (countsPerPeriod/2) / 100;
	
	
	if (dacUpTime < 1) {
		dacUpTime = 1;
		if (frequency > 1102) {
			incrementRate = 5;
		} else if (frequency > 882) {
			incrementRate = 4;
		} else if (frequency > 662) {
			incrementRate = 3;
		} else if (frequency > 441) {
			incrementRate = 2;
		}
	}

	while (count < length) {
	
		//for each half period, change direction of DacVoltage	
		if (count % (countsPerPeriod / 2) == 0) { 
			dacDirection = dacDirection * (-1);
		}
		
		//update the value at correct time to swing between 0 and 100 in half of a period
		if ((count % dacUpTime) == 0) {  
			dacVoltage = dacVoltage + (dacDirection * incrementRate);
			
			//checks if value have gone outside range, and cut the triangle signal if it have
			if (dacVoltage>100){   		
			      dacVoltage=100;
			} else if (dacVoltage < 0) {
				dacVoltage = 0;
			}
			
			//Give a sample to the DAC 
			*DAC0_CH0DATA = dacVoltage;
			*DAC0_CH1DATA = dacVoltage;
			
			//increace the count, to ceep track of note-length
			count++;
		}
	}
}


/*
	name: makeSound_sinus
	purpose: generate sound from a sinus wave by using a lookup table. The periode to the wave will be divided into 28 parts with equal amount of ticks, where the first part will generate samples of the lookup tables first cell, the second from the lookup table from the second cell, and so on.
	argument(s):
		frequency:
			range: minimum to maximum hearable frequency
			purpose: determend the harmonic frequency for the wave we want to ganerate 
		length:
			range: 0 to infty. Specified in mili seconds.
			purpose: determend how long the generated sound wave schould last
	return value: none
*/
void makeSound_sinus(int frequency, int length) {

	int count = 1;
	int dacVoltage = 0;
	int samplesPerSecond = 44100;
	int countsPerPeriod = samplesPerSecond / frequency;
	int cmuClockFrequencyInMHz = 14000;
	int clockPeriodsPerWhileLoopIteration = 9;
	
	//turn length into length into corresponding while itterations. 
	//length = {determed length in second} / {average while loop time}
	length = length * (cmuClockFrequencyInMHz / clockPeriodsPerWhileLoopIteration); 

	//Lookup table for sinus
	int sizeSinusLookupTable = 28;
	int sinusLookupTable[28] = { 100, 98, 95, 89, 81, 71, 61, 50, 38, 28, 18, 10, 4, 1, 0, 1, 4, 10, 18, 28, 38, 49, 61, 71, 81, 89, 95, 98};
	int indexsinusLookupTable = 0;


		while (count < length) {
		//update the value at correct time to the next value in the sinus look up table.
			if (count % (countsPerPeriod / sizeSinusLookupTable) == 0) {
			
				//Checks if we have reached the end of the look up table and start over if so
				if (indexsinusLookupTable == sizeSinusLookupTable) {
					indexsinusLookupTable = 0;
				}

				dacVoltage = sinusLookupTable[indexsinusLookupTable];
				indexsinusLookupTable++;

			}
			//Write a sample to DAC 
			*DAC0_CH0DATA = dacVoltage;
			*DAC0_CH1DATA = dacVoltage;

			//increace the count, to ceep track of note-length
			count++;
		}
	}
