
#include "chip8.h"

#define CPU_CLOCK_DELAY 1000            // 1 millisecond
#define TIMER_CLOCK_DIVISION 9

int main(int argc, char const *argv[])
{
	chip8 chip8;

	int divisionCycles = 0;
	int totalCycles = 0;

	// creates a buffer to store the pixel status for the emulator screen
	uint32_t *pixelBuffer = malloc((VIDEO_SIZE) * sizeof(uint32_t));

	initializeSystem(&chip8);
	loadRom(&chip8, argv[1]);

	while(chip8.isRunning)
	{
		executeInstruction(&chip8);
		divisionCycles++;
		totalCycles++;

		if (divisionCycles == TIMER_CLOCK_DIVISION) {
            updateTimers(&chip8);
            divisionCycles = 0;
        }

		do
		{
			//process user input
		}
		while (chip8.isPaused && chip8.isRunning);


		usleep(CPU_CLOCK_DELAY);
	}

    free(pixelBuffer);

	return 0;
}
