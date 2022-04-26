#include "chip8.h"


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
