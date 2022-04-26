#include "instructions.h"

/**
 * Do nothing. 
 */
void NOP(chip8 *chip8)
{}

/**
 * 0x0NNN:
 * Jump to a machine code routine at nnn.
 * 
 * This instruction is only used on the old computers 
 * on which Chip-8 was originally implemented. 
 * It is ignored by modern interpreters.
 */
void SYSA(chip8 *chip8)
{}

/**
 * 0x00E0: 
 * Clear the display.
 */
void CLS(chip8 *chip8)
{
	for (int i = 0; i < VIDEO_HEIGHT; i++) {
		for (int j = 0; j < VIDEO_WIDTH; j++) {
			chip8->gfx[i][j] = 0;
		}
	}

	chip8->drawingScreen = TRUE;
	chip8->pc += 2;
}

/**
 * 0x00EE: 
 * Return from a subroutine.
 */
void RET(chip8 *chip8)
{
	chip8->sp--;
	chip8->pc = chip8->stack[chip8->sp];
	chip8->pc += 2;
}

/**
 * 0x1NNN: 
 * Jump to location nnn.
 */
void JP(chip8 *chip8)
{
	uint16_t address = chip8->opcode & 0x0FFF;

	chip8->pc = address;
}

/**
 * 0x2NNN: 
 * Call subroutine at nnn.
 */
void CALL(chip8 *chip8)
{
	uint16_t address = chip8->opcode & 0x0FFF;

	chip8->stack[chip8->sp] = chip8->pc;
	chip8->sp++;
	chip8->pc = address;
}

/**
 * 0x3XNN: 
 * Skip next instruction if Vx == nn.
 */
void SEXB(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	if (chip8->V[Vx] == byte)
	{
		chip8->pc += 4;
	}
	else
	{
		chip8->pc += 2;
	}
}

/**
 * 0x4XNN: 
 * Skip next instruction if Vx != nn.
 */
void SNEXB(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	if (chip8->V[Vx] != byte)
	{
		chip8->pc += 4;
	}
	else
	{
		chip8->pc += 2;
	}
}

/**
 * 0x5XY0: 
 * Skip next instruction if Vx == Vy.
 */
void SEXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->V[Vx] == chip8->V[Vy])
	{
		chip8->pc += 4;
	}
	else
	{
		chip8->pc += 2;
	}
}

/**
 * 0x6XNN: 
 * Set Vx = nn.
 */
void LDXB(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	chip8->V[Vx] = byte;
	chip8->pc += 2;
}

/**
 * 0x7XNN: 
 * Set Vx += nnn.
 */
void ADDXB(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;

	chip8->V[Vx] += byte;
	chip8->pc += 2;
}

/**
 * 0x8XY0: 
 * Set Vx = Vy.
 */
void LDXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->V[Vx] += chip8->V[Vy];
	chip8->pc += 2;
}

/**
 * 0x8XY1: 
 * Set Vx = Vx OR Vy.
 */
void ORXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->V[Vx] |= chip8->V[Vy];
	chip8->pc += 2;
}

/**
 * 0x8XY2: 
 * Set Vx = Vx AND Vy.
 */
void ANDXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->V[Vx] &= chip8->V[Vy];
	chip8->pc += 2;
}

/**
 * 0x8XY3: 
 * Set Vx = Vx XOR Vy.
 */
void XORXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	chip8->V[Vx] ^= chip8->V[Vy];
	chip8->pc += 2;
}

/**
 * 0x8XY4: 
 * Set Vx = Vx + Vy. 
 * Set VF = carry.
 */
void ADDXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;
	uint16_t sum = (chip8->V[Vx] + chip8->V[Vy]);

	if (sum > 255)
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}
	
	chip8->V[Vx] = (sum & 0xFF);
	chip8->pc += 2;
}

/**
 * 0x8XY5: 
 * Set Vx = Vx - Vy. 
 * set VF = NOT borrow.
 */
void SUBXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->V[Vx] > chip8->V[Vy])
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}

	chip8->V[Vx] -= chip8->V[Vy];
	chip8->pc += 2;
}

/**
 * 0x8XY6: 
 * Set Vx = Vx SHR 1.
 */
void SHRX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	// check if the LSb is 1 (odd number in V[X] will have a LSB of 1) 
	if (chip8->V[Vx] % 2 == 1)
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}

	chip8->V[Vx] = (chip8->V[Vx] >> 1);
	chip8->pc += 2;
}

/**
 * 0x8XY7: 
 * Set Vx = Vy - Vx. 
 * Set VF = NOT borrow.
 */
void SUBNXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->V[Vy] > chip8->V[Vx])
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}

	chip8->V[Vx] = (chip8->V[Vy] - chip8->V[Vx]);
	chip8->pc += 2;
}

/**
 * 0x8XYE: 
 * Set Vx = Vx SHL 1.
 */
void SHLX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	// check if the msb is 1
	if ((chip8->V[Vx] & 0b10000000) == 1)
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}

	chip8->V[Vx] = (chip8->V[Vx] << 1);
	chip8->pc += 2;
}

/**
 * 0x9XY0: 
 * Skip next instruction if Vx != Vy.
 */
void SNEXY(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;

	if (chip8->V[Vx] != chip8->V[Vx])
	{
		chip8->pc += 4;
	}
	else
	{
		chip8->pc += 2;
	}
}

/**
 * 0xANNN: 
 * Set I = nnn.
 */
void LDIA(chip8 *chip8)
{
	uint16_t address = chip8->opcode & 0x0FFF;

	chip8->I = address;
	chip8->pc += 2;
}

/**
 * 0xBNNN: 
 * Jump to location nnn + V0.
 */
void JPVA(chip8 *chip8)
{
	uint16_t address = chip8->opcode & 0x0FFF;

	chip8->pc = (address + chip8->V[0]);
}

/**
 * 0xCXNN: 
 * Set Vx = random byte AND nn.
 */
void RNDXB(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t byte = chip8->opcode & 0x00FF;
	uint8_t random = rand() % 256;

	chip8->V[Vx] = random & byte;
	chip8->pc += 2;
}

/**
 * 0xDXYN: 
 * Display n-byte sprite starting at memory location I at (Vx, Vy). 
 * Set VF = collision.
 * Initial source of implimentation used as template found below:
 * http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
 */
void DRWXYN(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;
	uint8_t height = chip8->opcode & 0x000F;
	uint8_t positionX = chip8->V[Vx];
	uint8_t positionY = chip8->V[Vy];
	uint8_t pixel;

	// Reset collision register to FALSE
	chip8->V[0xF] = FALSE;
	
	for (int offsetY = 0; offsetY < height; offsetY++)
	{
		pixel = chip8->ram[chip8->I + offsetY];

		for (int offsetX = 0; offsetX < 8; offsetX++)
		{
			if ((pixel & (0x80 >> offsetX)) != 0)
			{
				if (chip8->gfx[positionY + offsetY][positionX + offsetX] == 1)
				{
					chip8->V[0xF] = TRUE;
				}

				chip8->gfx[positionY + offsetY][positionX + offsetX] ^= 1;
			}
		}
	}

	chip8->drawingScreen = TRUE;
	chip8->pc += 2;
}

/**
 * 0xEX9E: 
 * Skip next instruction if key with the value of Vx is pressed.
 */
void SKPX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t keyValue = chip8->V[Vx];

	if (chip8->keyboard[keyValue] != FALSE)
	{
		chip8->pc += 4;
	}
	else
	{
		chip8->pc += 2;
	}
}

/**
 * 0xEXA1: 
 * Skip next instruction if key with the value of Vx is not pressed.
 */
void SKNPX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
	uint8_t keyValue = chip8->V[Vx];

	if (chip8->keyboard[keyValue] == FALSE)
	{
		chip8->pc += 4;
	}
	else
	{
		chip8->pc += 2;
	}
}

/**
 * 0xFX07: 
 * Set Vx = delay timer value.
 */
void LDXT(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->V[Vx] = chip8->delayTimer;
	chip8->pc += 2;
}

/**
 * 0xFX0A: 
 * Wait for a key press, store the value of the key in Vx.
 */
void LDXK(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->keyPressed = FALSE;

	for (int i = 0; i < NUMBER_KEYS; i++)
	{
		if (chip8->keyboard[i] != FALSE)
		{
			chip8->V[Vx] = i;
			chip8->keyPressed = TRUE;
		}
	}

	if (chip8->keyPressed)
	{
		return;
	}

	chip8->pc += 2;
}

/**
 * 0xFX15: 
 * Set delay timer = Vx.
 */
void LDTX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->delayTimer = chip8->V[Vx];
	chip8->pc += 2;
}

/**
 * 0xFX18: 
 * Set sound timer = Vx.
 */
void LDSX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->soundTimer = chip8->V[Vx];
	chip8->pc += 2;
}

/**
 * 0xFX1E: 
 * Set I = I + Vx.
 */
void ADDIX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->I += chip8->V[Vx];
	chip8->pc += 2;
}

/**
 * 0xFX29: 
 * Set I = location of sprite for digit Vx.
 */
void LDFX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->I = (chip8->V[Vx] * 0x5);
	chip8->pc += 2;
}

/**
 * 0xFX33: 
 * Store BCD representation of Vx in memory locations I, I+1, and I+2.
 * Used this for page for a hint on this op implementation:
 * http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
 */
void LDBX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	chip8->ram[chip8->I] = chip8->V[Vx] / 100;                 // MSb
	chip8->ram[chip8->I + 1] = (chip8->V[Vx] / 10) % 10;
	chip8->ram[chip8->I + 2] = (chip8->V[Vx] % 100) % 10;      // LSb
	chip8->pc += 2;
}

/**
 * 0xFX55: 
 * Store registers V0 through Vx in memory starting at location I.
 */
void LDIX(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	for (int i = 0; i <= Vx; i++)
	{
		chip8->ram[chip8->I + i] = chip8->V[i];
	}

	chip8->pc += 2;
}

/**
 * 0xFX65: 
 * Read registers V0 through Vx from memory starting at location I.
 */
void LDXI(chip8 *chip8)
{
	uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;

	for (int i = 0; i <= Vx; i++)
	{
		chip8->V[i] = chip8->ram[chip8->I + i];
	}

	chip8->pc += 2;
}


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
