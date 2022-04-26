#include "chip8.h"

/**
 * load ROM file into memory
 * @param chip8 
 * @param filename 
 */
void loadRom(chip8 *chip8, const char *filename)
{
	// open the file
	FILE *rom = fopen(filename, "rb");

	if (rom == NULL)
	{
		printf(stderr, "Failed to read ROM file %s", filename);
		exit(0);
	}

	// get file size
	fseek(rom, 0L, SEEK_END);
	size_t fsize = ftell(rom);
	rewind(rom);

	if (fsize > (PROGRAM_END - PROGRAM_START))
	{
		printf(stderr, "ROM file too large");
		exit(0);
	}
	

	// allocate memory for the rom
	uint8_t *buffer = (uint8_t*) malloc(sizeof(uint8_t) * fsize);

	if (buffer == NULL)
	{
		printf(stderr, "Out of memory");
		exit(0);
	}

	// get file content
	fread(buffer, sizeof(uint8_t), fsize, rom);

	// put file content into memory
	for (size_t i = 0; i < fsize; i++) 
	{
		chip8->ram[i + 0x200] = buffer[i];
	}

	fclose(rom);
	free(buffer);
}

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
 * similar to the initialize function,
 * the RAM is not completely cleared
 * @param chip8 
 */
void resetSystem(chip8 *chip8)
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

	// clear ram between fontset and program ram
	for (int i = FONTSET_START; i < PROGRAM_START; i++)
	{
		chip8->ram[i] = 0;
	}

	// reset registers, fontset and keyboard
	for (int i = 0; i < REGISTER_SIZE; i++)
	{
		chip8->V[i] = 0;
		chip8->ram[i] = FONTSET[i];
		chip8->keyboard[i] = FALSE;
	}
	
	// Set timers to 0
	chip8->delayTimer = 0;
	chip8->soundTimer = 0;
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

/**
 * updates the system timers for the emulator
 * @param chip8 
 */
void updateTimers(chip8 *chip8)
{
	if (chip8->delayTimer > 0)
	{
		chip8->delayTimer--;
	}

	if (chip8->soundTimer > 0)
	{
		chip8->soundTimer--;
	}
}