#include "alias.h"
#include "opcodes.h"

#ifndef CPU_H
#define CPU_H

#define Z 128
#define N 64
#define H 32
#define C 16

#define ROM_PATH ".\\DMG_ROM.bin"
#define INTERNAL_ROM_SIZE 256

uint8_t Memory[0x10000];
uint8_t Halted, Stopped, WillDisableInterrupts, WillEnableInterrupts;

// Registers
union {
	struct {
		uint8_t f;
		uint8_t a;
	};
	uint16_t af;
} AF;
union {
	struct {
		uint8_t c;
		uint8_t b;
	};
	uint16_t bc;
} BC;
union {
	struct {
		uint8_t e;
		uint8_t d;
	};
	uint16_t de;
} DE;
union {
	struct {
		uint8_t l;
		uint8_t h;
	};
	uint16_t hl;
} HL;

register uint16_t SP;
register uint16_t PC;

// Init
void initCPU();
void Start();

// Flags
uint8_t getFlag(uint8_t flag);
void setFlag(uint8_t flag);
void resetFlag(uint8_t flag);

uint8_t ReadMem(uint16_t location);
void WriteMem(uint16_t location, uint8_t value);

// Instructions
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
void SWAP(uint8_t opcode);
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
void RES(uint8_t opcode, uint8_t bit_n);
void SET(uint8_t opcode, uint8_t bit_n);
void BIT(uint8_t opcode, int bit_n);
void RST(uint8_t opcode);

const char* CodeToString(uint8_t opcode);
const char* CBCodeToString(uint8_t opcode);

#endif /* CPU_H */