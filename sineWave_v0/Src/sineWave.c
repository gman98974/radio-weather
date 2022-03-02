/* ========================== include files =========================== */
#include <stdint.h>
#include <math.h>
#include "Audio_Drivers.h"

#include "sineWave.h"

#define PBSIZE 4096
#define SINELOOKUPSIZE 1024
#define PI 3.1415926535897

// Initial variables
float freq = 200.0f;
float currentPhase = 0.0f;

int16_t PlayBuff[PBSIZE];
int16_t SineBuff[SINELOOKUPSIZE];

enum eBufferStatus { empty, finished, firstHalfReq, firstHalfDone, secondHalfReq, secondHalfDone } bufferStatus = empty;

// Audio callback function prototypes
void myAudioHalfTransferCallback(void) {
	// First half of buffer is read
	bufferStatus = firstHalfReq;
}

void myAudioTransferCompleteCallback(void) {
	// Entire buffer contents read
	myAudioChangeBuffer(PlayBuff, PBSIZE);
	bufferStatus = secondHalfReq;
}



void flashGreen() {
	// Flashes green LED (PD12) to monitor performance when connected to oscilloscope probe
	// Needs to be adjusted as currently is always on
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 2);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER &= ~GPIO_MODER_MODE12_Msk;
	GPIOD->MODER |= 1 << GPIO_MODER_MODE12_Pos;
}

void SysTick_Handler() {
	// Interrupt handler - not a 100% on this
	static uint32_t count = 0;

	if ((count++ % 2) == 0) {
		GPIOD->BSRR = 0x1 << 12;
	}
	else {
		GPIOD->BSRR = 0x1 << 28;
	}
}

void setup() {
	// Sets up timer, peripherals & DAC and programs chip to run at 168 MHz
	initAudioTimer();
	myAudioSpeedUpTheSystemClock();
	myAudioInitialisePeripherals(OUTPUT_DEVICE_AUTO, 80, AUDIO_FREQUENCY_44K); // Board output, volume, sample rate
	// Output options: OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_AUTO or OUTPUT_DEVICE_BOTH

	SysTick_Handler(); // Need this here otherwise flashGreen() doesn't work

	// Create lookup table and fill with sin values
	for (int i = 0; i <= SINELOOKUPSIZE; i++) {
		float q = 32760 * sin(i * 2.0 * PI / SINELOOKUPSIZE);
		SineBuff[i] = (int16_t)q;
	}

	myAudioStartPlaying(PlayBuff, PBSIZE); // Play buffer contents
}

void loopAudio() {
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


