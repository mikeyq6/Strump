#include <assert.h>
#include "CPU.h"
#include "opcodes.h"

void clearFlags();

 // Testing
 void TestInstructions() {
	AF.a = 1;
	BC.c = 255;
	
	HL.hl = 0x9fff;
	assert(HL.h == 0x9f);
	assert(HL.l == 0xff);

	XOR(XOR_C, 0, 0);
	assert(AF.a == 0xfe);
	assert(getFlag(Z) == 0);
	
	XOR(XOR_A, 0, 0);
	assert(AF.a == 0);
	assert(getFlag(Z) == 1);
	
	BC.b = 16;
	XOR(XOR_B, 0, 0);
	assert(AF.a == 16);
	assert(getFlag(Z) == 0);
	
	HL.hl = 0x4fff;
	WriteMem(HL.hl, 48);
	XOR(XOR_HL, 0, 0);
	assert(AF.a == 32);
	assert(getFlag(Z) == 0);
	
	XOR(XOR_n, 48, 0);
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
	OR(OR_B, 0, 0);
	assert(AF.a == 13); // 1101
	assert(getFlag(Z) == 0);
	
	AF.a = 0;
	DE.d = 0;
	OR(OR_D, 0, 0);
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
 }
 
 void clearFlags() {
	 resetFlag(Z);
	 resetFlag(N);
	 resetFlag(C);
	 resetFlag(H);
 }