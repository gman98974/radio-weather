#include <stdint.h>
#include <stdio.h>
#include "sineWave.h"
#include "Audio_Drivers.h"

int main(void)
{
	setup();
	flashGreen();
	while(1) {
		flashGreen();
		loopAudio();
	}
}
