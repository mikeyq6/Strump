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
	
	AF.a = 0x06;
	HL.hl = 0x8003;
	Memory[0x8003] = 0x1a;
	ADD(ADD_A_HL, 0);
	assert(AF.a = 0x20);
	
	HL.hl = 0x1000;
	BC.bc = 0x1000;
	ADD(ADD_HL_BC, 0);
	assert(HL.hl == 0x2000);
	assert(getFlag(N) == 0);
	assert(getFlag(H) == 0);
	assert(getFlag(C) == 0);
	
	HL.hl = 0xffff;
	BC.bc = 0x0001;
	ADD(ADD_HL_BC, 0);
	assert(HL.hl == 0x0);
	assert(getFlag(N) == 0);
	assert(getFlag(H) == 1);
	assert(getFlag(C) == 1);
	
	// OR
	AF.a = 5; // 0101
	BC.b = 9; // 1001
	OR(OR_B, 0);
	assert(AF.a == 13); // 1101
	assert(getFlag(Z) == 0);
	
	AF.a = 0x42;
	BC.c = 0xc3;
	OR(OR_C, 0);
	assert(AF.a == 0xc3);
	assert(getFlag(Z) == 0);
	
	AF.a = 0x4a;
	BC.c = 0xc3;
	OR(OR_C, 0);
	assert(AF.a == 0xcb);
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
	
	// PUSH/POP
	AF.af = 0x2450;
	SP = 0x5010;
	PUSH(PUSH_AF);
	assert(SP == 0x500e);
	assert(Memory[SP] == 0x24);
	assert(Memory[SP + 1] == 0x50);
	AF.af = 0;
	POP(POP_AF);
	assert(SP == 0x5010);
	assert(AF.af == 0x2450);
	
	SP = 0x1000;
	BC.bc = 0xabcd;
	PUSH(PUSH_BC);
	assert(SP == 0x0ffe);
	assert(Memory[SP] == 0xab);
	assert(Memory[SP + 1] == 0xcd);
	BC.bc = 0;
	POP(POP_BC);
	assert(SP == 0x1000);
	assert(BC.bc == 0xabcd);
	
	SP = 0x1000;
	DE.de = 0xabcd;
	PUSH(PUSH_DE);
	assert(SP == 0x0ffe);
	assert(Memory[SP] == 0xab);
	assert(Memory[SP + 1] == 0xcd);
	DE.de = 0;
	POP(POP_DE);
	assert(SP == 0x1000);
	assert(DE.de == 0xabcd);
	
	SP = 0x1000;
	HL.hl = 0xabcd;
	PUSH(PUSH_HL);
	assert(SP == 0x0ffe);
	assert(Memory[SP] == 0xab);
	assert(Memory[SP + 1] == 0xcd);
	HL.hl = 0;
	POP(POP_HL);
	assert(SP == 0x1000);
	assert(HL.hl == 0xabcd);
	
	// RLC
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
	
	// SUB
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
	AF.a = 0x82;
	BC.b = 0x01;
	SUB(SUB_B, 0);
	assert(AF.a == 0x81);
	assert(getFlag(H) == 0);
	assert(getFlag(C) == 0);
	assert(getFlag(Z) == 0);
	
	clearFlags();
	AF.a = 0x82;
	BC.b = 0x82;
	SUB(SUB_B, 0);
	assert(AF.a == 0x00);
	assert(getFlag(H) == 0);
	assert(getFlag(C) == 0);
	assert(getFlag(Z) == 1);
	
	// RLA
	clearFlags();
	AF.a = 0xa1;
	RLA_();
	assert(getFlag(C) == 1);
	assert(AF.a == 0x42);
	RLA_();
	assert(getFlag(C) == 0);
	assert(AF.a == 0x85);
	
	// LD
	clearFlags();
	
	BC.b = 0x04;
	AF.a = 0x99;
	LD(LD_A_B, 0, 0);
	assert(AF.a == 0x04);
	
	BC.c = 0xaa;
	AF.a = 0x99;
	LD(LD_A_C, 0, 0);
	assert(AF.a == 0xaa);
	
	DE.d = 0x87;
	AF.a = 0x99;
	LD(LD_A_D, 0, 0);
	assert(AF.a == 0x87);
	
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
	
	AF.a = 0x09;
	Memory[0xff50] = 0x40;
	LD(LDH_A_n, 0x50, 0);
	assert(AF.a == 0x40);
	
	AF.a = 0x09;
	Memory[0xffff] = 0x40;
	LD(LDH_n_A, 0xff, 0);
	assert(Memory[0xffff] == 0x09);
	
	AF.a = 0x01;
	HL.l = 0x25;
	HL.h = 0x65;
	DE.e = 0x0d;
	LD(LD_A_L, 0, 0);
	assert(AF.a == 0x25);
	LD(LD_A_H, 0, 0);
	assert(AF.a == 0x65);
	LD(LD_A_E, 0, 0);
	assert(AF.a == 0x0d);
	
	// CP
	clearFlags();
	AF.a = 0x65;
	BC.b = 0x03;
	CP(CP_B, 0);
	assert(getFlag(Z) == 0);
	assert(getFlag(C) == 0);
	
	clearFlags();
	AF.a = 0x6a;
	BC.b = 0x6a;
	CP(CP_B, 0);
	assert(getFlag(Z) == 1);
	assert(getFlag(C) == 0);
	
	clearFlags();
	AF.a = 0x6;
	BC.b = 0x30;
	CP(CP_B, 0);
	assert(getFlag(Z) == 0);
	assert(getFlag(C) == 1);
	
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
	
	clearFlags();
	AF.a = 0x65;
	HL.hl = 0x8002;
	Memory[0x8002] = 0x3;
	CP(CP_HL, 0);
	assert(getFlag(Z) == 0);
	assert(getFlag(C) == 0);
	
	clearFlags();
	AF.a = 0x6a;
	HL.hl = 0x8002;
	Memory[0x8002] = 0x6a;
	CP(CP_HL, 0);
	assert(getFlag(Z) == 1);
	assert(getFlag(C) == 0);
	
	clearFlags();
	AF.a = 0x6;
	HL.hl = 0x8002;
	Memory[0x8002] = 0x30;
	CP(CP_HL, 0);
	assert(getFlag(Z) == 0);
	assert(getFlag(C) == 1);
	
	// DEC
	clearFlags();
	DE.e = 0x90;
	DEC(DEC_E);
	assert(DE.e == 0x8f);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 1);
	
	clearFlags();
	DE.e = 0x01;
	DEC(DEC_E);
	assert(DE.e == 0x00);
	assert(getFlag(Z) == 1);
	assert(getFlag(N) == 1);
	
	clearFlags();
	DE.e = 0x00;
	DEC(DEC_E);
	assert(DE.e == 0xff);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 1);
	
	clearFlags();
	AF.a = 0x90;
	DEC(DEC_A);
	assert(AF.a == 0x8f);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 1);
	
	clearFlags();
	AF.a = 0x01;
	DEC(DEC_A);
	assert(AF.a == 0x00);
	assert(getFlag(Z) == 1);
	assert(getFlag(N) == 1);
	
	clearFlags();
	AF.a = 0x00;
	DEC(DEC_A);
	assert(AF.a == 0xff);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 1);
	
	// INC
	clearFlags();
	HL.hl = 0x1002;
	INC(INC_HL);
	assert(HL.hl == 0x1003);
	assert(getFlag(H) == 0);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 0);
	
	clearFlags();
	HL.hl = 0xffff;
	INC(INC_HL);
	assert(HL.hl == 0x0);
	assert(getFlag(H) == 0);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 0);
	
	clearFlags();
	DE.e = 0xff;
	INC(INC_E);
	assert(DE.e == 0x0);
	assert(getFlag(H) == 1);
	assert(getFlag(Z) == 1);
	assert(getFlag(N) == 0);
	
	clearFlags();
	DE.e = 0x0f;
	INC(INC_E);
	assert(DE.e == 0x10);
	assert(getFlag(H) == 1);
	assert(getFlag(Z) == 0);
	assert(getFlag(N) == 0);
	
	// CALL / RET
	uint8_t skipPCIntTest = 0;
	PC = 0x1000;
	SP = 0x1789;
	CALL(CALL_nn, 0x45, 0x7a);
	assert(PC == 0x7a45);
	assert(SP == 0x1787);
	assert(Memory[SP] == 0x10);
	assert(Memory[SP + 1] == 0x03);
	RET_(RET, &skipPCIntTest);
	assert(PC == 0x1003);
	assert(SP == 0x1789);
	
	PC = 0x000;
 }
 
 void clearFlags() {
	 resetFlag(Z);
	 resetFlag(N);
	 resetFlag(C);
	 resetFlag(H);
 }
 