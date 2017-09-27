#include <assert.h>
#include "CPU.h"
#include "Commands.h"
#include "opcodes.h"

void clearFlags();

 // Testing
 void TestInstructions() {
	AF.a = 1;
	BC.c = 255;
	
	HL.hl = 0x9fff;
	assert(HL.h == 0x9f);
	assert(HL.l == 0xff);

	XOR(XOR_C, 0);
	assert(AF.a == 0xfe);
	assert(getFlag(Z) == 0);
	
	XOR(XOR_A, 0);
	assert(AF.a == 0);
	assert(getFlag(Z) == 1);
	
	BC.b = 16;
	XOR(XOR_B, 0);
	assert(AF.a == 16);
	assert(getFlag(Z) == 0);
	
	HL.hl = 0x8001;
	WriteMem(HL.hl, 48);
	XOR(XOR_HL, 0);
	assert(AF.a == 32);
	assert(getFlag(Z) == 0);
	
	XOR(XOR_n, 48);
	assert(AF.a == 16);
	assert(getFlag(Z) == 0);
	
	AF.a = 10;
	BC.b = 20;
	ADD(ADD_A_B, 0);
	assert(AF.a == 30);
	
	AF.a = 10;
	BC.c = 14;
	ADD(ADD_A_C, 0);
	assert(AF.a == 24);
	assert(getFlag(H) == 1);
	assert(getFlag(C) == 0);
	
	AF.a = 127;
	DE.d = 255;
	ADD(ADD_A_D, 0);
	assert(AF.a == 126);
	assert(getFlag(H) == 1);
	assert(getFlag(C) == 1);
	
	AF.a = 2;
	DE.e = 3;
	ADD(ADD_A_E, 0);
	assert(AF.a == 5);
	assert(getFlag(H) == 0);
	assert(getFlag(C) == 0);
	assert(getFlag(Z) == 0);
	
	AF.a = 0;
	HL.h = 0;
	ADD(ADD_A_H, 0);
	assert(AF.a == 0);
	assert(getFlag(Z) == 1);
	
	// OR
	AF.a = 5; // 0101
	BC.b = 9; // 1001
	OR(OR_B, 0);
	assert(AF.a == 13); // 1101
	assert(getFlag(Z) == 0);
	
	AF.a = 0;
	DE.d = 0;
	OR(OR_D, 0);
	assert(AF.a == 0);
	assert(getFlag(Z) == 1);
	
	// SWAP  10010100 -> 01001001
	AF.a = 0x94;
	SWAP(SWAP_A);
	assert(AF.a == 0x49);
	assert(getFlag(Z) == 0);
	
	AF.af = 0x2450;
	SP = 0x5000;
	uint16_t oldSP = SP;
	PUSH(PUSH_AF);
	assert(SP != oldSP);
	POP(POP_AF);
	assert(SP == oldSP);
	
	HL.h = 0x9a;
	resetFlag(C);
	RLC(RLC_H);
	assert(HL.h == 0x35);
	assert(getFlag(C) == 1);
	
	SP = 0x1000;
	BC.bc = 0xabcd;
	PUSH(PUSH_BC);
	assert(SP == 0x0ffe);
	assert(Memory[SP + 1] == 0xab);
	assert(Memory[SP + 2] == 0xcd);
	BC.bc = 0;
	POP(POP_BC);
	assert(SP == 0x1000);
	assert(BC.bc == 0xabcd);
	
	HL.h = 0x9a;
	resetFlag(C);
	RL(RL_H);
	//printf("HL.h = 0x%x\n", HL.h);
	assert(HL.h == 0x34);
	assert(getFlag(C) == 1);
	
	clearFlags();
	AF.a = 0x25;
	DE.e = 0x17;
	ADD(ADD_A_E, 0);
	assert(AF.a == 0x3c);
	assert(getFlag(H) == 0);
	assert(getFlag(C) == 0);
	DAA_();
	assert(AF.a == 0x42);
	
	clearFlags();
	AF.a = 0x72;
	BC.c = 0x15;
	SUB(SUB_C, 0);
	assert(AF.a == 0x5d);
	assert(getFlag(H) == 1);
	assert(getFlag(C) == 0);
	DAA_();
	assert(AF.a == 0x57);
		
	clearFlags();
	AF.a = 0xa1;
	RLA_();
	assert(getFlag(C) == 1);
	assert(AF.a == 0x42);
	RLA_();
	assert(getFlag(C) == 0);
	assert(AF.a == 0x85);
	
	clearFlags();
	BC.c = 0x42;
	LD(LD_C_n, 0x5a, 0);
	assert(BC.c == 0x5a);
	
	AF.a = 0x09;
	Memory[0x8001] = 0x76;
	DE.de = 0x8001;
	LD(LD_A_DE, 0, 0);
	assert(AF.a == 0x76);
	
	BC.b = 0x42;
	LD(LD_B_n, 0x5a, 0);
	assert(BC.b == 0x5a);
	
	// CP
	clearFlags();
	AF.a = 0x65;
	CP(CP_n, 0x3);
	assert(getFlag(Z) == 0);
	assert(getFlag(C) == 0);
	
	clearFlags();
	AF.a = 0x6a;
	CP(CP_n, 0x6a);
	assert(getFlag(Z) == 1);
	assert(getFlag(C) == 0);
	
	clearFlags();
	AF.a = 0x6;
	CP(CP_n, 0x30);
	assert(getFlag(Z) == 0);
	assert(getFlag(C) == 1);
 }
 
 void clearFlags() {
	 resetFlag(Z);
	 resetFlag(N);
	 resetFlag(C);
	 resetFlag(H);
 }
 