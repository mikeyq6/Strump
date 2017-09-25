#include "alias.h"
#include "opcodes.h"

#ifndef CPU_H
#define CPU_H

#define Z 128
#define N 64
#define H 32
#define C 16

#define FREQ 4194304
#define SCOUNTER_HZ 5000

#define I_VBlank 	0x40
#define I_LCDC 		0x48
#define I_Timer		0x50
#define I_Serial	0x58
#define I_Joypad	0x60

#define INTERNAL_ROM_SIZE 256
#define CARTRIDGE_SIZE 0x200000

#define WHITE		0x00ffffff
#define LT_GRAY		0x00555555
#define DK_GRAY		0x00cccccc
#define BLACK		0x00000000

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

// If not Windows, then don't use scanf_s
#ifndef _WIN32
#define scanf_s scanf
#endif

uint8_t Memory[0x10000];
uint8_t Cartridge[CARTRIDGE_SIZE];
uint8_t Halted, Stopped, WillDisableInterrupts, WillEnableInterrupts, Startup, InterruptsEnabled;
uint8_t RomBank, RamBank, RamEnabled;

// Registers
register union {
	struct {
		uint8_t f;
		uint8_t a;
	};
	uint16_t af;
} AF;
register union {
	struct {
		uint8_t c;
		uint8_t b;
	};
	uint16_t bc;
} BC;
register union {
	struct {
		uint8_t e;
		uint8_t d;
	};
	uint16_t de;
} DE;
register union {
	struct {
		uint8_t l;
		uint8_t h;
	};
	uint16_t hl;
} HL;

register uint16_t SP;
register uint16_t PC;
register uint16_t rDiv;
register int32_t sCounter;

// Init
void initCPU();
void Start();
void Quit();

// Flags
uint8_t getFlag(uint8_t flag);
void setFlag(uint8_t flag);
void resetFlag(uint8_t flag);

// Memory
uint8_t ReadMem(uint16_t location);
void WriteMem(uint16_t location, uint8_t value);

// Interrupts & Timers
void CheckInterrupts();
void SetInterrupt(uint8_t iRegister);
void ResetInterrupt(uint8_t iRegister);
void UpdateTimer(uint8_t opcode);

// Instructions
uint8_t GetNextInstruction();
uint8_t GetValueAt(uint16_t address);
void Run(uint8_t opcode, uint8_t param1, uint8_t param2);
void RunCBInstruction(uint8_t opcode);

// LCDC 
uint16_t GetBackgroundTileMapLocation();
uint16_t BGWindowTileLocation();
uint32_t GetColourFor(uint8_t number);
uint8_t IsLCDEnabled();
void SetLCDStatus();


#endif /* CPU_H */