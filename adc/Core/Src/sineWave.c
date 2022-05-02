#include <stdint.h>
#include <math.h>
#include "Audio_Drivers.h"
#include "stm32f407xx.h"
#include "sineWave.h"

#define PBSIZE 4096					// Size of playback buffer.
int16_t PlayBuff[PBSIZE];			// Defined as array of 16 bit integers.
#define SINELOOKUPSIZE 1024			// Size of buffer used to store sine wave.
int16_t SineBuff[SINELOOKUPSIZE];	// Same type as audio buffer.
#define PI 3.1415926535897

// Buffer status states.
enum eBufferStatus { empty, finished, firstHalfReq, firstHalfDone, secondHalfReq, secondHalfDone } bufferStatus = empty;

// Variables initialised with their starting values.
float currentPhase = 0.0f;
float volume = 80;


/*
 * 	Function definitions.
 */

// Used to monitor performance.
// Attach oscilloscope probe to green LED pin (PD12) and monitor how
// quickly the board generates new samples.
void flashGreen() {
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 2);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER &= ~GPIO_MODER_MODE12_Msk;
	GPIOD->MODER |= 1 << GPIO_MODER_MODE12_Pos;
}

void setup() {
	// Sets up timer, peripherals & DAC and programs chip to run at 168 MHz.
	initAudioTimer();
	myAudioSpeedUpTheSystemClock();
	myAudioInitialisePeripherals(OUTPUT_DEVICE_AUTO, volume, AUDIO_FREQUENCY_44K); // Board output, volume, sample rate
	// Output options: OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_AUTO or OUTPUT_DEVICE_BOTH

	// Fill previously created sine buffer with appropriate values.
	for (int i = 0; i <= SINELOOKUPSIZE; i++) {
		float q = 32760 * sin(2.0 * PI * i / SINELOOKUPSIZE);
		SineBuff[i] = (int16_t)q;
	}

	myAudioStartPlaying(PlayBuff, PBSIZE); // Play current buffer contents.
}


void loopAudio(float frequency) {
	float phaseInc = SINELOOKUPSIZE * frequency / AUDIO_FREQUENCY_44K;

	// If there's been a request to fill half of the buffer,
	// then set the start and end points to fill:
	uint32_t startFill = 0, endFill = 0;
	if (bufferStatus == firstHalfReq) {
		startFill = 0;
		endFill = PBSIZE / 2;
		bufferStatus = firstHalfDone;
	}
	else if (bufferStatus == secondHalfReq) {
		startFill = PBSIZE / 2;
		endFill = PBSIZE;
		bufferStatus = secondHalfDone;
	}

	if (startFill != endFill) {
		for (int i = startFill; i < endFill; i += 2) {
			currentPhase += phaseInc;
			if (currentPhase > SINELOOKUPSIZE) currentPhase -= SINELOOKUPSIZE;
			int16_t nextSample = SineBuff[(uint16_t)(currentPhase)];
			PlayBuff[i] = nextSample;
			PlayBuff[i + 1] = nextSample;
		}
	}
}

// Audio callback functions.
// First half of buffer has been read.
void myAudioHalfTransferCallback(void) {
	bufferStatus = firstHalfReq; 			// DMA has read first half, request ARM to update first half with new samples.
}

// Second half of buffer has been read.
void myAudioTransferCompleteCallback(void) {
	myAudioChangeBuffer(PlayBuff, PBSIZE);	// Set DMA to begin reading from beginning of buffer.
	bufferStatus = secondHalfReq; 			// DMA has read second half, request ARM to update second half with new samples.
}
