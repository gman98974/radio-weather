#include <stdint.h>
#include <stdio.h>
#include "Audio_Drivers.h"

#include "sineWave.h"



int main(void)
{
	setup();
	flashGreen(); // Not functioning correctly

	while(1) {
		loopAudio();
	}
}
