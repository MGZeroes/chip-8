#include "chip8.h"

/**
 * initilize the system to its startup state
 * @param chip8 
 */
void initializeSystem(chip8 *chip8)
{
	chip8->isRunning = TRUE;
	chip8->drawingScreen = FALSE;
	chip8->isPaused = FALSE;

	chip8->pc = PROGRAM_START;
	chip8->opcode = 0;
	chip8->sp = 0;
	chip8->I = 0;

	// clear display (memory)
	for (int i = 0; i < VIDEO_HEIGHT; i++)
	{
		for (int j = 0; j < VIDEO_WIDTH; j++)
		{
			chip8->gfx[i][j] = 0;
		}
	}

	// clear ram
	for (int i = 0; i < RAM_SIZE; i++)
	{
		chip8->ram[i] = 0;
	}

	// clear registers
	for (int i = 0; i < REGISTER_SIZE; i++)
	{
		chip8->V[i] = 0;
	}

	// load fontset into memory
	for(int i = 0; i < FONTSET_SIZE; i++)
	{
		chip8->ram[i] = FONTSET[i];
	}
	
	// set timers to 0
	chip8->delayTimer = 0;
	chip8->soundTimer = 0;

	// keyboard setup
	for (int i = 0; i < NUMBER_KEYS; i++)
	{
		chip8->keyboard[i] = FALSE;
	}

	chip8->keyPressed = FALSE;
}


/**
 * fetches the current opcode (opcode is 2 bytes)
 * @param chip8 
 * @return uint16_t 
 */
uint16_t fetchOpcode(chip8 *chip8)
{
	uint16_t opcode;
	uint8_t msb = chip8->ram[chip8->pc];
	uint8_t lsb = chip8->ram[chip8->pc + 1];

	opcode = msb << 8 | lsb;

	return opcode;
}

/**
 * calls the instruction to execute based on the fetched opcode
 * @param chip8 
 */
void executeInstruction(chip8 *chip8)
{
	uint16_t opcode = fetchOpcode(chip8);
	chip8->opcode = opcode;

	chip8Instructions[(opcode & 0xF000u)](chip8);
}
