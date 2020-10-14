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

void
setup_DAC ()
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

void
startDAC ()
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
void
stopDAC ()
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
void
startUpSong (int wave)
{

  int sizeVectors = 3;
  int frequencies[] = { Hl, Dl, F };
  int lengthPerfrequency[] = { 60, 60, 60 };

  makeSong (&frequencies, sizeVectors, &lengthPerfrequency, wave);

}

/*------------WAVE SELECT---------------*/

/*
name: updatewave
purpose: changes the value of wave so the microcontroller uses another waveformat when playing songs. It also shows which wave that it uses by ligth up leds accordingly
argument(s): none
return value: none
*/
void
updatewave (int *wave)
{
  //*GPIO_PA_DOUT = 0xffff;
  *wave += 1;
  if (*wave == 4)
    {
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
void
makeSound (int freqency, int length, int wave)
{
  switch (wave)
    {
    case 0:
      makeSound_square (freqency, length);
      break;
    case 1:
      makeSound_saberthoot (freqency, length);
      break;
    case 2:
      makeSound_triangle (freqency, length);
      break;
    case 3:
      makeSound_sinus (freqency, length);
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
void
makeSong (int *frecquencyVector, int sizeVectors, int *lengthFrequencyVector,
	  int wave)
{
  //play the song
  for (int i = 0; i < sizeVectors; i++)
    {
      makeSound (frecquencyVector[i], lengthFrequencyVector[i], wave);
    }
  //make sound to symbolixe the end of the song
  makeSound (frecquencyVector[sizeVectors - 1],
	     lengthFrequencyVector[sizeVectors - 1] * 2, wave);
}


/*-------------SQARE WAVE-------------------*/

void
makeSound_square (int freq, int length)
{
  int count = 1;		//tell antall tiks
  int dacvolt = 0;		//defines strength of sound
  length = length * 1000;	//tonens lengde=spesifisert lengde * 1ms
  while (count < length)
    {
      int countperiod = 44100 / freq;
      if (count % (countperiod / 2) == 0)
	{
	  if (dacvolt == 100)
	    {
	      dacvolt = 0;
	    }
	  else
	    {
	      dacvolt = 100;
	    }
	  *DAC0_CH0DATA = dacvolt;	//skriver voltverdien til utgangen
	  *DAC0_CH1DATA = dacvolt;
	}

      count++;
    }
}


/*---------------SABERTHOOT-------------------*/

void
makeSound_saberthoot (int freq, int length)
{
  int count = 1;
  int dacvolt = 0;
  int countperiod = 44100 / freq;
  int dacUpTime = countperiod / 100;	//calculates how often i must update voltage to swing between 0 and 100
  int rate = 1;
  if (dacUpTime < 1)
    {
      dacUpTime = 1;
      if (freq > 1323)
	{
	  rate = 3;
	}
      else if (freq > 882)
	{
	  rate = 2;
	}
    }
  int dacUpCount = 1;
  length = length * 1000;
  while (count < length)
    {
      if (count % countperiod == 0)
	{
	  dacvolt = 0;
	}
      *DAC0_CH0DATA = dacvolt;
      *DAC0_CH1DATA = dacvolt;
      if (dacUpCount >= dacUpTime)
	{
	  dacvolt = dacvolt + (1 * rate);
	  dacUpCount = 0;
	}
      count++;
      dacUpCount++;
    }
}



/*------------------TRIANGLE WAVE---------------------*/

void
makeSound_triangle (int freq, int length)
{
  int count = 1;
  int dacVolt = 0;
  int dacdir = 1;
  int countperiod = 44100 / freq;
  int dacUpTime = countperiod / 200;
  int rate = 1;
  if (dacUpTime < 1)
    {
      dacUpTime = 1;
      if (freq > 1102)
	{
	  rate = 5;
	}
      else if (freq > 882)
	{
	  rate = 4;
	}
      else if (freq > 662)
	{
	  rate = 3;
	}
      else if (freq > 441)
	{
	  rate = 2;
	}
    }
  int dacUpCount = 0;
  length = length * 1000;
  while (count < length)
    {
      if (count % (countperiod / 2) == 0)
	{
	  dacdir = dacdir * (-1);
	}
      *DAC0_CH0DATA = dacVolt;
      *DAC0_CH1DATA = dacVolt;
      if (dacUpCount >= dacUpTime)
	{
	  dacUpCount = 0;
	  dacVolt = dacVolt + (dacdir * rate);
	  //if (dacVolt>100){
	  //      dacVolt=100;
	  //} else 
	  if (dacVolt < 0)
	    {
	      dacVolt = 0;
	    }
	}
      count++;
      dacUpCount++;
    }
}

/*---------------SINUS WAVE-------------------*/

void
makeSound_sinus (int freq, int length)
{
  int sinusVecLength = 28;
  //int sinusVec[28] = {100, 95, 80, 57, 32, 12, 1, 2, 14, 36, 60, 83, 97};
  int sinusVec[28] =
    { 100, 98, 95, 89, 81, 71, 61, 50, 38, 28, 18, 10, 4, 1, 0, 1, 4,
    10, 18, 28, 38, 49, 61, 71, 81, 89, 95, 98
  };
  int sinus_table_index = 0;
  int count = 1;		//tell antall tiks
  int dacvolt = 0;		//defines strength of sound
  length = length * 1000;	//tonens lengde=spesifisert lengde * 1ms
  while (count < length)
    {
      int countperiod = 44100 / freq;
      if (count % (countperiod / 28) == 0)
	{
	  if (sinus_table_index == sinusVecLength)
	    {
	      sinus_table_index = 0;
	    }
	  dacvolt = sinusVec[sinus_table_index];
	  sinus_table_index++;

	}
	*DAC0_CH0DATA = dacvolt;	//skriver voltverdien til utgangen
	*DAC0_CH1DATA = dacvolt;
	count++;
    }
}
