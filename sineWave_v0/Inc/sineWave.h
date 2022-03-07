#ifndef SINEWAVE_H_
#define SINEWAVE_H_

// Audio callbacks
void myAudioHalfTransferCallback(void);
void mmyAudioTransferCompleteCallback(void);

// Buffer setup & playback functions
void setup(void);
void loopAudio(void);

// Functions used to monitor performance
void flashGreen(void);
void SysTick_Handler(void);

#endif
