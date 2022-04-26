#ifndef CHIP8
#define CHIP8

#include <stdint.h>
#include <stdio.h>

#include "instructions.h"

#define RAM_SIZE 4096
#define STACK_SIZE 16
#define REGISTER_SIZE 16

#define VIDEO_HEIGHT 32
#define VIDEO_WIDTH 64

#define NUMBER_KEYS 16

#define TRUE 1
#define FALSE 0


typedef struct chip8 chip8;


const uint8_t FONTSET[FONTSET_SIZE] =
{ 
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct chip8
{
	// memory
	uint8_t ram[RAM_SIZE]; 			// 4kB of memory
	uint16_t stack[STACK_SIZE];		// 16 levels of stack

	// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	// 0x200-0xFFF - Program ROM and work RAM

	// register
	uint8_t V[REGISTER_SIZE];	// general purpose register (V0 - VE), carry flag register (VF)
	uint16_t I;					// index register 
	uint16_t pc;				// program counter register
	uint16_t sp;				// stack pointer register

	// timers
	uint8_t delayTimer;
	uint8_t soundTimer;

	uint16_t opcode;	// current opcode

	// screen
	uint8_t gfx[VIDEO_HEIGHT][VIDEO_WIDTH];

	// keys
	uint8_t keyboard[NUMBER_KEYS];
	uint8_t keyPressed;

	// emulator status flags
	uint8_t isRunning;
	uint8_t drawingScreen;
	uint8_t isPaused;
};

void initializeSystem(chip8 *chip8);
void resetSystem(chip8 *chip8);
uint16_t fetchOpcode(chip8 *chip8);
void executeInstruction(chip8 *chip8);
#endif // CHIP8