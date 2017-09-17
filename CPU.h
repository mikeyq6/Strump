#include "alias.h"
#include "opcodes.h"

#ifndef CPU_H
#define CPU_H

#define Z 128
#define N 64
#define H 32
#define C 16

#define FREQ 4194304

#define I_VBlank 	0x40
#define I_LCDC 		0x48
#define I_Timer		0x50
#define I_Serial	0x58
#define I_Joypad	0x60

#define NO_ROMBANK	0x00
#define MBC1		0x01
#define MBC2		0x02
#define MBC3		0x04
#define MBC5		0x08
#define MM01		0x10

#define INTERNAL_ROM_SIZE 256
#define CARTRIDGE_SIZE 0x200000

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

typedef struct _cartridgeInfo {
	char title[17];
	uint8_t gbc;
	uint8_t sgb;
	uint8_t type;
	char *typeName;
	uint8_t romType;
	char *romTypeName;
	uint8_t ramType;
	char *ramTypeName;
	uint8_t destinationCode;
	uint8_t licenseeCode;
	uint8_t controllerType;
	uint8_t hasBattery;
	uint8_t hasRumble;
	uint8_t hasTimer;
	uint8_t hasSRAM;
	
} CartridgeInfo;

CartridgeInfo *CartInfo;

// Init
void initCPU();
void Start();
void Quit();
void SetCartridgeInfo();
void DisplayCartridgeInfo();

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
void Run(uint8_t opcode, uint8_t param1, uint8_t param2);
void RunCBInstruction(uint8_t opcode);

void OR(uint8_t opcode, uint8_t param);
void XOR(uint8_t opcode, uint8_t param);
void AND(uint8_t opcode, uint8_t param);
void ADD(uint8_t opcode, uint8_t param);
void ADC(uint8_t opcode, uint8_t param);
void SUB(uint8_t opcode, uint8_t param);
void SBC(uint8_t opcode, uint8_t param);
void CP(uint8_t opcode, uint8_t param);
void LD(uint8_t opcode, uint8_t param, uint8_t param2);
void DEC(uint8_t opcode);
void INC(uint8_t opcode);
void JR(uint8_t opcode, uint8_t param1, uint8_t param2);
void CALL(uint8_t opcode, uint8_t param1, uint8_t param2);
void RET_(uint8_t opcode);
void DAA_();
void RRCA_();
void RRA_();
void RLCA_();
void RLA_();

void PUSH(uint8_t opcode);
void POP(uint8_t opcode);
void RRC(uint8_t opcode);
void RLC(uint8_t opcode);
void RL(uint8_t opcode);
void RR(uint8_t opcode);
void RES(uint8_t opcode, uint8_t bit_n);
void SET(uint8_t opcode, uint8_t bit_n);
void BIT(uint8_t opcode, int bit_n);
void RST(uint8_t opcode);
void SWAP(uint8_t opcode);
void SRL(uint8_t opcode);
void SRA(uint8_t opcode);
void SLA(uint8_t opcode);

const char* CodeToString(uint8_t opcode);
const char* CBCodeToString(uint8_t opcode);

#endif /* CPU_H */