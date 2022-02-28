#ifndef SINEWAVE_H_
#define SINEWAVE_H_

// Audio callbacks
void myAudioHalfTransferCallback(void);
void mmyAudioTransferCompleteCallback(void);

// Buffer setup & playback functions
void setup(void);
void loopAudio(void);
void flashGreen(void);

#endif
