#ifndef CHIP8_INSTRUCTIONS
#define CHIP8_INSTRUCTIONS

#include "chip8.h"

void NOP(chip8 *chip8);
void SYSA(chip8 *chip8);
void CLS(chip8 *chip8);
void RET(chip8 *chip8);
void JP(chip8 *chip8);
void CALL(chip8 *chip8);
void SEXB(chip8 *chip8);
void SNEXB(chip8 *chip8);
void SEXY(chip8 *chip8);
void LDXB(chip8 *chip8);
void ADDXB(chip8 *chip8);

void LDXY(chip8 *chip8);
void ORXY(chip8 *chip8);
void ANDXY(chip8 *chip8);
void XORXY(chip8 *chip8);
void ADDXY(chip8 *chip8);
void SUBXY(chip8 *chip8);
void SHRX(chip8 *chip8);
void SUBNXY(chip8 *chip8);
void SHLX(chip8 *chip8);
void SNEXY(chip8 *chip8);

void LDIA(chip8 *chip8);
void JPVA(chip8 *chip8);
void RNDXB(chip8 *chip8);
void DRWXYN(chip8 *chip8);
void SKPX(chip8 *chip8);
void SKNPX(chip8 *chip8);
void LDXT(chip8 *chip8);
void LDXK(chip8 *chip8);
void LDTX(chip8 *chip8);
void LDSX(chip8 *chip8);
void ADDIX(chip8 *chip8);
void LDFX(chip8 *chip8);
void LDBX(chip8 *chip8);
void LDIX(chip8 *chip8);
void LDXI(chip8 *chip8);

void ZERO(chip8 *chip8);
void ARITHMETIC(chip8 *chip8);
void KEYBOARD(chip8 *chip8);
void INDEX(chip8 *chip8);

void (*chip8Instructions[16])(chip8 *chip8);
void (*chip8InstructionsArithmetic[16])(chip8 *chip8);

const void *chip8Instructions[16] = 
{
	ZERO,
	JP,		CALL,		SEXB,	SNEXB, 
	SEXY,	LDXB,		ADDXB,	ARITHMETIC, 
	SNEXY,	LDIA,		JPVA,	RNDXB, 
	DRWXYN, KEYBOARD,	INDEX
};

const void *chip8InstructionsArithmetic[16] = 
{
	LDXY, 
	ORXY,	ANDXY,	XORXY,	ADDXY, 
	SUBXY,	SUBXY,	SHRX,	SUBNXY, 
	SHLX,	NOP,	NOP,	NOP, 
	NOP,	NOP,	SNEXY
};

#endif // CHIP8_INSTRUCTIONS