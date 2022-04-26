#include "instructions.h"


void ZERO(chip8 *chip8)
{
	switch (chip8->opcode)
	{
		case 0x00E0:
			CLS(chip8);
			break;

		case 0x00EE:
			RET(chip8);
			break;
		
		default:
			SYSA(chip8);
			break;
	}
}

void ARITHMETIC(chip8 *chip8)
{
	chip8InstructionsArithmetic[chip8->opcode & 0x000Fu](chip8);
}

void KEYBOARD(chip8 *chip8)
{
	switch (chip8->opcode & 0xFF)
	{
		case 0x9E:
			SKPX(chip8);
			break;

		case 0xA1:
			SKNPX(chip8);
			break;
	}
}

void INDEX(chip8 *chip8)
{
	switch (chip8->opcode & 0xFF)
	{
		case 0x07:
			LDXT(chip8);
			break;

		case 0x0A:
			LDXK(chip8);
			break;

		case 0x15:
			LDTX(chip8);
			break;

		case 0x18:
			LDSX(chip8);
			break;

		case 0x1E:
			ADDIX(chip8);
			break;

		case 0x29:
			LDFX(chip8);
			break;

		case 0x33:
			LDBX(chip8);
			break;

		case 0x55:
			LDIX(chip8);
			break;

		case 0x65:
			LDXI(chip8);
			break;
	}
}
