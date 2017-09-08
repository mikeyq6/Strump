#include <stdio.h>
#include <conio.h>

#include "CPU.h"
#include "test.h"

#define RUNTESTS
#define STEPTHROUGH 1

#ifdef RUNTESTS
void TestInstructions();
#endif
#ifdef STEPTHROUGH
void DisplayState();
int skip = 0;
int skipuntil = 0x100;
#endif

void readInitialProgram();
void setDefaults();
void runInitialProgram();
short GetParameters(uint8_t opcode);
uint16_t GetCycles(opcode);

// Internal startup ROM
uint8_t InternalRom[INTERNAL_ROM_SIZE] = { 0x31,0xFE,0xFF,0xAF,0x21,0xFF,0x9F,0x32,0xCB,0x7C,0x20,0xFB,0x21,0x26,0xFF,0x0E,0x11,0x3E,0x80,0x32,0xE2,0x0C,0x3E,0xF3,0xE2,0x32,0x3E,0x77,0x77,0x3E,0xFC,0xE0,0x47,0x11,0x04,0x01,0x21,0x10,0x80,0x1A,0xCD,0x95,0x00,0xCD,0x96,0x00,0x13,0x7B,0xFE,0x34,0x20,0xF3,0x11,0xD8,0x00,0x06,0x08,0x1A,0x13,0x22,0x23,0x05,0x20,0xF9,0x3E,0x19,0xEA,0x10,0x99,0x21,0x2F,0x99,0x0E,0x0C,0x3D,0x28,0x08,0x32,0x0D,0x20,0xF9,0x2E,0x0F,0x18,0xF3,0x67,0x3E,0x64,0x57,0xE0,0x42,0x3E,0x91,0xE0,0x40,0x04,0x1E,0x02,0x0E,0x0C,0xF0,0x44,0xFE,0x90,0x20,0xFA,0x0D,0x20,0xF7,0x1D,0x20,0xF2,0x0E,0x13,0x24,0x7C,0x1E,0x83,0xFE,0x62,0x28,0x06,0x1E,0xC1,0xFE,0x64,0x20,0x06,0x7B,0xE2,0x0C,0x3E,0x87,0xE2,0xF0,0x42,0x90,0xE0,0x42,0x15,0x20,0xD2,0x05,0x20,0x4F,0x16,0x20,0x18,0xCB,0x4F,0x06,0x04,0xC5,0xCB,0x11,0x17,0xC1,0xCB,0x11,0x17,0x05,0x20,0xF5,0x22,0x23,0x22,0x23,0xC9,0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E,0x3C,0x42,0xB9,0xA5,0xB9,0xA5,0x42,0x3C,0x21,0x04,0x01,0x11,0xA8,0x00,0x1A,0x13,0xBE,0x20,0xFE,0x23,0x7D,0xFE,0x34,0x20,0xF5,0x06,0x19,0x78,0x86,0x23,0x05,0x20,0xFB,0x86,0x20,0xFE,0x3E,0x01,0xE0,0x50 };

//uint8_t InternalRom[0x330] = { 0x00, 0x01, 0x00, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x07, 0x08, 0x00, 0x00, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x00, 0x0F, 0x10, 0x00, 0x11, 0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x16, 0x00, 0x17, 0x18, 0x00, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x00, 0x1F, 0x20, 0x00, 0x21, 0x00, 0x00, 0x22, 0x23, 0x24, 0x25, 0x26, 0x00, 0x27, 0x28, 0x00, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x00, 0x2F, 0x30, 0x00, 0x31, 0x00, 0x00, 0x32, 0x33, 0x34, 0x35, 0x36, 0x00, 0x37, 0x38, 0x00, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x00, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0x00, 0x00, 0xC3, 0x00, 0x00, 0xC4, 0x00, 0x00, 0xC5, 0xC6, 0x00, 0xC7, 0xC8, 0xC9, 0xCA, 0x00, 0x00, 0xCC, 0x00, 0x00, 0xCD, 0x00, 0x00, 0xCE, 0x00, 0xCF, 0xD0, 0xD1, 0xD2, 0x00, 0x00, 0xD4, 0x00, 0x00, 0xD5, 0xD6, 0x00, 0xD7, 0xD8, 0xD9, 0xDA, 0x00, 0x00, 0xDC, 0x00, 0x00, 0xDE, 0x00, 0xDF, 0xE0, 0x00, 0xE1, 0xE2, 0xE5, 0xE6, 0x00, 0xE7, 0xE8, 0x00, 0xE9, 0xEA, 0x00, 0x00, 0xEE, 0x00, 0xEF, 0xF0, 0x00, 0xF1, 0xF2, 0xF3, 0xF5, 0xF6, 0x00, 0xF7, 0xF8, 0x00, 0xF9, 0xFA, 0x00, 0x00, 0xFB, 0xFE, 0x00, 0xFF, 0xCB, 0x00, 0xCB, 0x01, 0xCB, 0x02, 0xCB, 0x03, 0xCB, 0x04, 0xCB, 0x05, 0xCB, 0x06, 0xCB, 0x07, 0xCB, 0x08, 0xCB, 0x09, 0xCB, 0x0A, 0xCB, 0x0B, 0xCB, 0x0C, 0xCB, 0x0D, 0xCB, 0x0E, 0xCB, 0x0F, 0xCB, 0x10, 0xCB, 0x11, 0xCB, 0x12, 0xCB, 0x13, 0xCB, 0x14, 0xCB, 0x15, 0xCB, 0x16, 0xCB, 0x17, 0xCB, 0x18, 0xCB, 0x19, 0xCB, 0x1A, 0xCB, 0x1B, 0xCB, 0x1C, 0xCB, 0x1D, 0xCB, 0x1E, 0xCB, 0x1F, 0xCB, 0x20, 0xCB, 0x21, 0xCB, 0x22, 0xCB, 0x23, 0xCB, 0x24, 0xCB, 0x25, 0xCB, 0x26, 0xCB, 0x27, 0xCB, 0x28, 0xCB, 0x29, 0xCB, 0x2A, 0xCB, 0x2B, 0xCB, 0x2C, 0xCB, 0x2D, 0xCB, 0x2E, 0xCB, 0x2F, 0xCB, 0x30, 0xCB, 0x31, 0xCB, 0x32, 0xCB, 0x33, 0xCB, 0x34, 0xCB, 0x35, 0xCB, 0x36, 0xCB, 0x37, 0xCB, 0x38, 0xCB, 0x39, 0xCB, 0x3A, 0xCB, 0x3B, 0xCB, 0x3C, 0xCB, 0x3D, 0xCB, 0x3E, 0xCB, 0x3F, 0xCB, 0x40, 0xCB, 0x41, 0xCB, 0x42, 0xCB, 0x43, 0xCB, 0x44, 0xCB, 0x45, 0xCB, 0x46, 0xCB, 0x47, 0xCB, 0x48, 0xCB, 0x49, 0xCB, 0x4A, 0xCB, 0x4B, 0xCB, 0x4C, 0xCB, 0x4D, 0xCB, 0x4E, 0xCB, 0x4F, 0xCB, 0x50, 0xCB, 0x51, 0xCB, 0x52, 0xCB, 0x53, 0xCB, 0x54, 0xCB, 0x55, 0xCB, 0x56, 0xCB, 0x57, 0xCB, 0x58, 0xCB, 0x59, 0xCB, 0x5A, 0xCB, 0x5B, 0xCB, 0x5C, 0xCB, 0x5D, 0xCB, 0x5E, 0xCB, 0x5F, 0xCB, 0x60, 0xCB, 0x61, 0xCB, 0x62, 0xCB, 0x63, 0xCB, 0x64, 0xCB, 0x65, 0xCB, 0x66, 0xCB, 0x67, 0xCB, 0x68, 0xCB, 0x69, 0xCB, 0x6A, 0xCB, 0x6B, 0xCB, 0x6C, 0xCB, 0x6D, 0xCB, 0x6E, 0xCB, 0x6F, 0xCB, 0x70, 0xCB, 0x71, 0xCB, 0x72, 0xCB, 0x73, 0xCB, 0x74, 0xCB, 0x75, 0xCB, 0x76, 0xCB, 0x77, 0xCB, 0x78, 0xCB, 0x79, 0xCB, 0x7A, 0xCB, 0x7B, 0xCB, 0x7C, 0xCB, 0x7D, 0xCB, 0x7E, 0xCB, 0x7F, 0xCB, 0x80, 0xCB, 0x81, 0xCB, 0x82, 0xCB, 0x83, 0xCB, 0x84, 0xCB, 0x85, 0xCB, 0x86, 0xCB, 0x87, 0xCB, 0x88, 0xCB, 0x89, 0xCB, 0x8A, 0xCB, 0x8B, 0xCB, 0x8C, 0xCB, 0x8D, 0xCB, 0x8E, 0xCB, 0x8F, 0xCB, 0x90, 0xCB, 0x91, 0xCB, 0x92, 0xCB, 0x93, 0xCB, 0x94, 0xCB, 0x95, 0xCB, 0x96, 0xCB, 0x97, 0xCB, 0x98, 0xCB, 0x99, 0xCB, 0x9A, 0xCB, 0x9B, 0xCB, 0x9C, 0xCB, 0x9D, 0xCB, 0x9E, 0xCB, 0x9F, 0xCB, 0xA0, 0xCB, 0xA1, 0xCB, 0xA2, 0xCB, 0xA3, 0xCB, 0xA4, 0xCB, 0xA5, 0xCB, 0xA6, 0xCB, 0xA7, 0xCB, 0xA8, 0xCB, 0xA9, 0xCB, 0xAA, 0xCB, 0xAB, 0xCB, 0xAC, 0xCB, 0xAD, 0xCB, 0xAE, 0xCB, 0xAF, 0xCB, 0xB0, 0xCB, 0xB1, 0xCB, 0xB2, 0xCB, 0xB3, 0xCB, 0xB4, 0xCB, 0xB5, 0xCB, 0xB6, 0xCB, 0xB7, 0xCB, 0xB8, 0xCB, 0xB9, 0xCB, 0xBA, 0xCB, 0xBB, 0xCB, 0xBC, 0xCB, 0xBD, 0xCB, 0xBE, 0xCB, 0xBF, 0xCB, 0xC0, 0xCB, 0xC1, 0xCB, 0xC2, 0xCB, 0xC3, 0xCB, 0xC4, 0xCB, 0xC5, 0xCB, 0xC6, 0xCB, 0xC7, 0xCB, 0xC8, 0xCB, 0xC9, 0xCB, 0xCA, 0xCB, 0xCB, 0xCB, 0xCC, 0xCB, 0xCD, 0xCB, 0xCE, 0xCB, 0xCF, 0xCB, 0xD0, 0xCB, 0xD1, 0xCB, 0xD2, 0xCB, 0xD3, 0xCB, 0xD4, 0xCB, 0xD5, 0xCB, 0xD6, 0xCB, 0xD7, 0xCB, 0xD8, 0xCB, 0xD9, 0xCB, 0xDA, 0xCB, 0xDB, 0xCB, 0xDC, 0xCB, 0xDD, 0xCB, 0xDE, 0xCB, 0xDF, 0xCB, 0xE0, 0xCB, 0xE1, 0xCB, 0xE2, 0xCB, 0xE3, 0xCB, 0xE4, 0xCB, 0xE5, 0xCB, 0xE6, 0xCB, 0xE7, 0xCB, 0xE8, 0xCB, 0xE9, 0xCB, 0xEA, 0xCB, 0xEB, 0xCB, 0xEC, 0xCB, 0xED, 0xCB, 0xEE, 0xCB, 0xEF, 0xCB, 0xF0, 0xCB, 0xF1, 0xCB, 0xF2, 0xCB, 0xF3, 0xCB, 0xF4, 0xCB, 0xF5, 0xCB, 0xF6, 0xCB, 0xF7, 0xCB, 0xF8, 0xCB, 0xF9, 0xCB, 0xFA, 0xCB, 0xFB, 0xCB, 0xFC, 0xCB, 0xFD, 0xCB, 0xFE, 0xCB, 0xFF };

void initCPU() {
	Halted = Stopped = WillEnableInterrupts = WillDisableInterrupts = 0;
	Startup = 1;
	
	PC = 0x000;
	SP = 0xfffe;
	AF.a = 0;
	AF.f = 1;
	
#ifdef RUNTESTS
	TestInstructions();
#endif
}

void setDefaults() {
	AF.af = 0x01b0;
	BC.bc = 0x0013;
	DE.de = 0x00d8;
	HL.hl = 0x014d;
	SP = 0xfffe;
	WriteMem(TIMA, 0x00); // TIMA
	WriteMem(TMA, 0x00); // TMA
	WriteMem(TAC, 0x00); // TAC
	WriteMem(NR10, 0x80); // NR10
	WriteMem(NR11, 0xbf); // NR11
	WriteMem(NR12, 0xf3); // NR12
	WriteMem(NR14, 0xbf); // NR14
	WriteMem(NR21, 0x3f); // NR21
	WriteMem(NR22, 0x00); // NR22
	WriteMem(NR24, 0xbf); // NR24
	WriteMem(NR30, 0x7f); // NR30
	WriteMem(NR31, 0xff); // NR31
	WriteMem(NR32, 0x9f); // NR32
	WriteMem(NR33, 0xbf); // NR33
	WriteMem(NR41, 0xff); // NR41
	WriteMem(NR42, 0x00); // NR42
	WriteMem(NR43, 0x00); // NR43
	WriteMem(NR44, 0xbf); // NR44
	WriteMem(NR50, 0x77); // NR50
	WriteMem(NR51, 0xf3); // NR51
	WriteMem(NR52, 0xf1); // NR52 GB
	WriteMem(LCDC, 0x91); // LCDC
	WriteMem(SCY, 0x00); // SCY
	WriteMem(SCX, 0x00); // SCX
	WriteMem(LYC, 0x00); // LYC
	WriteMem(BGP, 0xfc); // BGP
	WriteMem(OBP0, 0xff); // OBP0
	WriteMem(OBP1, 0xff); // OBP1
	WriteMem(WY, 0x00); // WY
	WriteMem(WX, 0x00); // WX
	WriteMem(IE, 0x00); // IE
}

void Start() {
	//readInitialProgram();
	//runInitialProgram();
	setDefaults();

	for(;;) {
		uint8_t inst = Startup ? InternalRom[PC] : Cartridge[PC];
		short params = GetParameters(inst);
		
		//printf("%x ", InternalRom[i]);
		if(params == 0) {
			printf(CodeToString(inst));
			Run(inst, 0, 0);
		} else if(params == 1) {
			if(inst == CB) {
				printf(CodeToString(inst), CBCodeToString(InternalRom[PC+1]));
			} else {
				printf(CodeToString(inst), InternalRom[PC+1]);
			}
			Run(inst, InternalRom[PC+1], 0);
		} else if(params == 2) {
			printf(CodeToString(inst), InternalRom[PC+1], InternalRom[PC+2]);
			Run(inst, InternalRom[PC+1], InternalRom[PC+2]);
		}
		printf("\n");
#ifdef STEPTHROUGH
		if(skip > 0) { skip--; }
		else if(skipuntil > 0 && skipuntil != PC) {}
		else {
			skipuntil = 0;
			DisplayState();
			char x = _getch();
			if(x == 'q') { break; }
			else if(x == 's') { skip = 0xfff;}
		}
#endif
		if(PC >= 0x100) { Startup = 0; }
	}
}
 
uint8_t getFlag(uint8_t flag) {
	return (AF.f & flag) == flag ? 1 : 0;
}
void setFlag(uint8_t flag) {
	AF.f |= flag;
}
void resetFlag(uint8_t flag) {
	uint8_t mask = ~flag;
	AF.f &= mask;
	//printf("mask: %x, flag: %x, flags: %x\n", mask, flag, flags);
}
 
// Memory
uint8_t ReadMem(uint16_t location) {
	
	if(location >= 0x100 && location <= 0x14c) {
		return Cartridge[location];
	} else if(location >= 0xe000 && location < 0xfe00) { // Allow for the mirrored internal RAM
		return Memory[location - 0x2000];
	} else {
		return Memory[location];
	}
	return 0;
}
 void WriteMem(uint16_t location, uint8_t value) {
	 
	if(location >= 0xe000 && location < 0xfe00) { // Allow for the mirrored internal RAM
		Memory[location - 0x2000] = value;
	} else {
		Memory[location] = value;
	}
 }
 
// Instructions 
void Run(uint8_t opcode, uint8_t param1, uint8_t param2) {
	uint8_t skipPCInc = 0;
	
	switch(opcode) {
		case CB:
			RunCBInstruction(param1); break;
		case NOP:
		case LD_A_A:
		case LD_B_B:
		case LD_C_C:
		case LD_E_E:
		case LD_H_H:
		case LD_L_L:
		case AND_A:
			break; // No effect
		case LD_A_B:
		case LD_A_C:
		case LD_A_D:
		case LD_A_E:
		case LD_A_H:
		case LD_A_L:
		case LD_A_HL:
		case LD_B_A:
		case LD_B_C:
		case LD_B_D:
		case LD_B_E:
		case LD_B_H:
		case LD_B_L:
		case LD_B_HL:
		case LD_C_A:
		case LD_C_B:
		case LD_C_D:
		case LD_C_E:
		case LD_C_H:
		case LD_C_L:
		case LD_C_HL:
		case LD_D_A:
		case LD_D_B:
		case LD_D_C:
		case LD_D_E:
		case LD_D_H:
		case LD_D_L:
		case LD_D_HL:
		case LD_E_A:
		case LD_E_B:
		case LD_E_C:
		case LD_E_D:
		case LD_E_H:
		case LD_E_L:
		case LD_E_HL:
		case LD_H_A:
		case LD_H_B:
		case LD_H_C:
		case LD_H_D:
		case LD_H_E:
		case LD_H_L:
		case LD_H_HL:
		case LD_L_A:
		case LD_L_B:
		case LD_L_C:
		case LD_L_D:
		case LD_L_E:
		case LD_L_H:
		case LD_L_HL:
		case LD_HL_A:
		case LD_HL_B:
		case LD_HL_C:
		case LD_HL_D:
		case LD_HL_E:
		case LD_HL_H:
		case LD_HL_L:
		case LD_BC_A:
		case LD_DE_A:
		case LD_HLP_A:
		case LD_HLM_A:
		case LD_A_HLP:
		case LD_A_HLM:
		case LD_A_BC:
		case LD_A_DE:
		case LD_A_n:
		case LD_B_n:
		case LD_C_n:
		case LD_D_n:
		case LD_E_n:
		case LD_H_n:
		case LD_L_n:
		case LD__HL__n:
		case LD_BC_nn:
		case LD_DE_nn:
		case LD_HL_nn:
		case LD_SP_nn:
		case LD_nn_SP:
		case LD__C__A:
		case LD_A__C_:
		case LD_nn_A:
		case LD_A_nn:
		case LD_HL_SP_n:
		case LD_SP_HL:
			LD(opcode, param1, param2); break;
		case AND_B:
		case AND_C:
		case AND_D:
		case AND_E:
		case AND_H:
		case AND_L:
		case AND_HL:
		case AND_n:
			AND(opcode, param1); break;
		case OR_A:
		case OR_B:
		case OR_C:
		case OR_D:
		case OR_E:
		case OR_H:
		case OR_L:
		case OR_HL:
		case OR_n:
			OR(opcode, param1); break;
		case ADD_A_A:
		case ADD_A_B:
		case ADD_A_C:
		case ADD_A_D:
		case ADD_A_E:
		case ADD_A_H:
		case ADD_A_L:
		case ADD_A_HL:
		case ADD_SP_n:
		case ADD_HL_BC:
		case ADD_HL_DE:
		case ADD_HL_HL:
		case ADD_HL_SP:
		case ADD_A_n:
			ADD(opcode, param1); break;
		case ADC_A_A:
		case ADC_A_B:
		case ADC_A_C:
		case ADC_A_D:
		case ADC_A_E:
		case ADC_A_H:
		case ADC_A_L:
		case ADC_A_HL:
		case ADC_A_n:
			ADC(opcode, param1); break;
		case SUB_A:
		case SUB_B:
		case SUB_C:
		case SUB_D:
		case SUB_E:
		case SUB_H:
		case SUB_L:
		case SUB_HL:
		case SUB_n:
			SUB(opcode, param1); break;
		case SBC_A_A:
		case SBC_A_B:
		case SBC_A_C:
		case SBC_A_D:
		case SBC_A_E:
		case SBC_A_H:
		case SBC_A_L:
		case SBC_A_HL:
		case SBC_A_n:
			SBC(opcode, param1); break;
		case CP_B:
		case CP_C:
		case CP_D:
		case CP_E:
		case CP_H:
		case CP_L:
		case CP_HL:
		case CP_n:
			CP(opcode, param1); break;
		case XOR_A:
		case XOR_B:
		case XOR_C:
		case XOR_D:
		case XOR_E:
		case XOR_H:
		case XOR_L:
		case XOR_HL:
		case XOR_n:
			XOR(opcode, param1); break;
		case INC_A:
		case INC_B:
		case INC_C:
		case INC_D:
		case INC_E:
		case INC_H:
		case INC_L:
		case INC__HL_:
		case INC_BC:
		case INC_DE:
		case INC_HL:
		case INC_SP:
			INC(opcode); break;
		case DEC_A:
		case DEC_B:
		case DEC_C:
		case DEC_D:
		case DEC_E:
		case DEC_H:
		case DEC_L:
		case DEC__HL_:
		case DEC_BC:
		case DEC_DE:
		case DEC_HL:
		case DEC_SP:
			DEC(opcode); break;
		case PUSH_AF:
		case PUSH_DE:
		case PUSH_BC:
		case PUSH_HL:
			PUSH(opcode); break;
		case POP_AF:
		case POP_DE:
		case POP_BC:
		case POP_HL:
			POP(opcode); break;
		case JR_NC_n:
		case JR_NZ_n:
		case JR_Z_n:
		case JR_C_n:
		case JR_n:
		case JP_NC_nn:
		case JP_NZ_nn:
		case JP_C_nn:
		case JP_Z_nn:
		case JP_nn:
		case JP_HL:
			JR(opcode, param1, param2); break;
		case CALL_NZ_nn:
		case CALL_NC_nn:
		case CALL_C_nn:
		case CALL_Z_nn:
		case CALL_nn:
			skipPCInc = 1;
			CALL(opcode, param1, param2); break;
		case RET_NC:
		case RET_NZ:
		case RET_Z:
		case RET_C:
		case RET:
		case RETI:
			RET_(opcode); break;
		case RST_00H:
		case RST_10H:
		case RST_20H:
		case RST_30H:
		case RST_08H:
		case RST_18H: 
		case RST_28H:
		case RST_38H:
			RST(opcode); break;
		case DAA:
			DAA_(); break;
		case RRCA:
			RRCA_(); break;
		case RRA:
			RRA_(); break;
		case RLCA:
			RLCA_(); break;
		case RLA:
			RLA_(); break;
		case SCF:
			setFlag(C); resetFlag(N); resetFlag(H); break;
		case CCF:
			if(getFlag(C)) { resetFlag(C); } else { setFlag(C); }; resetFlag(N); resetFlag(H); break;
		case CPL:
			AF.a ^= 0xff; setFlag(N); setFlag(H); break;
		case HALT:
			Halted = 1; break;
		case STOP:
			Stopped = 1; break;
		case DI:
			WillDisableInterrupts = 1; break;
		case EI:
			WillEnableInterrupts = 1; break;
		default:
			break;
			//printf("instruction [%x] [%x] [%x] not implemented\n", opcode, param1, param2);
	}
	if(skipPCInc) { skipPCInc = 0;}
	else { PC += (1 + GetParameters(opcode)); }
}
void RunCBInstruction(uint8_t opcode) {
	switch(opcode) {
		case SWAP_A:
		case SWAP_B:
		case SWAP_C:
		case SWAP_D:
		case SWAP_E:
		case SWAP_H:
		case SWAP_L:
		case SWAP_HL:
			SWAP(opcode); break;
		case BIT0_B:
		case BIT0_C:
		case BIT0_D:
		case BIT0_E:
		case BIT0_H:
		case BIT0_L:
		case BIT0_HL:
		case BIT0_A:
			BIT(opcode, 0); break;
		case BIT1_B:
		case BIT1_C:
		case BIT1_D:
		case BIT1_E:
		case BIT1_H:
		case BIT1_L:
		case BIT1_HL:
		case BIT1_A:
			BIT(opcode, 1); break;
		case BIT2_B:
		case BIT2_C:
		case BIT2_D:
		case BIT2_E:
		case BIT2_H:
		case BIT2_L:
		case BIT2_HL:
		case BIT2_A:
			BIT(opcode, 2); break;
		case BIT3_B:
		case BIT3_C:
		case BIT3_D:
		case BIT3_E:
		case BIT3_H:
		case BIT3_L:
		case BIT3_HL:
		case BIT3_A:
			BIT(opcode, 3); break;
		case BIT4_B:
		case BIT4_C:
		case BIT4_D:
		case BIT4_E:
		case BIT4_H:
		case BIT4_L:
		case BIT4_HL:
		case BIT4_A:
			BIT(opcode, 4); break;
		case BIT5_B:
		case BIT5_C:
		case BIT5_D:
		case BIT5_E:
		case BIT5_H:
		case BIT5_L:
		case BIT5_HL:
		case BIT5_A:
			BIT(opcode, 5); break;
		case BIT6_B:
		case BIT6_C:
		case BIT6_D:
		case BIT6_E:
		case BIT6_H:
		case BIT6_L:
		case BIT6_HL:
		case BIT6_A:
			BIT(opcode, 6); break;
		case BIT7_B:
		case BIT7_C:
		case BIT7_D:
		case BIT7_E:
		case BIT7_H:
		case BIT7_L:
		case BIT7_HL:
		case BIT7_A:
			BIT(opcode, 7); break;
		case RES0_B:
		case RES0_C:
		case RES0_D:
		case RES0_E:
		case RES0_H:
		case RES0_L:
		case RES0_HL:
		case RES0_A:
			RES(opcode, 0); break;
		case RES1_B:
		case RES1_C:
		case RES1_D:
		case RES1_E:
		case RES1_H:
		case RES1_L:
		case RES1_HL:
		case RES1_A:
			RES(opcode, 1); break;
		case RES2_B:
		case RES2_C:
		case RES2_D:
		case RES2_E:
		case RES2_H:
		case RES2_L:
		case RES2_HL:
		case RES2_A:
			RES(opcode, 2); break;
		case RES3_B:
		case RES3_C:
		case RES3_D:
		case RES3_E:
		case RES3_H:
		case RES3_L:
		case RES3_HL:
		case RES3_A:
			RES(opcode, 3); break;
		case RES4_B:
		case RES4_C:
		case RES4_D:
		case RES4_E:
		case RES4_H:
		case RES4_L:
		case RES4_HL:
		case RES4_A:
			RES(opcode, 4); break;
		case RES5_B:
		case RES5_C:
		case RES5_D:
		case RES5_E:
		case RES5_H:
		case RES5_L:
		case RES5_HL:
		case RES5_A:
			RES(opcode, 5); break;
		case RES6_B:
		case RES6_C:
		case RES6_D:
		case RES6_E:
		case RES6_H:
		case RES6_L:
		case RES6_HL:
		case RES6_A:
			RES(opcode, 6); break;
		case RES7_B:
		case RES7_C:
		case RES7_D:
		case RES7_E:
		case RES7_H:
		case RES7_L:
		case RES7_HL:
		case RES7_A:
			RES(opcode, 7); break;
		case SET0_B:
		case SET0_C:
		case SET0_D:
		case SET0_E:
		case SET0_H:
		case SET0_L:
		case SET0_HL:
		case SET0_A:
			SET(opcode, 0); break;
		case SET1_B:
		case SET1_C:
		case SET1_D:
		case SET1_E:
		case SET1_H:
		case SET1_L:
		case SET1_HL:
		case SET1_A:
			SET(opcode, 1); break;
		case SET2_B:
		case SET2_C:
		case SET2_D:
		case SET2_E:
		case SET2_H:
		case SET2_L:
		case SET2_HL:
		case SET2_A:
			SET(opcode, 2); break;
		case SET3_B:
		case SET3_C:
		case SET3_D:
		case SET3_E:
		case SET3_H:
		case SET3_L:
		case SET3_HL:
		case SET3_A:
			SET(opcode, 3); break;
		case SET4_B:
		case SET4_C:
		case SET4_D:
		case SET4_E:
		case SET4_H:
		case SET4_L:
		case SET4_HL:
		case SET4_A:
			SET(opcode, 4); break;
		case SET5_B:
		case SET5_C:
		case SET5_D:
		case SET5_E:
		case SET5_H:
		case SET5_L:
		case SET5_HL:
		case SET5_A:
			SET(opcode, 5); break;
		case SET6_B:
		case SET6_C:
		case SET6_D:
		case SET6_E:
		case SET6_H:
		case SET6_L:
		case SET6_HL:
		case SET6_A:
			SET(opcode, 6); break;
		case SET7_B:
		case SET7_C:
		case SET7_D:
		case SET7_E:
		case SET7_H:
		case SET7_L:
		case SET7_HL:
		case SET7_A:
			SET(opcode, 7); break;
		case RLC_B:
		case RLC_C:
		case RLC_D:
		case RLC_E:
		case RLC_H:
		case RLC_L:
		case RLC_HL:
		case RLC_A:
			RLC(opcode); break;
		case RRC_B:
		case RRC_C:
		case RRC_D:
		case RRC_E:
		case RRC_H:
		case RRC_L:
		case RRC_HL:
		case RRC_A:
			RRC(opcode); break;
		case RL_A:
		case RL_B:
		case RL_C:
		case RL_D:
		case RL_E:
		case RL_H:
		case RL_L:
		case RL_HL:
			RL(opcode); break;
		case RR_A:
		case RR_B:
		case RR_C:
		case RR_D:
		case RR_E:
		case RR_H:
		case RR_L:
		case RR_HL:
			RR(opcode); break;
		case SRA_A:
		case SRA_B:
		case SRA_C:
		case SRA_D:
		case SRA_E:
		case SRA_L:
		case SRA_H:
		case SRA_HL:
			SRA(opcode);
		case SLA_A:
		case SLA_B:
		case SLA_C:
		case SLA_D:
		case SLA_E:
		case SLA_L:
		case SLA_H:
		case SLA_HL:
			SLA(opcode);
		case SRL_A:
		case SRL_B:
		case SRL_C:
		case SRL_D:
		case SRL_E:
		case SRL_L:
		case SRL_H:
		case SRL_HL:
			SRL(opcode);
	}
}

void EnableInterrupts() {}
void DisableInterrupts() {}

void RRCA_() {
	resetFlag(Z);
	resetFlag(H);
	resetFlag(N);
	resetFlag(C);
	
	uint8_t c = AF.a & 1;
	AF.a >>= 1;
	AF.a += c << 7;
	if(c) {
		setFlag(C);
	}
}
void RRA_() {
	uint8_t oldC = getFlag(C);
	resetFlag(Z);
	resetFlag(H);
	resetFlag(N);
	resetFlag(C);
	
	uint8_t c = AF.a & 1;
	AF.a >>= 1;
	AF.a += oldC << 7;
	if(c) {
		setFlag(C);
	}
}
void RLCA_() {
	resetFlag(Z);
	resetFlag(H);
	resetFlag(N);
	resetFlag(C);
	
	uint8_t c = AF.a & 0x80;
	AF.a <<= 1;
	AF.a += c >> 7;
	if(c) {
		setFlag(C);
	}
}
void RLA_() {
	uint8_t oldC = getFlag(C);
	resetFlag(Z);
	resetFlag(H);
	resetFlag(N);
	resetFlag(C);
	
	uint8_t c = AF.a & 0x80;
	AF.a <<= 1;
	AF.a += oldC;
	if(c) {
		setFlag(C);
	}
}

void XOR(uint8_t opcode, uint8_t param) {
	 
	 switch(opcode) {
		 case XOR_A:
			AF.a ^= AF.a; break;
		case XOR_B:
			AF.a ^= BC.b; break;
		case XOR_C:
			AF.a ^= BC.c; break;
		case XOR_D:
			AF.a ^= DE.d; break;
		case XOR_E:
			AF.a ^= DE.e; break;
		case XOR_H:
			AF.a ^= HL.h; break;
		case XOR_L:
			AF.a ^= HL.l; break;
		case XOR_HL:
			AF.a ^= ReadMem(HL.hl); break;
		case XOR_n:
			AF.a ^= param; break;
	 }
	 
	 if(AF.a == 0) {
		 setFlag(Z);
	 } else {
		 resetFlag(Z);
	 }
 }
 
void ADD(uint8_t opcode, uint8_t param) {
	uint8_t a, val;
	uint16_t val16;
	
	a = AF.a;
	
	switch(opcode) {
		case ADD_A_A:
			AF.a += AF.a; val = AF.a; break;
		case ADD_A_B:
			AF.a += BC.b; val = BC.b; break;
		case ADD_A_C:
			AF.a += BC.c; val = BC.c; break;
		case ADD_A_D:
			AF.a += DE.d; val = DE.d; break;
		case ADD_A_E:
			AF.a += DE.e; val = DE.e; break;
		case ADD_A_H:
			AF.a += HL.h; val = HL.h; break;
		case ADD_A_L:
			AF.a += HL.l; val = HL.l; break;
		case ADD_A_HL:
			val = ReadMem(HL.hl); AF.a += val; break;
		case ADD_SP_n:
			SP += param; val = (int8_t)param; break;
		case ADD_HL_BC:
			HL.hl += BC.bc; val16 = BC.bc; break;
		case ADD_HL_DE:
			HL.hl += DE.de; val16 = DE.de; break;
		case ADD_HL_HL:
			HL.hl += HL.hl; val16 = HL.hl; break;
		case ADD_HL_SP:
			HL.hl += SP; val16 = SP; break;
		case ADD_A_n:
			AF.a += param; val = param; break;
	}
	
	if(opcode == ADD_HL_BC || opcode == ADD_HL_DE || opcode == ADD_HL_HL || opcode == ADD_HL_SP) {
		if(((HL.hl & 0xfff) + (val16 & 0xfff)) & 0x1000)
			setFlag(H);
		else
			resetFlag(H);
		
		if((HL.hl + val16) & 0x10000)
			setFlag(C);
		else
			resetFlag(C);
		resetFlag(N);
	} else if(opcode == ADD_SP_n) {
		if(((SP & 0xfff) + (val & 0xfff)) & 0x1000)
			setFlag(H);
		else
			resetFlag(H);
		
		if((SP + val) & 0x10000)
			setFlag(C);
		else
			resetFlag(C);
		resetFlag(Z);
	} else {
		if(((a & 0xf) + (val & 0xf)) & 0x10)
			setFlag(H);
		else
			resetFlag(H);
		
		if((a + val) & 0x100)
			setFlag(C);
		else
			resetFlag(C);
		
		if(AF.a == 0) {
			setFlag(Z);
		} else {
			resetFlag(Z);
		}
	}
	resetFlag(N);
}

void ADC(uint8_t opcode, uint8_t param) {
	uint8_t a, val;
	
	a = AF.a;
	short flag = getFlag(C);
	
	switch(opcode) {
		case ADC_A_A:
			AF.a += AF.a + flag; val = AF.a; break;
		case ADC_A_B:
			AF.a += BC.b + flag; val = BC.b; break;
		case ADC_A_C:
			AF.a += BC.c + flag; val = BC.c; break;
		case ADC_A_D:
			AF.a += DE.d + flag; val = DE.d; break;
		case ADC_A_E:
			AF.a += DE.e + flag; val = DE.e; break;
		case ADC_A_H:
			AF.a += HL.h + flag; val = HL.h; break;
		case ADC_A_L:
			AF.a += HL.l + flag; val = HL.l; break;
		case ADC_A_HL:
			val = ReadMem(HL.hl); AF.a += val + flag; break;
		case ADC_A_n:
			AF.a += param + flag; val = param; break; 
	}
	
	if(((a & 0xf) + (val & 0xf)) & 0x10)
		setFlag(H);
	else
		resetFlag(H);
	
	if((a + val) & 0x100)
		setFlag(C);
	else
		resetFlag(C);
	
	if(AF.a == 0) {
		setFlag(Z);
	} else {
		resetFlag(Z);
	}
	resetFlag(N);
}
void AND(uint8_t opcode, uint8_t param) {	
	switch(opcode) {
		case AND_B:
			AF.a &= BC.b; break;
		case AND_C:
			AF.a &= BC.c; break;
		case AND_D:
			AF.a &= DE.d; break;
		case AND_E:
			AF.a &= DE.e; break;
		case AND_H:
			AF.a &= HL.h; break;
		case AND_L:
			AF.a &= HL.l;break;
		case AND_HL:
			AF.a &= ReadMem(HL.hl); break;
		case AND_n:
			AF.a &= param; break;
	}
	
	setFlag(H);
	resetFlag(C);
	resetFlag(N);
	
	if(AF.a == 0) {
		setFlag(Z);
	} else {
		resetFlag(Z);
	}
}
 
void LD(uint8_t opcode, uint8_t param1, uint8_t param2) {
	uint16_t address = (param2 << 8) + param1;
	
	switch(opcode) {
		case LD_A_A:
			break; // No effect
		case LD_A_B:
			AF.a = BC.b; break;
		case LD_A_C:
			AF.a = BC.c; break;
		case LD_A_D:
			AF.a = DE.d; break;
		case LD_A_E:
			AF.a = DE.e; break;
		case LD_A_H:
			AF.a = HL.h; break;
		case LD_A_L:
			AF.a = HL.l; break;
		case LD_A_HL:
			AF.a = ReadMem(HL.hl); break;
		case LD_A_n:
			AF.a = param1; break;
		case LD_B_B:
			break;
		case LD_B_A:
			BC.b = AF.a; break;
		case LD_B_C:
			BC.b = BC.c; break;
		case LD_B_D:
			BC.b = DE.d; break;
		case LD_B_E:
			BC.b = DE.e; break;
		case LD_B_H:
			BC.b = HL.h; break;
		case LD_B_L:
			BC.b = HL.l; break;
		case LD_B_HL:
			BC.b = ReadMem(HL.hl); break;
		case LD_B_n:
			BC.b = param1; break;
		case LD_C_C:
			break;
		case LD_C_A:
			BC.c = AF.a; break;
		case LD_C_B:
			BC.c = BC.b; break;
		case LD_C_D:
			BC.c = DE.d; break;
		case LD_C_E:
			BC.c = DE.e; break;
		case LD_C_H:
			BC.c = HL.h; break;
		case LD_C_L:
			BC.c = HL.l; break;
		case LD_C_HL:
			BC.c = ReadMem(HL.hl); break;
		case LD_C_n:
			BC.c = param1; break;
		case LD_D_D:
			break;
		case LD_D_A:
			DE.d = AF.a; break;
		case LD_D_B:
			DE.d = BC.b; break;
		case LD_D_C:
			DE.d = BC.c; break;
		case LD_D_E:
			DE.d = DE.e; break;
		case LD_D_H:
			DE.d = HL.h; break;
		case LD_D_L:
			DE.d = HL.l; break;
		case LD_D_HL:
			DE.d = ReadMem(HL.hl); break;
		case LD_D_n:
			DE.d = param1; break;
		case LD_E_E:
			break;
		case LD_E_A:
			DE.e = AF.a; break;
		case LD_E_B:
			DE.e = BC.b; break;
		case LD_E_C:
			DE.e = BC.c; break;
		case LD_E_D:
			DE.e = DE.d; break;
		case LD_E_H:
			DE.e = HL.h; break;
		case LD_E_L:
			DE.e = HL.l; break;
		case LD_E_HL:
			DE.e = ReadMem(HL.hl); break;
		case LD_E_n:
			DE.e = param1; break;
		case LD_H_H:
			break;
		case LD_H_A:
			HL.h = AF.a; break;
		case LD_H_B:
			HL.h = BC.b; break;
		case LD_H_C:
			HL.h = BC.c; break;
		case LD_H_D:
			HL.h = DE.d; break;
		case LD_H_E:
			HL.h = DE.e; break;
		case LD_H_L:
			HL.h = HL.l; break;
		case LD_H_HL:
			HL.h = ReadMem(HL.hl); break;
		case LD_H_n:
			HL.h = param1; break;
		case LD_L_L:
			break;
		case LD_L_A:
			HL.l = AF.a; break;
		case LD_L_B:
			HL.l = BC.b; break;
		case LD_L_C:
			HL.l = BC.c; break;
		case LD_L_D:
			HL.l = DE.d; break;
		case LD_L_E:
			HL.l = DE.e; break;
		case LD_L_H:
			HL.l = HL.h; break;
		case LD_L_HL:
			HL.l = ReadMem(HL.hl); break;
		case LD_L_n:
			HL.l = param1; break;
		case LD_HL_A:
			WriteMem(HL.hl, AF.a); break;
		case LD_HL_B:
			WriteMem(HL.hl, BC.b); break;
		case LD_HL_C:
			WriteMem(HL.hl, BC.c); break;
		case LD_HL_D:
			WriteMem(HL.hl, DE.d); break;
		case LD_HL_E:
			WriteMem(HL.hl, DE.e); break;
		case LD_HL_H:
			WriteMem(HL.hl, HL.h); break;
		case LD_HL_L:
			WriteMem(HL.hl, HL.l); break;
		case LD__HL__n:
			WriteMem(HL.hl, param1); break;
		case LD_BC_nn:
			BC.bc = address; break;
		case LD_DE_nn:
			DE.de = address; break;
		case LD_HL_nn:
			HL.hl = address; break;
		case LD_SP_nn:
			//printf("\nparam1=%02x, param2=%02x, address=%04x", param1, param2, address);
			SP = address; break;
		case LD_nn_SP:
			uint16_t temp = address;
			WriteMem(temp, ReadMem(SP)); break;
		case LD_BC_A:
			WriteMem(BC.bc, AF.a); break;
		case LD_DE_A:
			WriteMem(DE.de, AF.a); break;
		case LD_HLP_A:
			WriteMem(HL.hl, AF.a); HL.hl++; break;
		case LD_HLM_A:
			WriteMem(HL.hl, AF.a); HL.hl--; break;
		case LD_A_HLP:
			AF.a = ReadMem(HL.hl); HL.hl++; break;
		case LD_A_HLM:
			AF.a = ReadMem(HL.hl); HL.hl--; break;
		case LD_A_BC:
			AF.a = ReadMem(BC.bc); break;
		case LD_A_DE:
			AF.a = ReadMem(DE.de); break;
		case LD__C__A:
			WriteMem(0xff + BC.c, AF.a); break;
		case LD_A__C_:
			AF.a = ReadMem(0xff + BC.c); break;
		case LDH_n_A:
			WriteMem(0xff + param1, AF.a); break;
		case LDH_A_n:
			AF.a = ReadMem(0xff + param1); break;
		case LD_nn_A:
			WriteMem(address, AF.a); break;
		case LD_A_nn:
			AF.a = ReadMem(address); break;
		case LD_HL_SP_n:
			resetFlag(Z);
			resetFlag(N);
			if(((SP & 0xfff) + (param1 & 0xfff)) & 0x1000)
				setFlag(H);
			else
				resetFlag(H);
			
			if((SP + param1) & 0x10000)
				setFlag(C);
			else
				resetFlag(C);
			resetFlag(Z);
			HL.hl = SP + (int8_t)param1;
		case LD_SP_HL:
			SP = HL.hl; break;
	 }
 }
 
void INC(uint8_t opcode) {
	resetFlag(Z);
	
	switch(opcode) {
		case INC_A:
			AF.a++; if(AF.a == 0) { setFlag(Z); } break;
		case INC_B:
			BC.b++; if(BC.b == 0) { setFlag(Z); }  break;
		case INC_C:
			BC.c++; if(BC.c == 0) { setFlag(Z); }  break;
		case INC_D:
			DE.d++; if(DE.d == 0) { setFlag(Z); }  break;
		case INC_E:
			DE.e++; if(DE.e == 0) { setFlag(Z); }  break;
		case INC_H:
			HL.h++; if(HL.h == 0) { setFlag(Z); }  break;
		case INC_L:
			HL.l++; if(HL.l == 0) { setFlag(Z); }  break;
		case INC__HL_:
			uint8_t val = ReadMem(HL.hl); WriteMem(HL.hl, val+1); if((val+1) == 0) { setFlag(Z); }  break;
		case INC_BC:
			BC.bc++; break;
		case INC_DE:
			DE.de++; break;
		case INC_HL:
			HL.hl++; break;
		case INC_SP:
			SP++; break;
	}
	
	// TODO: Half carry
}

void DEC(uint8_t opcode) {
	resetFlag(Z);
	
	switch(opcode) {
		case DEC_A:
			AF.a--; if(AF.a == 0) { setFlag(Z); } break;
		case DEC_B:
			BC.b--; if(BC.b == 0) { setFlag(Z); }  break;
		case DEC_C:
			BC.c--; if(BC.c == 0) { setFlag(Z); }  break;
		case DEC_D:
			DE.d--; if(DE.d == 0) { setFlag(Z); }  break;
		case DEC_E:
			DE.e--; if(DE.e == 0) { setFlag(Z); }  break;
		case DEC_H:
			HL.h--; if(HL.h == 0) { setFlag(Z); }  break;
		case DEC_L:
			HL.l--; if(HL.l == 0) { setFlag(Z); }  break;
		case DEC__HL_:
			uint8_t val = ReadMem(HL.hl); WriteMem(HL.hl, val-1); if((val-1) == 0) { setFlag(Z); }  break;
		case DEC_BC:
			BC.bc--; break;
		case DEC_DE:
			DE.de--; break;
		case DEC_HL:
			HL.hl--; break;
		case DEC_SP:
			SP--; break;
	}
	
	// TODO: Half carry
}

void JR(uint8_t opcode, uint8_t param1, uint8_t param2) {
	uint16_t address = param1 + (param2 << 8);
	
	switch(opcode) {
		case JR_NC_n:
			if(!getFlag(C)) { PC += (int8_t)param1; }; break;
		case JR_NZ_n:
			if(!getFlag(Z)) { PC += (int8_t)param1; }; break;
		case JR_Z_n:
			if(getFlag(Z)) { PC += (int8_t)param1; }; break;
		case JR_C_n:
			if(getFlag(C)) { PC += (int8_t)param1; }; break;
		case JR_n:
			PC += (int8_t)param1; break;
		case JP_NC_nn:
			if(!getFlag(C)) { PC = address; }; break;
		case JP_NZ_nn:
			if(!getFlag(Z)) { PC = address; }; break;
		case JP_C_nn:
			if(getFlag(C)) { PC = address; }; break;
		case JP_Z_nn:
			if(getFlag(Z)) { PC = address; }; break;
		case JP_nn:
			PC = address; break;
		case JP_HL:
			PC = ReadMem(HL.hl); break;
	}
}
void CALL(uint8_t opcode, uint8_t param1, uint8_t param2) {
	uint16_t address = (param2 << 8) + param1;
	uint8_t doCall = 0;
	short i=0;
	printf("\nparam1=%02x, param2=%02x, address=%04x\n", param1, param2, address);
	
	switch(opcode) {
		case CALL_NC_nn:
			if(!getFlag(C)) { doCall = 1; }; break;
		case CALL_NZ_nn:
			if(!getFlag(Z)) { doCall = 1; }; break;
		case CALL_Z_nn:
			if(getFlag(Z)) { doCall = 1; }; break;
		case CALL_C_nn:
			if(getFlag(C)) { doCall = 1; }; break;
		case CALL_nn:
			doCall = 1; break;
	}
	
	if(doCall) {
		uint16_t next = PC + 1;
		WriteMem(SP, (uint8_t)(next >> 8)); SP--;
		WriteMem(SP, (uint8_t)(next & 0xff)); SP--;
		PC = address;
	}
}
void RET_(uint8_t opcode) {
	uint16_t address;
	
	address = ReadMem(SP+1) + (ReadMem(SP+2) << 8);
	
	switch(opcode) {
		case RET_NC:
			if(!getFlag(C)) { PC = address; SP += 2; }; break;
		case RET_NZ:
			if(!getFlag(Z)) { PC = address; SP += 2; }; break;
		case RET_Z:
			if(getFlag(Z)) { PC = address; SP += 2; }; break;
		case RET_C:
			if(getFlag(C)) { PC = address; SP += 2; }; break;
		case RET:
			PC = address; SP += 2; break;
		case RETI:
			PC = address; SP += 2; EnableInterrupts(); break;
		
	}
}

void SWAP(uint8_t opcode) {
	uint8_t tmp, val;
	resetFlag(Z);
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	
	switch(opcode) {
		case SWAP_A:
			tmp = AF.a << 4; AF.a = (AF.a >> 4) | tmp; tmp = AF.a; break;
		case SWAP_B:
			tmp = BC.b << 4; BC.b = (BC.b >> 4) | tmp; tmp = BC.b; break;
		case SWAP_C:
			tmp = BC.c << 4; BC.c = (BC.c >> 4) | tmp; tmp = BC.c; break;
		case SWAP_D:
			tmp = DE.d << 4; DE.d = (DE.d >> 4) | tmp; tmp = DE.d; break;
		case SWAP_E:
			tmp = DE.e << 4; DE.e = (DE.e >> 4) | tmp; tmp = DE.e; break;
		case SWAP_H:
			tmp = HL.h << 4; HL.h = (HL.h >> 4) | tmp; tmp = HL.h; break;
		case SWAP_L:
			tmp = HL.l << 4; HL.l = (HL.l >> 4) | tmp; tmp = HL.l; break;
		case SWAP_HL:
			val = ReadMem(HL.hl);
			tmp = val << 4; val = (val >> 4) | tmp; tmp = val; 
			WriteMem(HL.hl, val); break;
	}
	
	if(tmp == 0) { setFlag(Z); }

}

void BIT(uint8_t opcode, int bit_n) {
	uint8_t set = 0;
	uint8_t bit = 0;
	
	bit = 1 << bit_n;
	
	switch(opcode) {
		case BIT0_A:
		case BIT1_A:
		case BIT2_A:
		case BIT3_A:
		case BIT4_A:
		case BIT5_A:
		case BIT6_A:
		case BIT7_A:
			bit = AF.a & bit; break;
		case BIT0_B:
		case BIT1_B:
		case BIT2_B:
		case BIT3_B:
		case BIT4_B:
		case BIT5_B:
		case BIT6_B:
		case BIT7_B:
			bit = BC.b & bit; break;
		case BIT0_C:
		case BIT1_C:
		case BIT2_C:
		case BIT3_C:
		case BIT4_C:
		case BIT5_C:
		case BIT6_C:
		case BIT7_C:
			bit = BC.c & bit; break;
		case BIT0_D:
		case BIT1_D:
		case BIT2_D:
		case BIT3_D:
		case BIT4_D:
		case BIT5_D:
		case BIT6_D:
		case BIT7_D:
			bit = DE.d & bit; break;
		case BIT0_E:
		case BIT1_E:
		case BIT2_E:
		case BIT3_E:
		case BIT4_E:
		case BIT5_E:
		case BIT6_E:
		case BIT7_E:
			bit = DE.e & bit; break;
		case BIT0_H:
		case BIT1_H:
		case BIT2_H:
		case BIT3_H:
		case BIT4_H:
		case BIT5_H:
		case BIT6_H:
		case BIT7_H:
			bit = HL.h & bit; break;
		case BIT0_L:
		case BIT1_L:
		case BIT2_L:
		case BIT3_L:
		case BIT4_L:
		case BIT5_L:
		case BIT6_L:
		case BIT7_L:
			bit = HL.l & bit; break;
		case BIT0_HL:
		case BIT1_HL:
		case BIT2_HL:
		case BIT3_HL:
		case BIT4_HL:
		case BIT5_HL:
		case BIT6_HL:
		case BIT7_HL:
			bit = ReadMem(HL.hl) & bit; break;
		
	}
	
	if(bit) {
		resetFlag(Z);
	} else {
		setFlag(Z);
	}
	resetFlag(N);
	setFlag(H);
}

void SET(uint8_t opcode, uint8_t bit_n) {
	uint8_t bit = 0;
	
	bit = 1 << bit_n;
	
	switch(opcode) {
		case SET0_A:
		case SET1_A:
		case SET2_A:
		case SET3_A:
		case SET4_A:
		case SET5_A:
		case SET6_A:
		case SET7_A:
			AF.a |= bit; break;
		case SET0_B:
		case SET1_B:
		case SET2_B:
		case SET3_B:
		case SET4_B:
		case SET5_B:
		case SET6_B:
		case SET7_B:
			BC.b |= bit; break;
		case SET0_C:
		case SET1_C:
		case SET2_C:
		case SET3_C:
		case SET4_C:
		case SET5_C:
		case SET6_C:
		case SET7_C:
			BC.c |= bit; break;
		case SET0_D:
		case SET1_D:
		case SET2_D:
		case SET3_D:
		case SET4_D:
		case SET5_D:
		case SET6_D:
		case SET7_D:
			DE.d |= bit; break;
		case SET0_E:
		case SET1_E:
		case SET2_E:
		case SET3_E:
		case SET4_E:
		case SET5_E:
		case SET6_E:
		case SET7_E:
			DE.e |= bit; break;
		case SET0_H:
		case SET1_H:
		case SET2_H:
		case SET3_H:
		case SET4_H:
		case SET5_H:
		case SET6_H:
		case SET7_H:
			HL.h |= bit; break;
		case SET0_L:
		case SET1_L:
		case SET2_L:
		case SET3_L:
		case SET4_L:
		case SET5_L:
		case SET6_L:
		case SET7_L:
			HL.l |= bit; break;
		case SET0_HL:
		case SET1_HL:
		case SET2_HL:
		case SET3_HL:
		case SET4_HL:
		case SET5_HL:
		case SET6_HL:
		case SET7_HL:
			WriteMem(HL.hl, ReadMem(HL.hl) | bit); break;
	}
}
void RES(uint8_t opcode, uint8_t bit_n) {
	uint8_t bit = 0;
	
	bit = (1 << bit_n) ^ 1;
	
	switch(opcode) {
		case RES0_A:
		case RES1_A:
		case RES2_A:
		case RES3_A:
		case RES4_A:
		case RES5_A:
		case RES6_A:
		case RES7_A:
			AF.a &= bit; break;
		case RES0_B:
		case RES1_B:
		case RES2_B:
		case RES3_B:
		case RES4_B:
		case RES5_B:
		case RES6_B:
		case RES7_B:
			BC.b &= bit; break;
		case RES0_C:
		case RES1_C:
		case RES2_C:
		case RES3_C:
		case RES4_C:
		case RES5_C:
		case RES6_C:
		case RES7_C:
			BC.c &= bit; break;
		case RES0_D:
		case RES1_D:
		case RES2_D:
		case RES3_D:
		case RES4_D:
		case RES5_D:
		case RES6_D:
		case RES7_D:
			DE.d &= bit; break;
		case RES0_E:
		case RES1_E:
		case RES2_E:
		case RES3_E:
		case RES4_E:
		case RES5_E:
		case RES6_E:
		case RES7_E:
			DE.e &= bit; break;
		case RES0_H:
		case RES1_H:
		case RES2_H:
		case RES3_H:
		case RES4_H:
		case RES5_H:
		case RES6_H:
		case RES7_H:
			HL.h &= bit; break;
		case RES0_L:
		case RES1_L:
		case RES2_L:
		case RES3_L:
		case RES4_L:
		case RES5_L:
		case RES6_L:
		case RES7_L:
			HL.l &= bit; break;
		case RES0_HL:
		case RES1_HL:
		case RES2_HL:
		case RES3_HL:
		case RES4_HL:
		case RES5_HL:
		case RES6_HL:
		case RES7_HL:
			WriteMem(HL.hl, ReadMem(HL.hl) & bit); break;
	}
}
void RLC(uint8_t opcode) {
	resetFlag(Z);
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	uint8_t outbit = 1 << 7;
	uint8_t val = 0;
	
	switch(opcode) {
		case RLC_A:
			outbit &= AF.a; AF.a <<= 1; outbit >>= 7; AF.a += outbit; val = AF.a; break;
		case RLC_B:
			outbit &= BC.b; BC.b <<= 1; outbit >>= 7; BC.b += outbit; val = BC.b; break;
		case RLC_C:
			outbit &= BC.c; BC.c <<= 1; outbit >>= 7; BC.c += outbit; val = BC.c; break;
		case RLC_D:
			outbit &= DE.d; DE.d <<= 1; outbit >>= 7; DE.d += outbit; val = DE.d; break;
		case RLC_E:
			outbit &= DE.e; DE.e <<= 1; outbit >>= 7; DE.e += outbit; val = DE.e; break;
		case RLC_H:
			outbit &= HL.h; HL.h <<= 1; outbit >>= 7; HL.h += outbit; val = HL.h; break;
		case RLC_L:
			outbit &= HL.l; HL.l <<= 1; outbit >>= 7; HL.l += outbit; val = HL.l; break;
		case RLC_HL:
			val = ReadMem(HL.hl);
			outbit &= val; val <<= 1; outbit >>= 7; val += outbit;
			WriteMem(HL.hl, val); break;
	}
	
	if(outbit) {
		setFlag(C);
	}
	if(val == 0) {
		setFlag(Z);
	}
}
void RL(uint8_t opcode) {
	uint8_t oldC = getFlag(C);
	resetFlag(Z);
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	uint8_t outbit = 1 << 7;
	uint8_t val = 0;
	
	switch(opcode) {
		case RL_A:
			outbit &= AF.a; AF.a <<= 1; outbit >>= 7; AF.a += oldC; val = AF.a; break;
		case RL_B:
			outbit &= BC.b; BC.b <<= 1; outbit >>= 7; BC.b += oldC; val = BC.b; break;
		case RL_C:
			outbit &= BC.c; BC.c <<= 1; outbit >>= 7; BC.c += oldC; val = BC.c; break;
		case RL_D:
			outbit &= DE.d; DE.d <<= 1; outbit >>= 7; DE.d += oldC; val = DE.d; break;
		case RL_E:
			outbit &= DE.e; DE.e <<= 1; outbit >>= 7; DE.e += oldC; val = DE.e; break;
		case RL_H:
			outbit &= HL.h; HL.h <<= 1; outbit >>= 7; HL.h += oldC; val = HL.h; break;
		case RL_L:
			outbit &= HL.l; HL.l <<= 1; outbit >>= 7; HL.l += oldC; val = HL.l; break;
		case RL_HL:
			val = ReadMem(HL.hl);
			outbit &= val; val <<= 1; outbit >>= 7; val += oldC;
			WriteMem(HL.hl, val); break;
	}
	
	if(outbit) {
		setFlag(C);
	}
	if(val == 0) {
		setFlag(Z);
	}
}
void RRC(uint8_t opcode) {
	resetFlag(Z);
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	uint8_t outbit = 1;
	uint8_t val = 0;
	
	switch(opcode) {
		case RRC_A:
			outbit &= AF.a; AF.a >>= 1; outbit <<= 7; AF.a += outbit; val = AF.a; break;
		case RRC_B:
			outbit &= BC.b; BC.b >>= 1; outbit <<= 7; BC.b += outbit; val = BC.b; break;
		case RRC_C:
			outbit &= BC.c; BC.c >>= 1; outbit <<= 7; BC.c += outbit; val = BC.c; break;
		case RRC_D:
			outbit &= DE.d; DE.d >>= 1; outbit <<= 7; DE.d += outbit; val = DE.d; break;
		case RRC_E:
			outbit &= DE.e; DE.e >>= 1; outbit <<= 7; DE.e += outbit; val = DE.e; break;
		case RRC_H:
			outbit &= HL.h; HL.h >>= 1; outbit <<= 7; HL.h += outbit; val = HL.h; break;
		case RRC_L:
			outbit &= HL.l; HL.l >>= 1; outbit <<= 7; HL.l += outbit; val = HL.l; break;
		case RRC_HL:
			val = ReadMem(HL.hl);
			outbit &= val; val >>= 1; outbit <<= 7; val += outbit;
			WriteMem(HL.hl, val); break;
	}
	
	if(outbit) {
		setFlag(C);
	}
	if(val == 0) {
		setFlag(Z);
	}
}
void RR(uint8_t opcode) {
	uint8_t oldC = getFlag(C);
	resetFlag(Z);
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	uint8_t outbit = 1;
	uint8_t val = 0;
	
	switch(opcode) {
		case RR_A:
			outbit &= AF.a; AF.a >>= 1; outbit <<= 7; AF.a += oldC; val = AF.a; break;
		case RR_B:
			outbit &= BC.b; BC.b >>= 1; outbit <<= 7; BC.b += oldC; val = BC.b; break;
		case RR_C:
			outbit &= BC.c; BC.c >>= 1; outbit <<= 7; BC.c += oldC; val = BC.c; break;
		case RR_D:
			outbit &= DE.d; DE.d >>= 1; outbit <<= 7; DE.d += oldC; val = DE.d; break;
		case RR_E:
			outbit &= DE.e; DE.e >>= 1; outbit <<= 7; DE.e += oldC; val = DE.e; break;
		case RR_H:
			outbit &= HL.h; HL.h >>= 1; outbit <<= 7; HL.h += oldC; val = HL.h; break;
		case RR_L:
			outbit &= HL.l; HL.l >>= 1; outbit <<= 7; HL.l += oldC; val = HL.l; break;
		case RR_HL:
			val = ReadMem(HL.hl);
			outbit &= val; val >>= 1; outbit <<= 7; val += oldC;
			WriteMem(HL.hl, val); break;
	}
	
	if(outbit) {
		setFlag(C);
	}
	if(val == 0) {
		setFlag(Z);
	}
}
void SRL(uint8_t opcode) {
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	resetFlag(Z);
	uint8_t val = 0;
	
	switch(opcode) {
		case SRL_A:
			if(AF.a & 1) { setFlag(C); }; AF.a >>= 1; val = AF.a; break;
		case SRL_B:
			if(BC.b & 1) { setFlag(C); }; BC.b >>= 1; val = BC.b; break;
		case SRL_C:
			if(BC.c & 1) { setFlag(C); }; BC.c >>= 1; val = BC.c; break;
		case SRL_D:
			if(DE.d & 1) { setFlag(C); }; DE.d >>= 1; val = DE.d; break;
		case SRL_E:
			if(DE.e & 1) { setFlag(C); }; DE.e >>= 1; val = DE.e; break;
		case SRL_H:
			if(HL.h & 1) { setFlag(C); }; HL.h >>= 1; val = HL.h; break;
		case SRL_L:
			if(HL.l & 1) { setFlag(C); }; HL.l >>= 1; val = HL.l; break;
		case SRL_HL:
			val = ReadMem(HL.hl); if(val & 1) { setFlag(C); }; val >>= 1; WriteMem(HL.hl, val); break;
	}
	
	if(val == 0) {
		setFlag(Z);
	}
	
}
void SRA(uint8_t opcode) {
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	resetFlag(Z);
	uint8_t val = 0;
	uint8_t bit7 = 0;
	
	switch(opcode) {
		case SRA_A:
			if(AF.a & 1) { setFlag(C); }; bit7 = (AF.a & 0x80); AF.a &= 0x7f; AF.a >>= 1; AF.a ^= bit7; val = AF.a; break;
		case SRA_B:
			if(BC.b & 1) { setFlag(C); }; bit7 = (BC.b & 0x80); BC.b &= 0x7f; BC.b >>= 1; BC.b ^= bit7; val = BC.b; break;
		case SRA_C:
			if(BC.c & 1) { setFlag(C); }; bit7 = (BC.c & 0x80); BC.c &= 0x7f; BC.c >>= 1; BC.c ^= bit7; val = BC.c; break;
		case SRA_D:
			if(DE.d & 1) { setFlag(C); }; bit7 = (DE.d & 0x80); DE.d &= 0x7f; DE.d >>= 1; DE.d ^= bit7; val = DE.d; break;
		case SRA_E:
			if(DE.e & 1) { setFlag(C); }; bit7 = (DE.e & 0x80); DE.e &= 0x7f; DE.e >>= 1; DE.e ^= bit7; val = DE.e; break;
		case SRA_H:
			if(HL.h & 1) { setFlag(C); }; bit7 = (HL.h & 0x80); HL.h &= 0x7f; HL.h >>= 1; HL.h ^= bit7; val = HL.h; break;
		case SRA_L:
			if(HL.l & 1) { setFlag(C); }; bit7 = (HL.l & 0x80); HL.l &= 0x7f; HL.l >>= 1; HL.l ^= bit7; val = HL.l; break;
		case SRA_HL:
			val = ReadMem(HL.hl); if(val & 1) { setFlag(C); }; bit7 = (val & 0x80); val &= 0x7f; val >>= 1; val ^= bit7; WriteMem(HL.hl, val); break;
		default: 
			return;
	}
	
	if(val == 0) {
		setFlag(Z);
	}
}
void SLA(uint8_t opcode) {
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	resetFlag(Z);
	uint8_t val = 0;
	
	switch(opcode) {
		case SLA_A:
			if(AF.a & 0x80) { setFlag(C); }; AF.a <<= 1; val = AF.a; break;
		case SLA_B:
			if(BC.b & 0x80) { setFlag(C); }; BC.b <<= 1; val = BC.b; break;
		case SLA_C:
			if(BC.c & 0x80) { setFlag(C); }; BC.c <<= 1; val = BC.c; break;
		case SLA_D:
			if(DE.d & 0x80) { setFlag(C); }; DE.d <<= 1; val = DE.d; break;
		case SLA_E:
			if(DE.e & 0x80) { setFlag(C); }; DE.e <<= 1; val = DE.e; break;
		case SLA_H:
			if(HL.h & 0x80) { setFlag(C); }; HL.h <<= 1; val = HL.h; break;
		case SLA_L:
			if(HL.l & 0x80) { setFlag(C); }; HL.l <<= 1; val = HL.l; break;
		case SLA_HL:
			val = ReadMem(HL.hl); if(val & 0x80) { setFlag(C); }; val <<= 1; WriteMem(HL.hl, val); break;
	}
	
	if(val == 0) {
		setFlag(Z);
	}
}

void OR(uint8_t opcode, uint8_t param) {
	resetFlag(Z);
	resetFlag(N);
	resetFlag(H);
	resetFlag(C);
	
	switch(opcode) {
		case OR_A:
			AF.a = 0xff; break;
		case OR_B:
			AF.a |= BC.b; break;
		case OR_C:
			AF.a |= BC.c; break;
		case OR_D:
			AF.a |= DE.d; break;
		case OR_E:
			AF.a |= DE.e; break;
		case OR_H:
			AF.a |= HL.h; break;
		case OR_L:
			AF.a |= HL.l; break;
		case OR_HL:
			AF.a |= ReadMem(HL.hl); break;
		case OR_n:
			AF.a |= param; break;
	}
	
	if(AF.a == 0) { setFlag(Z); }
}

void SUB(uint8_t opcode, uint8_t param) {
	resetFlag(Z);
	setFlag(N);
	resetFlag(H);
	resetFlag(C);
	
	uint8_t val = 0;
	uint8_t oldA = AF.a;
	
	switch(opcode) {
		case SUB_A:
			val = AF.a; AF.a -= AF.a; break;
		case SUB_B:
			AF.a -= BC.b; val = BC.b; break;
		case SUB_C:
			AF.a -= BC.c; val = BC.c; break;
		case SUB_D:
			AF.a -= DE.d; val = DE.d; break;
		case SUB_E:
			AF.a -= DE.e; val = DE.e; break;
		case SUB_H:
			AF.a -= HL.h; val = HL.h; break;
		case SUB_L:
			AF.a -= HL.l; val = HL.l; break;
		case SUB_n:
			AF.a -= param; val = param; break;
		case SUB_HL:
			val = ReadMem(HL.hl); AF.a -= val; break;
	}
	
	if(AF.a == 0) { setFlag(Z); }
	
	// Half carry and full carry
	//printf("AF.a = 0x%x, val = 0x%x, (AF.a & 0xf) - (val & 0xf) = 0x%x\n", AF.a, val, (AF.a & 0xf) - (val & 0xf));
	if((oldA & 0xf) - (val & 0xf) < 0) {
		setFlag(H);
	}
	if(((oldA >> 4) & 0xf) - ((val >> 4) & 0xf) < 0) {
		setFlag(C);
	}
}
void SBC(uint8_t opcode, uint8_t param) {
	uint8_t cFlag = getFlag(C);
	resetFlag(Z);
	setFlag(N);
	resetFlag(H);
	resetFlag(C);
	
	uint8_t val = 0;
	
	switch(opcode) {
		case SBC_A_A:
			val = AF.a + cFlag; AF.a -= val; break;
		case SBC_A_B:
			val = BC.b + cFlag; AF.a -= val; break;
		case SBC_A_C:
			val = BC.c + cFlag; AF.a -= val; break;
		case SBC_A_D:
			val = DE.d + cFlag; AF.a -= val; break;
		case SBC_A_E:
			val = DE.e + cFlag; AF.a -= val; break;
		case SBC_A_H:
			val = HL.h + cFlag; AF.a -= val; break;
		case SBC_A_L:
			val = HL.l + cFlag; AF.a -= val; break;
		case SBC_A_HL:
			val = ReadMem(HL.hl) + cFlag; AF.a -= val; break;
		case SBC_A_n:
			val = param + cFlag; AF.a -= val; break;
	}
	
	if(AF.a == 0) { setFlag(Z); }
	
	// Half carry and full carry
	if((AF.a & 0xf) - (val & 0xf) < 0) {
		setFlag(H);
	}
	if(((AF.a >> 4) & 0xf) - ((val >> 4) & 0xf) < 0) {
		setFlag(C);
	}
}

void CP(uint8_t opcode, uint8_t param) {
	resetFlag(Z);
	resetFlag(C);
	resetFlag(N);
	resetFlag(H);
	uint8_t val = 0, res = 0;
	
	switch(opcode) {
		case CP_A:
			val = AF.a; res = AF.a - val; break;
		case CP_B:
			res = AF.a - BC.b; val = BC.b; break;
		case CP_C:
			res = AF.a - BC.c; val = BC.c; break;
		case CP_D:
			res = AF.a - DE.d; val = DE.d; break;
		case CP_E:
			res = AF.a - DE.e; val = DE.e; break;
		case CP_H:
			res = AF.a - HL.h; val = HL.h; break;
		case CP_L:
			res = AF.a - HL.l; val = HL.l; break;
		case CP_HL:
			val = ReadMem(HL.hl); res = AF.a - val; break;
	}
	
	if(res == 0) { setFlag(Z); }
	
	// Half carry and full carry
	if((res & 0xf) - (val & 0xf) < 0) {
		setFlag(H);
	}
	if(((res >> 4) & 0xf) - ((val >> 4) & 0xf) < 0) {
		setFlag(C);
	}
}
void RST(uint8_t opcode) {
	switch(opcode) {
		case RST_00H:
			PC = 0; break; 
		case RST_10H:
			PC = 0x10; break;
		case RST_20H:
			PC = 0x20; break;
		case RST_30H:
			PC = 0x30; break;
		case RST_08H:
			PC = 0x08; break;
		case RST_18H:
			PC = 0x18; break;
		case RST_28H:
			PC = 0x28; break;
		case RST_38H:
			PC = 0x38; break;
			
	}
}

void PUSH(uint8_t opcode) {
	switch(opcode) {
		case PUSH_AF:
			WriteMem(SP, AF.f); WriteMem(SP-1, AF.a); SP -= 2; break;
		case PUSH_DE:
			WriteMem(SP, DE.e); WriteMem(SP-1, DE.d); SP -= 2; break;
		case PUSH_BC:
			WriteMem(SP, BC.c); WriteMem(SP-1, BC.b); SP -= 2; break;
		case PUSH_HL:
			WriteMem(SP, HL.l); WriteMem(SP-1, HL.h); SP -= 2; break;
	}
}

void POP(uint8_t opcode) {
	switch(opcode) {
		case POP_AF:
			AF.f = ReadMem(SP); AF.a = ReadMem(SP+1); SP += 2; break;
		case POP_DE:
			DE.e = ReadMem(SP); DE.d = ReadMem(SP+1); SP += 2; break;
		case POP_BC:
			BC.c = ReadMem(SP); BC.b = ReadMem(SP+1); SP += 2; break;
		case POP_HL:
			HL.l = ReadMem(SP); HL.h = ReadMem(SP+1); SP += 2; break;
	}
}
 
void DAA_() {
	uint8_t hFlag = getFlag(H);
	uint8_t cFlag = getFlag(C);
	uint8_t nFlag = getFlag(N);
	
	resetFlag(Z);
	resetFlag(C);
	resetFlag(H);
	
	if(nFlag) { // After subtraction
		if (hFlag && !cFlag && (AF.a & 0xf) > 5 && ((AF.a >> 4) & 0xf) < 9) {
			if(AF.a + 0xfa < 0) { setFlag(C); }
			AF.a += 0xfa;			
		} else if (cFlag && !hFlag && (AF.a & 0xf) < 0xa && ((AF.a >> 4) & 0xf) > 6 ) {
			if(AF.a + 0xa0 < 0) { setFlag(C); }
			AF.a += 0xa0;
		} else if (cFlag && hFlag && (AF.a & 0xf) > 5 && ((AF.a >> 4) & 0xf) > 5 ) {
			if(AF.a + 0x9a < 0) { setFlag(C); }
			AF.a += 0x9a;
		} 
	} else {
		if(hFlag || (AF.a & 0xf) > 9) {
			if(AF.a + 0x06 > 0x100) { setFlag(C); }
			AF.a += 0x6;
		}
		if(cFlag || ((AF.a >> 4) & 0xf) > 9) {
			if(AF.a + 0x60 > 0x100) { setFlag(C); }
			AF.a += 0x60;
		}	
	}
	
	if(AF.a == 0) {
		setFlag(Z);
	}
} 
 
 // Figure out how many paramters the given instruction requires
short GetParameters(uint8_t opcode) {
	short numParams = 0;

	switch(opcode) {
		case XOR_n:
		case LD_A_n:
		case LD_B_n:
		case LD_C_n:
		case LD_D_n:
		case LD_E_n:
		case LD_H_n:
		case LD_L_n:
		case LD__HL__n:
		case LD_HL_SP_n:
		case CB:
		case JR_NZ_n:
		case JR_NC_n:
		case JR_Z_n:
		case JR_C_n:
		case JR_n:
		case ADD_SP_n:
		case ADC_A_n:
		case SUB_n:
		case SBC_A_n:
		case AND_n:
		case OR_n:
		case CP_n:
		case ADD_A_n:
		case LDH_n_A:
		case LDH_A_n:
			numParams = 1; break;
		case LD_BC_nn:
		case LD_DE_nn:
		case LD_HL_nn:
		case LD_SP_nn:
		case LD_nn_SP:
		case LD_nn_A:
		case LD_A_nn:
		case JP_NZ_nn:
		case JP_NC_nn:
		case JP_Z_nn:
		case JP_C_nn:
		case JP_nn:
		case CALL_NC_nn:
		case CALL_NZ_nn:
		case CALL_Z_nn:
		case CALL_C_nn:
		case CALL_nn:
			numParams = 2; break;
			
	}
	 
	return numParams;
}

uint16_t GetCycles(opcode) {
	uint16_t cycles = 4;
	
	switch(opcode) {
		case LD_BC_nn:
		case LD_DE_nn:
		case LD_HL_nn:
		case LD_SP_nn:
		case JR_n:
			cycles = 12; break;
		case LD_BC_A:
		case LD_DE_A:
		case LD_HLM_A:
		case LD_HLP_A:
		case LD_A_BC:
		case LD_A_DE:
		case LD_A_HLP:
		case LD_A_HLM:
		case INC_BC:
		case INC_DE:
		case INC_HL:
		case INC_SP:
		case DEC_BC:
		case DEC_DE:
		case DEC_HL:
		case DEC_SP:
		case LD_A_n:
		case LD_B_n:
		case LD_C_n:
		case LD_D_n:
		case LD_E_n:
		case LD_H_n:
		case LD_L_n:
		case LD__HL__n:
		case ADD_HL_BC:
		case ADD_HL_DE:
		case ADD_HL_HL:
		case ADD_HL_SP:
			cycles = 8; break;
		case JR_Z_n:
		case JR_C_n:
		case JR_NZ_n:
		case JR_NC_n:
			cycles = 0x080c;
		
	}
	
	return cycles;
}
 
const char* CodeToString(uint8_t opcode) {

	printf("%04x ", PC);
	const char *name;
	
	switch(opcode) {
		case CB:
			name = "CB %s"; break;
		case NOP:
			name = "NOP"; break;
		case DAA:
			name = "DAA"; break;
		case LD_A_A:
			name = "LD A, A"; break;
		case LD_B_B:
			name = "LD B, B"; break;
		case LD_C_C:
			name = "LD C, C"; break;
		case LD_D_D:
			name = "LD D, D"; break;
		case LD_E_E:
			name = "LD E, E"; break;
		case LD_H_H:
			name = "LD H, H"; break;
		case LD_L_L:
			name = "LD L, L"; break; 
		case LD_A_B:
			name = "LD A, B"; break;
		case LD_A_C:
			name = "LD A, C"; break;
		case LD_A_D:
			name = "LD A, D"; break;
		case LD_A_E:
			name = "LD A, E"; break;
		case LD_A_H:
			name = "LD A, H"; break;
		case LD_A_L:
			name = "LD A, L"; break;
		case LD_A_HL:
			name = "LD A, (HL)"; break;
		case LD_B_A:
			name = "LD B, A"; break;
		case LD_B_C:
			name = "LD B, C"; break;
		case LD_B_D:
			name = "LD B, D"; break;
		case LD_B_E:
			name = "LD B, E"; break;
		case LD_B_H:
			name = "LD B, H"; break;
		case LD_B_L:
			name = "LD B, L"; break;
		case LD_B_HL:
			name = "LD B, (HL)"; break;
		case LD_C_A:
			name = "LD C, A"; break;
		case LD_C_B:
			name = "LD C, B"; break;
		case LD_C_D:
			name = "LD C, D"; break;
		case LD_C_E:
			name = "LD C, E"; break;
		case LD_C_H:
			name = "LD C, H"; break;
		case LD_C_L:
			name = "LD C, L"; break;
		case LD_C_HL:
			name = "LD C, (HL)"; break;
		case LD_D_A:
			name = "LD D, A"; break;
		case LD_D_B:
			name = "LD D, B"; break;
		case LD_D_C:
			name = "LD D, C"; break;
		case LD_D_E:
			name = "LD D, E"; break;
		case LD_D_H:
			name = "LD D, H"; break;
		case LD_D_L:
			name = "LD D, L"; break;
		case LD_D_HL:
			name = "LD D, (HL)"; break;
		case LD_E_A:
			name = "LD E, A"; break;
		case LD_E_B:
			name = "LD E, B"; break;
		case LD_E_C:
			name = "LD E, C"; break;
		case LD_E_D:
			name = "LD E, D"; break;
		case LD_E_H:
			name = "LD E, H"; break;
		case LD_E_L:
			name = "LD E, L"; break;
		case LD_E_HL:
			name = "LD E, (HL)"; break;
		case LD_H_A:
			name = "LD H, A"; break;
		case LD_H_B:
			name = "LD H, B"; break;
		case LD_H_C:
			name = "LD H, C"; break;
		case LD_H_D:
			name = "LD H, D"; break;
		case LD_H_E:
			name = "LD H, E"; break;
		case LD_H_L:
			name = "LD H, L"; break;
		case LD_H_HL:
			name = "LD H, (HL)"; break;
		case LD_L_A:
			name = "LD L, A"; break;
		case LD_L_B:
			name = "LD L, B"; break;
		case LD_L_C:
			name = "LD L, C"; break;
		case LD_L_D:
			name = "LD L, D"; break;
		case LD_L_E:
			name = "LD L, E"; break;
		case LD_L_H:
			name = "LD L, H"; break;
		case LD_L_HL:
			name = "LD L, (HL)"; break;
		case LD_HL_A:
			name = "LD (HL), A"; break;
		case LD_HL_B:
			name = "LD (HL), B"; break;
		case LD_HL_C:
			name = "LD (HL), C"; break;
		case LD_HL_D:
			name = "LD (HL), D"; break;
		case LD_HL_E:
			name = "LD (HL), E"; break;
		case LD_HL_H:
			name = "LD (HL), H"; break;
		case LD_HL_L:
			name = "LD (HL), L"; break;
		case LD__HL__n:
			name = "LD (HL), %02x"; break;
		case LD_BC_nn:
			name = "LD BC, %02x %02x"; break;
		case LD_DE_nn:
			name = "LD DE, %02x %02x"; break;
		case LD_HL_nn:
			name = "LD HL, %02x %02x"; break;
		case LD_SP_nn:
			name = "LD SP, %02x %02x"; break;
		case LD_nn_SP:
			name = "LD %02x %02x, SP"; break;
		case LD_BC_A:
			name = "LD (BC), A"; break;
		case LD_DE_A:
			name = "LD (DE), A"; break;
		case LD_HLP_A:
			name = "LD (HL+), A"; break;
		case LD_HLM_A:
			name = "LD (HL-), A"; break;
		case LD_B_n:
			name = "LD B, %x"; break;
		case LD_C_n:
			name = "LD C, %x"; break;
		case LD_D_n:
			name = "LD D, %x"; break;
		case LD_E_n:
			name = "LD E, %x"; break;
		case LD_H_n:
			name = "LD H, %x"; break;
		case LD_L_n:
			name = "LD L, %x"; break;
		case LD_A_n:
			name = "LD A, %x"; break;
		case LD_A_BC:
			name = "LD A, (BC)"; break;
		case LD_A_DE:
			name = "LD A, (DE)"; break;
		case LD_A_HLP:
			name = "LD A, (HL+)"; break;
		case LD_A_HLM:
			name = "LD A, (HL-)"; break;
		case LD__C__A:
			name = "LD (C), A"; break;
		case LD_A__C_:
			name = "LD A, (C)"; break;
		case LDH_n_A:
			name = "LDH (%x), A"; break;
		case LDH_A_n:
			name = "LDH A, (%x)"; break;
		case LD_nn_A:
			name = "LD (%x%x), A"; break;
		case LD_A_nn:
			name = "LD A, %x %x"; break;
		case LD_HL_SP_n:
			name = "LD HL, SP+%x"; break;
		case LD_SP_HL:
			name = "LD SP, HL"; break;
		case AND_A:
			name = "AND A, A"; break;
		case AND_B:
			name = "AND A, B"; break;
		case AND_C:
			name = "AND A, C"; break;
		case AND_D:
			name = "AND A, D"; break;
		case AND_E:
			name = "AND A, E"; break;
		case AND_H:
			name = "AND A, H"; break;
		case AND_L:
			name = "AND A, L"; break;
		case AND_HL:
			name = "AND A, (HL)"; break;
		case AND_n:
			name = "AND %x"; break;
		case XOR_A:
			name = "XOR A"; break;
		case XOR_B:
			name = "XOR B"; break;
		case XOR_C:
			name = "XOR C"; break;
		case XOR_D:
			name = "XOR D"; break;
		case XOR_E:
			name = "XOR E"; break;
		case XOR_H:
			name = "XOR H"; break;
		case XOR_L:
			name = "XOR L"; break;
		case XOR_HL:
			name = "XOR (HL)"; break;
		case XOR_n:
			name = "XOR %02x"; break;
		case OR_A:
			name = "OR A"; break;
		case OR_B:
			name = "OR B"; break;
		case OR_C:
			name = "OR C"; break;
		case OR_D:
			name = "OR D"; break;
		case OR_E:
			name = "OR E"; break;
		case OR_H:
			name = "OR H"; break;
		case OR_L:
			name = "OR L"; break;
		case OR_HL:
			name = "OR (HL)"; break;
		case OR_n:
			name = "OR %x"; break;
		case ADD_A_A:
			name = "ADD A, A"; break;
		case ADD_A_B:
			name = "ADD A, B"; break;
		case ADD_A_C:
			name = "ADD A, C"; break;
		case ADD_A_D:
			name = "ADD A, D"; break;
		case ADD_A_E:
			name = "ADD A, E"; break;
		case ADD_A_H:
			name = "ADD A, H"; break;
		case ADD_A_L:
			name = "ADD A, L"; break;
		case ADD_A_HL:
			name = "ADD A, (HL)"; break;
		case ADD_SP_n:
			name = "ADD SP, %x"; break;
		case ADD_HL_BC:
			name = "ADD HL, BC"; break;
		case ADD_HL_DE:
			name = "ADD HL, DE"; break;
		case ADD_HL_HL:
			name = "ADD HL, HL"; break;
		case ADD_HL_SP:
			name = "ADD HL, SP"; break;
		case ADD_A_n:
			name = "ADD A, %x"; break;
		case ADC_A_A:
			name = "ADC A, A"; break;
		case ADC_A_B:
			name = "ADC A, B"; break;
		case ADC_A_C:
			name = "ADC A, C"; break;
		case ADC_A_D:
			name = "ADC A, D"; break;
		case ADC_A_E:
			name = "ADC A, E"; break;
		case ADC_A_H:
			name = "ADC A, H"; break;
		case ADC_A_L:
			name = "ADC A, L"; break;
		case ADC_A_HL:
			name = "ADC A, (HL)"; break;
		case ADC_A_n:
			name = "ADC A, %x"; break;
		case SUB_A:
			name = "SUB A"; break;
		case SUB_B:
			name = "SUB B"; break;
		case SUB_C:
			name = "SUB C"; break;
		case SUB_D:
			name = "SUB D"; break;
		case SUB_E:
			name = "SUB E"; break;
		case SUB_H:
			name = "SUB H"; break;
		case SUB_L:
			name = "SUB L"; break;
		case SUB_HL:
			name = "SUB (HL)"; break;
		case SUB_n:
			name = "SUB %x"; break;
		case SBC_A_A:
			name = "SBC A, A"; break;
		case SBC_A_B:
			name = "SBC A, B"; break;
		case SBC_A_C:
			name = "SBC A, C"; break;
		case SBC_A_D:
			name = "SBC A, D"; break;
		case SBC_A_E:
			name = "SBC A, E"; break;
		case SBC_A_H:
			name = "SBC A, H"; break;
		case SBC_A_L:
			name = "SBC A, L"; break;
		case SBC_A_HL:
			name = "SBC A, (HL)"; break;
		case SBC_A_n:
			name = "SBC A, %x"; break;
		case CP_A:
			name = "CP A"; break;
		case CP_B:
			name = "CP B"; break;
		case CP_C:
			name = "CP C"; break;
		case CP_D:
			name = "CP D"; break;
		case CP_E:
			name = "CP E"; break;
		case CP_H:
			name = "CP H"; break;
		case CP_L:
			name = "CP L"; break;
		case CP_HL:
			name = "CP (HL)"; break;
		case CP_n:
			name = "CP %x"; break;
		case INC_A:
			name = "INC A"; break;
		case INC_B:
			name = "INC B";  break;
		case INC_C:
			name = "INC C";  break;
		case INC_D:
			name = "INC D"; break;
		case INC_E:
			name = "INC E";  break;
		case INC_H:
			name = "INC H"; break;
		case INC_L:
			name = "INC L";  break;
		case INC__HL_:
			name = "INC (HL)"; break;
		case INC_BC:
			name = "INC BC"; break;
		case INC_DE:
			name = "INC DE";break;
		case INC_HL:
			name = "INC HL"; break;
		case INC_SP:
			name = "INC SP"; break;
		case DEC_A:
			name = "DEC A"; break;
		case DEC_B:
			name = "DEC B";  break;
		case DEC_C:
			name = "DEC C";  break;
		case DEC_D:
			name = "DEC D"; break;
		case DEC_E:
			name = "DEC E";  break;
		case DEC_H:
			name = "DEC H"; break;
		case DEC_L:
			name = "DEC L";  break;
		case DEC__HL_:
			name = "DEC (HL)"; break;
		case DEC_BC:
			name = "DEC BC"; break;
		case DEC_DE:
			name = "DEC DE";break;
		case DEC_HL:
			name = "DEC HL"; break;
		case DEC_SP:
			name = "DEC SP"; break;
		case PUSH_AF:
			name = "PUSH AF"; break;
		case PUSH_DE:
			name = "PUSH DE"; break;
		case PUSH_BC:
			name = "PUSH BC"; break;
		case PUSH_HL:
			name = "PUSH HL"; break;
		case POP_AF:
			name = "POP AF"; break;
		case POP_DE:
			name = "POP DE"; break;
		case POP_BC:
			name = "POP BC"; break;
		case POP_HL:
			name = "POP HL"; break;
		case JR_NZ_n:
			name = "JR NZ, %x"; break;
		case JR_NC_n:
			name = "JR NC, %x"; break;
		case JR_Z_n:
			name = "JR Z, %x"; break;
		case JR_C_n:
			name = "JR C, %x"; break;
		case JR_n:
			name = "JR %x"; break;
		case JP_NC_nn:
			name = "JP NC, %x %x"; break;
		case JP_NZ_nn:
			name = "JP NZ, %x %x"; break;
		case JP_C_nn:
			name = "JP C, %x %x"; break;
		case JP_Z_nn:
			name = "JP Z, %x %x"; break;
		case JP_nn:
			name = "JP %x %x"; break;
		case JP_HL:
			name = "JP (HL)"; break;
		case CALL_NC_nn:
			name = "CALL NC, %x %x"; break;
		case CALL_NZ_nn:
			name = "CALL NZ, %x %x"; break;
		case CALL_Z_nn:
			name = "CALL Z, %x %x"; break;
		case CALL_C_nn:
			name = "CALL C, %x %x"; break;
		case CALL_nn:
			name = "CALL %x %x"; break;
		case RET_NC:
			name = "RET NC"; break;
		case RET_NZ:
			name = "RET NZ"; break;
		case RET_Z:
			name = "RET Z"; break;
		case RET_C:
			name = "RET C"; break;
		case RET:
			name = "RET"; break;
		case RETI:
			name = "RETI"; break;
		case RST_00H:
			name = "RST 00H"; break; 
		case RST_10H:
			name = "RST 10H"; break; 
		case RST_20H:
			name = "RST 20H"; break; 
		case RST_30H:
			name = "RST 30H"; break; 
		case RST_08H:
			name = "RST 08H"; break; 
		case RST_18H:
			name = "RST 18H"; break; 
		case RST_28H:
			name = "RST 28H"; break; 
		case RST_38H:
			name = "RST 38H"; break; 
		case RRCA:
			name = "RRCA"; break;
		case RRA:
			name = "RRA"; break;
		case RLCA:
			name = "RLCA"; break;
		case RLA:
			name = "RLA"; break;
		case SCF:
			name = "SCF"; break;
		case CCF:
			name = "CCF"; break;
		case CPL:
			name = "CPL"; break;
		case HALT:
			name = "HALT"; break;
		case STOP:
			name = "STOP"; break;
		case DI:
			name = "DI"; break;
		case EI:
			name = "EI"; break;
		default:
			name = "Undefined"; break;
	}
	return name;
}
const char* CBCodeToString(uint8_t opcode) {
	const char *name;
	
	switch(opcode) {
		case SWAP_A:
			name = "SWAP A"; break;
		case SWAP_B:
			name = "SWAP B"; break;
		case SWAP_C:
			name = "SWAP C"; break;
		case SWAP_D:
			name = "SWAP D"; break;
		case SWAP_E:
			name = "SWAP E"; break;
		case SWAP_H:
			name = "SWAP H"; break;
		case SWAP_L:
			name = "SWAP L"; break;
		case SWAP_HL:
			name = "SWAP (HL)"; break;
		case BIT0_B:
			name = "BIT 0, B"; break;
		case BIT0_C:
			name = "BIT 0, C"; break;
		case BIT0_D:
			name = "BIT 0, D"; break;
		case BIT0_E:
			name = "BIT 0, E"; break;
		case BIT0_H:
			name = "BIT 0, H"; break;
		case BIT0_L:
			name = "BIT 0, L"; break;
		case BIT0_HL:
			name = "BIT 0, (HL)"; break;
		case BIT0_A:
			name = "BIT 0, A"; break;
		case BIT1_B:
			name = "BIT 1, B"; break;
		case BIT1_C:
			name = "BIT 1, C"; break;
		case BIT1_D:
			name = "BIT 1, D"; break;
		case BIT1_E:
			name = "BIT 1, E"; break;
		case BIT1_H:
			name = "BIT 1, H"; break;
		case BIT1_L:
			name = "BIT 1, L"; break;
		case BIT1_HL:
			name = "BIT 1, (HL)"; break;
		case BIT1_A:
			name = "BIT 1, A"; break;
		case BIT2_B:
			name = "BIT 2, B"; break;
		case BIT2_C:
			name = "BIT 2, C"; break;
		case BIT2_D:
			name = "BIT 2, D"; break;
		case BIT2_E:
			name = "BIT 2, E"; break;
		case BIT2_H:
			name = "BIT 2, H"; break;
		case BIT2_L:
			name = "BIT 2, L"; break;
		case BIT2_HL:
			name = "BIT 2, (HL)"; break;
		case BIT2_A:
			name = "BIT 2, A"; break;
		case BIT3_B:
			name = "BIT 3, B"; break;
		case BIT3_C:
			name = "BIT 3, C"; break;
		case BIT3_D:
			name = "BIT 3, D"; break;
		case BIT3_E:
			name = "BIT 3, E"; break;
		case BIT3_H:
			name = "BIT 3, H"; break;
		case BIT3_L:
			name = "BIT 3, L"; break;
		case BIT3_HL:
			name = "BIT 3, (HL)"; break;
		case BIT3_A:
			name = "BIT 3, A"; break;
		case BIT4_B:
			name = "BIT 4, B"; break;
		case BIT4_C:
			name = "BIT 4, C"; break;
		case BIT4_D:
			name = "BIT 4, D"; break;
		case BIT4_E:
			name = "BIT 4, E"; break;
		case BIT4_H:
			name = "BIT 4, H"; break;
		case BIT4_L:
			name = "BIT 4, L"; break;
		case BIT4_HL:
			name = "BIT 4, (HL)"; break;
		case BIT4_A:
			name = "BIT 4, A"; break;
		case BIT5_B:
			name = "BIT 5, B"; break;
		case BIT5_C:
			name = "BIT 5, C"; break;
		case BIT5_D:
			name = "BIT 5, D"; break;
		case BIT5_E:
			name = "BIT 5, E"; break;
		case BIT5_H:
			name = "BIT 5, H"; break;
		case BIT5_L:
			name = "BIT 5, L"; break;
		case BIT5_HL:
			name = "BIT 5, (HL)"; break;
		case BIT5_A:
			name = "BIT 5, A"; break;
		case BIT6_B:
			name = "BIT 6, B"; break;
		case BIT6_C:
			name = "BIT 6, C"; break;
		case BIT6_D:
			name = "BIT 6, D"; break;
		case BIT6_E:
			name = "BIT 6, E"; break;
		case BIT6_H:
			name = "BIT 6, H"; break;
		case BIT6_L:
			name = "BIT 6, L"; break;
		case BIT6_HL:
			name = "BIT 6, (HL)"; break;
		case BIT6_A:
			name = "BIT 6, A"; break;
		case BIT7_B:
			name = "BIT 7, B"; break;
		case BIT7_C:
			name = "BIT 7, C"; break;
		case BIT7_D:
			name = "BIT 7, D"; break;
		case BIT7_E:
			name = "BIT 7, E"; break;
		case BIT7_H:
			name = "BIT 7, H"; break;
		case BIT7_L:
			name = "BIT 7, L"; break;
		case BIT7_HL:
			name = "BIT 7, (HL)"; break;
		case BIT7_A:
			name = "BIT 7, A"; break;
		case RES0_B:
			name = "RES 0, B"; break;
		case RES0_C:
			name = "RES 0, C"; break;
		case RES0_D:
			name = "RES 0, D"; break;
		case RES0_E:
			name = "RES 0, E"; break;
		case RES0_H:
			name = "RES 0, H"; break;
		case RES0_L:
			name = "RES 0, L"; break;
		case RES0_HL:
			name = "RES 0, (HL)"; break;
		case RES0_A:
			name = "RES 0, A"; break;
		case RES1_B:
			name = "RES 1, B"; break;
		case RES1_C:
			name = "RES 1, C"; break;
		case RES1_D:
			name = "RES 1, D"; break;
		case RES1_E:
			name = "RES 1, E"; break;
		case RES1_H:
			name = "RES 1, H"; break;
		case RES1_L:
			name = "RES 1, L"; break;
		case RES1_HL:
			name = "RES 1, (HL)"; break;
		case RES1_A:
			name = "RES 1, A"; break;
		case RES2_B:
			name = "RES 2, B"; break;
		case RES2_C:
			name = "RES 2, C"; break;
		case RES2_D:
			name = "RES 2, D"; break;
		case RES2_E:
			name = "RES 2, E"; break;
		case RES2_H:
			name = "RES 2, H"; break;
		case RES2_L:
			name = "RES 2, L"; break;
		case RES2_HL:
			name = "RES 2, (HL)"; break;
		case RES2_A:
			name = "RES 2, A"; break;
		case RES3_B:
			name = "RES 3, B"; break;
		case RES3_C:
			name = "RES 3, C"; break;
		case RES3_D:
			name = "RES 3, D"; break;
		case RES3_E:
			name = "RES 3, E"; break;
		case RES3_H:
			name = "RES 3, H"; break;
		case RES3_L:
			name = "RES 3, L"; break;
		case RES3_HL:
			name = "RES 3, (HL)"; break;
		case RES3_A:
			name = "RES 3, A"; break;
		case RES4_B:
			name = "RES 4, B"; break;
		case RES4_C:
			name = "RES 4, C"; break;
		case RES4_D:
			name = "RES 4, D"; break;
		case RES4_E:
			name = "RES 4, E"; break;
		case RES4_H:
			name = "RES 4, H"; break;
		case RES4_L:
			name = "RES 4, L"; break;
		case RES4_HL:
			name = "RES 4, (HL)"; break;
		case RES4_A:
			name = "RES 4, A"; break;
		case RES5_B:
			name = "RES 5, B"; break;
		case RES5_C:
			name = "RES 5, C"; break;
		case RES5_D:
			name = "RES 5, D"; break;
		case RES5_E:
			name = "RES 5, E"; break;
		case RES5_H:
			name = "RES 5, H"; break;
		case RES5_L:
			name = "RES 5, L"; break;
		case RES5_HL:
			name = "RES 5, (HL)"; break;
		case RES5_A:
			name = "RES 5, A"; break;
		case RES6_B:
			name = "RES 6, B"; break;
		case RES6_C:
			name = "RES 6, C"; break;
		case RES6_D:
			name = "RES 6, D"; break;
		case RES6_E:
			name = "RES 6, E"; break;
		case RES6_H:
			name = "RES 6, H"; break;
		case RES6_L:
			name = "RES 6, L"; break;
		case RES6_HL:
			name = "RES 6, (HL)"; break;
		case RES6_A:
			name = "RES 6, A"; break;
		case RES7_B:
			name = "RES 7, B"; break;
		case RES7_C:
			name = "RES 7, C"; break;
		case RES7_D:
			name = "RES 7, D"; break;
		case RES7_E:
			name = "RES 7, E"; break;
		case RES7_H:
			name = "RES 7, H"; break;
		case RES7_L:
			name = "RES 7, L"; break;
		case RES7_HL:
			name = "RES 7, (HL)"; break;
		case RES7_A:
			name = "RES 7, A"; break;
		case SET0_B:
			name = "SET 0, B"; break;
		case SET0_C:
			name = "SET 0, C"; break;
		case SET0_D:
			name = "SET 0, D"; break;
		case SET0_E:
			name = "SET 0, E"; break;
		case SET0_H:
			name = "SET 0, H"; break;
		case SET0_L:
			name = "SET 0, L"; break;
		case SET0_HL:
			name = "SET 0, (HL)"; break;
		case SET0_A:
			name = "SET 0, A"; break;
		case SET1_B:
			name = "SET 1, B"; break;
		case SET1_C:
			name = "SET 1, C"; break;
		case SET1_D:
			name = "SET 1, D"; break;
		case SET1_E:
			name = "SET 1, E"; break;
		case SET1_H:
			name = "SET 1, H"; break;
		case SET1_L:
			name = "SET 1, L"; break;
		case SET1_HL:
			name = "SET 1, (HL)"; break;
		case SET1_A:
			name = "SET 1, A"; break;
		case SET2_B:
			name = "SET 2, B"; break;
		case SET2_C:
			name = "SET 2, C"; break;
		case SET2_D:
			name = "SET 2, D"; break;
		case SET2_E:
			name = "SET 2, E"; break;
		case SET2_H:
			name = "SET 2, H"; break;
		case SET2_L:
			name = "SET 2, L"; break;
		case SET2_HL:
			name = "SET 2, (HL)"; break;
		case SET2_A:
			name = "SET 2, A"; break;
		case SET3_B:
			name = "SET 3, B"; break;
		case SET3_C:
			name = "SET 3, C"; break;
		case SET3_D:
			name = "SET 3, D"; break;
		case SET3_E:
			name = "SET 3, E"; break;
		case SET3_H:
			name = "SET 3, H"; break;
		case SET3_L:
			name = "SET 3, L"; break;
		case SET3_HL:
			name = "SET 3, (HL)"; break;
		case SET3_A:
			name = "SET 3, A"; break;
		case SET4_B:
			name = "SET 4, B"; break;
		case SET4_C:
			name = "SET 4, C"; break;
		case SET4_D:
			name = "SET 4, D"; break;
		case SET4_E:
			name = "SET 4, E"; break;
		case SET4_H:
			name = "SET 4, H"; break;
		case SET4_L:
			name = "SET 4, L"; break;
		case SET4_HL:
			name = "SET 4, (HL)"; break;
		case SET4_A:
			name = "SET 4, A"; break;
		case SET5_B:
			name = "SET 5, B"; break;
		case SET5_C:
			name = "SET 5, C"; break;
		case SET5_D:
			name = "SET 5, D"; break;
		case SET5_E:
			name = "SET 5, E"; break;
		case SET5_H:
			name = "SET 5, H"; break;
		case SET5_L:
			name = "SET 5, L"; break;
		case SET5_HL:
			name = "SET 5, (HL)"; break;
		case SET5_A:
			name = "SET 5, A"; break;
		case SET6_B:
			name = "SET 6, B"; break;
		case SET6_C:
			name = "SET 6, C"; break;
		case SET6_D:
			name = "SET 6, D"; break;
		case SET6_E:
			name = "SET 6, E"; break;
		case SET6_H:
			name = "SET 6, H"; break;
		case SET6_L:
			name = "SET 6, L"; break;
		case SET6_HL:
			name = "SET 6, (HL)"; break;
		case SET6_A:
			name = "SET 6, A"; break;
		case SET7_B:
			name = "SET 7, B"; break;
		case SET7_C:
			name = "SET 7, C"; break;
		case SET7_D:
			name = "SET 7, D"; break;
		case SET7_E:
			name = "SET 7, E"; break;
		case SET7_H:
			name = "SET 7, H"; break;
		case SET7_L:
			name = "SET 7, L"; break;
		case SET7_HL:
			name = "SET 7, (HL)"; break;
		case SET7_A:
			name = "SET 7, A"; break;
		case RLC_B:
			name = "RLC B"; break;
		case RLC_C:
			name = "RLC C"; break;
		case RLC_D:
			name = "RLC D"; break;
		case RLC_E:
			name = "RLC E"; break;
		case RLC_H:
			name = "RLC H"; break;
		case RLC_L:
			name = "RLC L"; break;
		case RLC_HL:
			name = "RLC (HL)"; break;
		case RLC_A:
			name = "RLC A"; break;
		case RRC_B:
			name = "RRC B"; break;
		case RRC_C:
			name = "RRC B"; break;
		case RRC_D:
			name = "RRC C"; break;
		case RRC_E:
			name = "RRC E"; break;
		case RRC_H:
			name = "RRC H"; break;
		case RRC_L:
			name = "RRC L"; break;
		case RRC_HL:
			name = "RRC (HL)"; break;
		case RRC_A:
			name = "RRC A"; break;
		case RL_A:
			name = "RL A"; break;
		case RL_B:
			name = "RL B"; break;
		case RL_C:
			name = "RL C"; break;
		case RL_D:
			name = "RL D"; break;
		case RL_E:
			name = "RL E"; break;
		case RL_H:
			name = "RL H"; break;
		case RL_L:
			name = "RL L"; break;
		case RL_HL:
			name = "RL (HL)"; break;
		case RR_A:
			name = "RR A"; break;
		case RR_B:
			name = "RR B"; break;
		case RR_C:
			name = "RR C"; break;
		case RR_D:
			name = "RR D"; break;
		case RR_E:
			name = "RR E"; break;
		case RR_H:
			name = "RR H"; break;
		case RR_L:
			name = "RR L"; break;
		case RR_HL:
			name = "RR (HL)"; break;
		case SRA_A:
			name = "SRA A"; break;
		case SRA_B:
			name = "SRA B"; break;
		case SRA_C:
			name = "SRA C"; break;
		case SRA_D:
			name = "SRA D"; break;
		case SRA_E:
			name = "SRA E"; break;
		case SRA_L:
			name = "SRA H"; break;
		case SRA_H:
			name = "SRA L"; break;
		case SRA_HL:
			name = "SRA (HL)"; break;
		case SLA_A:
			name = "SLA A"; break;
		case SLA_B:
			name = "SLA B"; break;
		case SLA_C:
			name = "SLA C"; break;
		case SLA_D:
			name = "SLA D"; break;
		case SLA_E:
			name = "SLA E"; break;
		case SLA_L:
			name = "SLA H"; break;
		case SLA_H:
			name = "SLA L"; break;
		case SLA_HL:
			name = "SLA (HL)"; break;
		case SRL_A:
			name = "SRL A"; break;
		case SRL_B:
			name = "SRL B"; break;
		case SRL_C:
			name = "SRL C"; break;
		case SRL_D:
			name = "SRL D"; break;
		case SRL_E:
			name = "SRL E"; break;
		case SRL_L:
			name = "SRL H"; break;
		case SRL_H:
			name = "SRL L"; break;
		case SRL_HL:
			name = "SRL (HL)"; break;
		default:
			name = "Undefined"; break;

	}
	return name;
}

#ifdef STEPTHROUGH
void DisplayState() {
	printf("Registers:\nAF: %02x%02x\tBC: %02x%02x\nDE: %02x%02x\tHL: %02x%02x\nSP: %04x\tPC: %04x\n\n",
		AF.a, AF.f, BC.b, BC.c, DE.d, DE.e, HL.h, HL.l, SP, PC);
}
#endif