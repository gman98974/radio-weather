#include <stdint.h>
#include <math.h>
#include "Audio_Drivers.h"
#include "stm32f407xx.h" // Uncomment?

#define PBSIZE 4096
int16_t PlayBuff[PBSIZE];
#define SINELOOKUPSIZE 1024
int16_t SineBuff[SINELOOKUPSIZE];
#define PI 3.1415926535897

enum eBufferStatus { empty, finished, firstHalfReq, firstHalfDone, secondHalfReq, secondHalfDone } bufferStatus = empty;

//float freq = 300;
void flashGreen() {
	// Flashes green LED (PD12) to monitor performance
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 2);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER &= ~GPIO_MODER_MODE12_Msk;
	GPIOD->MODER |= 1 << GPIO_MODER_MODE12_Pos;
}

void setup() {
	// Sets up timer, peripherals & DAC and programs chip to run at 168 MHz
	initAudioTimer();
	myAudioSpeedUpTheSystemClock();
	myAudioInitialisePeripherals(OUTPUT_DEVICE_AUTO, 80, AUDIO_FREQUENCY_44K); // Board output, volume, sample rate
	// Output options: OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_AUTO or OUTPUT_DEVICE_BOTH

	// Create lookup table and fill with sin values
	for (int i = 0; i <= SINELOOKUPSIZE; i++) {
		float q = 32760 * sin(i * 2.0 * PI / SINELOOKUPSIZE);
		SineBuff[i] = (int16_t)q;
	}

	myAudioStartPlaying(PlayBuff, PBSIZE); // Play buffer contents
}


void loopAudio(float freq) {
	// Fill buffer with

	float currentPhase = 0.0f;
	float phaseInc = SINELOOKUPSIZE * freq / AUDIO_FREQUENCY_44K;

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

// Audio callback functions
void myAudioHalfTransferCallback(void) {
	// First half of buffer is read
	bufferStatus = firstHalfReq;
}

void myAudioTransferCompleteCallback(void) {
	// Entire buffer contents read
	myAudioChangeBuffer(PlayBuff, PBSIZE);
	bufferStatus = secondHalfReq;
}
