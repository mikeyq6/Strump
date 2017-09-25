#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "Commands.h"
#include "ROMInfo.h"
#include "Draw.h"

#include "CPU.h"
#include "test.h"

#define RUNTESTS
#define STEPTHROUGH 0
//#define DEBUG_TIMER

#ifdef RUNTESTS
void TestInstructions();
#endif
#ifdef STEPTHROUGH 
void DisplayState();
int skip = 0;
int skipuntil = 0;
short tempShow = 1;
uint8_t bdata[BACKGROUNDTILES * 16];
FILE *fp;
#endif

void readInitialProgram();
void setDefaults();
void runInitialProgram();
short GetParameters(uint8_t opcode);
uint8_t GetCycles(opcode);
short ShouldPrint();
void PushPCOntoStack();
uint32_t GetColourForPaletteNumber(uint8_t pNumber);

// Internal startup ROM
uint8_t InternalRom[INTERNAL_ROM_SIZE] = { 0x31,0xFE,0xFF,0xAF,0x21,0xFF,0x9F,0x32,0xCB,0x7C,0x20,0xFB,0x21,0x26,0xFF,0x0E,0x11,0x3E,0x80,0x32,0xE2,0x0C,0x3E,0xF3,0xE2,0x32,0x3E,0x77,0x77,0x3E,0xFC,0xE0,0x47,0x11,0x04,0x01,0x21,0x10,0x80,0x1A,0xCD,0x95,0x00,0xCD,0x96,0x00,0x13,0x7B,0xFE,0x34,0x20,0xF3,0x11,0xD8,0x00,0x06,0x08,0x1A,0x13,0x22,0x23,0x05,0x20,0xF9,0x3E,0x19,0xEA,0x10,0x99,0x21,0x2F,0x99,0x0E,0x0C,0x3D,0x28,0x08,0x32,0x0D,0x20,0xF9,0x2E,0x0F,0x18,0xF3,0x67,0x3E,0x64,0x57,0xE0,0x42,0x3E,0x91,0xE0,0x40,0x04,0x1E,0x02,0x0E,0x0C,0xF0,0x44,0xFE,0x90,0x20,0xFA,0x0D,0x20,0xF7,0x1D,0x20,0xF2,0x0E,0x13,0x24,0x7C,0x1E,0x83,0xFE,0x62,0x28,0x06,0x1E,0xC1,0xFE,0x64,0x20,0x06,0x7B,0xE2,0x0C,0x3E,0x87,0xE2,0xF0,0x42,0x90,0xE0,0x42,0x15,0x20,0xD2,0x05,0x20,0x4F,0x16,0x20,0x18,0xCB,0x4F,0x06,0x04,0xC5,0xCB,0x11,0x17,0xC1,0xCB,0x11,0x17,0x05,0x20,0xF5,0x22,0x23,0x22,0x23,0xC9,0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E,0x3C,0x42,0xB9,0xA5,0xB9,0xA5,0x42,0x3C,0x21,0x04,0x01,0x11,0xA8,0x00,0x1A,0x13,0xBE,0x20,0xFE,0x23,0x7D,0xFE,0x34,0x20,0xF5,0x06,0x19,0x78,0x86,0x23,0x05,0x20,0xFB,0x86,0x20,0xFE,0x3E,0x01,0xE0,0x50 };

void initCPU() {
	Halted = Stopped = WillEnableInterrupts = WillDisableInterrupts = InterruptsEnabled = 0;
	Startup = 1;
	RomBank = 1;
	RamBank = 1;
	RamEnabled = 0;
	sCounter = 0;
	PC = 0x000;
	
#ifdef STEPTHROUGH
	InitCartridgeInfo();
	for(int i=0; i<BACKGROUNDTILES * 16; i++) {
		bdata[i] = 0;
	}
#endif
	
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
	rDiv = 0;
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
	
#ifdef DEBUG_TIMER
		WriteMem(TAC, 0x07);
		InterruptsEnabled = 1;
#endif

	for(;;) {
		uint8_t inst = GetNextInstruction();
		short params = GetParameters(inst);
		if(WillEnableInterrupts) {
			WillEnableInterrupts = 0;
			InterruptsEnabled = 1;
		}
		if(WillDisableInterrupts) {
			WillDisableInterrupts = 0;
			InterruptsEnabled = 0;
		}
		
		//printf("%x ", InternalRom[i]);
		if(params == 0) {
#ifdef STEPTHROUGH
			if(ShouldPrint()) { printf(CodeToString(inst)); }
#endif
			Run(inst, 0, 0);
		} else if(params == 1) {
#ifdef STEPTHROUGH
			if(ShouldPrint()) { 
				if(inst == CB) {
					printf(CodeToString(inst), CBCodeToString((Startup ? InternalRom[PC+1] : Cartridge[PC+1])));
				} else {
					printf(CodeToString(inst), (Startup ? InternalRom[PC+1] : Cartridge[PC+1]));
				}
			}
#endif
			Run(inst, (Startup ? InternalRom[PC+1] : Cartridge[PC+1]), 0);
		} else if(params == 2) {
#ifdef STEPTHROUGH
			if(ShouldPrint()) { printf(CodeToString(inst), (Startup ? InternalRom[PC+1] : Cartridge[PC+1]), (Startup ? InternalRom[PC+2] : Cartridge[PC+2])); }
#endif
			Run(inst, (Startup ? InternalRom[PC+1] : Cartridge[PC+1]), (Startup ? InternalRom[PC+2] : Cartridge[PC+2]));
		}
		if(ShouldPrint()) { 
			printf("\n");
		}
		
#ifdef STEPTHROUGH

		debug_routine:if(skip > 0) { 
			if(ShouldPrint()) {
				DisplayState();
			}
			skip--; }
		else if(skipuntil > 0 && skipuntil != PC) {
			if(ShouldPrint()) {
				DisplayState();
			}			
		} else {
			DisplayState();
			skipuntil = 0;
			char x = _getch();
			if(x == 'q') { break; }
			else if(x == 's') { 
				printf("Skip to address: ");
				unsigned int input;
				scanf_s("%x", &input);
				skipuntil = (uint16_t)input;
			}
			else if(x == 'm') {
				printf("Check memory at address: ");
				unsigned int input;
				scanf_s("%x", &input);
				printf("Memory[%04x] = %02x\n\n", (uint16_t)input, Memory[(uint16_t)input]);
				goto debug_routine;
			}
			else if(x == 'd') {
				// Dump tile data to file
				int err = fopen_s(&fp, "FrameData.bin", "w");
				fwrite(&Memory[0x8000], sizeof(uint8_t), sizeof(uint8_t) * 0x1fff, fp);
				// for(int i=0x8000; i< 0x9000; i++) {
					// fprintf(fp, "%x", memory[i]);
				// }
				fclose(fp);
				goto debug_routine;
			}
			else if(x == 'b') {
				// Dump background
				loadBackground();
				for(int i=0; i<BACKGROUNDTILES; i++) {
					for(int j=0; j<16; j++) {
						//printf("background[%02x]->data[%02x] = %02x\n",i , j, background[i]->data[j]);
						bdata[(i * 16) + j] = background[i]->data[j];
					}
				}
				int err = fopen_s(&fp, "Background.bin", "w");
				fwrite(bdata, sizeof(uint8_t), sizeof(uint8_t) * (BACKGROUNDTILES * 16), fp);
				// for(int i=0x8000; i< 0x9000; i++) {
					// fprintf(fp, "%x", memory[i]);
				// }
				fclose(fp);
				goto debug_routine;
			}
		}
#endif
		
		if(InterruptsEnabled) {
			CheckInterrupts();
		}
		UpdateTimer(inst);
		if(sCounter++ % SCOUNTER_HZ == 0) {
			callRefresh();
		}
	}
}

void Quit() {
	free(CartInfo);
}

short ShouldPrint() {
#ifdef STEPTHROUGH
	return (skip == 0 && skipuntil == 0) || STEPTHROUGH;
#else
	return 0;
#endif
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
	//printf("\nReadMem(%04x)\n", location);
// #ifdef STEPTHROUGH
	// if(location == LY && tempShow) { return 0x90; tempShow = 0; } // debug
// #else
	// 
// #endif
	if(location == LY) { return (Startup ? 0x90 : 0x91); }
	
	if(location < 0x100 && Startup) {
		return InternalRom[location];
	} else if(location >= 0 && location <= 0x14d) {
		return Cartridge[location];
	} else if(location >= 0xe000 && location < 0xfe00) { // Allow for the mirrored internal RAM
		return Memory[location - 0x2000];
	} else {
		return Memory[location];
	}
	return 0;
}
void WriteMem(uint16_t location, uint8_t value) {
	
	if(location == DIV) {
		Memory[location] = 0; // Always set DIV to 0 on write
	} else if(location >= 0x2000 && location <= 0x3fff) { // ROM Switching
		RomBank = value;
	} else if(location >= 0x4000 && location <= 0x5fff) { // RAM Switching
		RamBank = value;
	} else if(location >= 0xe000 && location < 0xfe00) { // Allow for the mirrored internal RAM
		Memory[location - 0x2000] = value;
	} else if(location == ENDSTART) {;
		Startup = 0;
	} else {
		Memory[location] = value;
	}
}
 
// InterruptsEnabled
void CheckInterrupts() {
	uint8_t bit = 1;
	uint8_t iFlag = Memory[IF];
	uint8_t ieFlag = Memory[IE];
	
	// Check VBlank
	if(iFlag & ieFlag & bit) {
		PushPCOntoStack(); 
		PC = I_VBlank;
		ResetInterrupt(I_VBlank);
	}
	bit <<= 1;
	// Check LCDC
	if(iFlag & ieFlag & bit) {
		PushPCOntoStack();
		PC = I_LCDC;
		ResetInterrupt(I_LCDC);
	}
	bit <<= 1;
	// Check Timer
	//printf("iflag=%02x, bit=%02x\n", iFlag, bit);
	if(iFlag & ieFlag & bit) {
		PushPCOntoStack();
		PC = I_Timer; 
		ResetInterrupt(I_Timer);
		_getch();
	}
	bit <<= 1;
	// Check Serial
	if(iFlag & ieFlag & bit) {
		PushPCOntoStack();
		PC = I_Serial;
		ResetInterrupt(I_Serial);
	}
	bit <<= 1;
	// Check Serial
	if(iFlag & ieFlag & bit) {
		PushPCOntoStack();
		PC = I_Joypad;
		ResetInterrupt(I_Joypad);
	}
}

void SetInterrupt(uint8_t iRegister) {
	uint8_t iFlag = ReadMem(IF);
	
	switch(iRegister) {
		case I_VBlank:
			Memory[IF] = (iFlag | 0x01); break;
		case I_LCDC:
			Memory[IF] = (iFlag | 0x02); break;
		case I_Timer:
			Memory[IF] = (iFlag | 0x04); break;
		case I_Serial:
			Memory[IF] = (iFlag | 0x08); break;
		case I_Joypad:
			Memory[IF] = (iFlag | 0x0f); break;
	}
}
void ResetInterrupt(uint8_t iRegister) {
	uint8_t iFlag = ReadMem(IF);
	
	switch(iRegister) {
		case I_VBlank:
			Memory[IF] = (iFlag & 0xfe); break;
		case I_LCDC:
			Memory[IF] = (iFlag & 0xfd); break;
		case I_Timer:
			Memory[IF] = (iFlag & 0xfb); break;
		case I_Serial:
			Memory[IF] = (iFlag & 0xf7); break;
		case I_Joypad:
			Memory[IF] = (iFlag & 0xef); break;
	}
}

void UpdateTimer(uint8_t opcode) {
	uint8_t cycles = GetCycles(opcode);
	uint8_t rTAC = ReadMem(TAC);
	
	uint32_t speed = 0;
	
	switch(rTAC & 0x3) {
		case 0:
			speed = FREQ / 4096; break;
		case 1:
			speed = FREQ / 262144; break;
		case 2:
			speed = FREQ / 65536; break;
		case 3: 
			speed = FREQ / 16384; break;
	}
	
	if(rTAC & 0x04) { // Timer is started
		uint16_t tmp = rDiv + cycles;
#ifdef DEBUG_TIMER
		printf("DIV=%02x, F=%02x,tmp=%x, speed=%x\n", Memory[DIV], AF.f, tmp, speed);
#endif
		if(tmp >= speed) {
			rDiv = (rDiv + cycles) % speed;
			setFlag(C);
			
			Memory[DIV]++;
			if(Memory[DIV] == 0) {
				// set timer interrupt
				SetInterrupt(I_Timer);
			}
		} else {
			rDiv += cycles;
		}
		
	}
}

static void PushPCOntoStack() {
	Memory[SP] = (uint8_t)(PC >> 8); SP--;
	Memory[SP] = (uint8_t)(PC & 0xff); SP--;
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
		case LDH_A_n:
		case LDH_n_A:
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
			JR(opcode, param1, param2, &skipPCInc); break;
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
			skipPCInc = 1;
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
uint8_t GetNextInstruction() {
	uint8_t inst = 0;
	
	if(PC >= 0x4000 && PC <= 0x7fff) {
		uint16_t address = ((RomBank - 1) * 0x4000) + 0x4000;
		inst = Cartridge[address];
		// if(!Startup) {
			// printf("Getting instruction from RomBank(%x), address(%04x) = %02x\n", RomBank, address, inst);
		// }
	} else {
		inst = Startup ? InternalRom[PC] : Cartridge[PC];
		// if(!Startup) {
			// printf("Getting instruction from address(%04x) = %02x\n", PC, inst);
		// }
	}
	return inst;
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
// Figure out how many CPU cycles the given instruction requires
uint8_t GetCycles(opcode) {
	uint8_t cycles = 4;
	
	switch(opcode) {
		case CALL_nn:
			cycles = 24; break;
		case LD_nn_SP:
			cycles = 20; break;
		case RET:
		case RETI:
		case RST_00H:
		case RST_08H:
		case RST_10H:
		case RST_18H:
		case RST_20H:
		case RST_28H:
		case RST_30H:
		case RST_38H:
		case PUSH_AF:
		case PUSH_BC:
		case PUSH_DE:
		case PUSH_HL:
		case JP_nn:
		case ADD_SP_n:
		case LD_nn_A:
		case LD_A_nn:
			cycles = 16; break;
		case LD_BC_nn:
		case LD_DE_nn:
		case LD_HL_nn:
		case LD_SP_nn:
		case JR_n:
		case LDH_A_n:
		case LDH_n_A:
		case POP_AF:
		case POP_BC:
		case POP_DE:
		case POP_HL:
		case LD_HL_SP_n:
		case LD__HL__n:
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
		case LD_A_HL:
		case LD_B_HL:
		case LD_C_HL:
		case LD_D_HL:
		case LD_E_HL:
		case LD_H_HL:
		case LD_L_HL:
		case LD_HL_A:
		case LD_HL_B:
		case LD_HL_C:
		case LD_HL_D:
		case LD_HL_E:
		case LD_HL_H:
		case LD_HL_L:
		case LD__C__A:
		case LD_A__C_:
		case LD_SP_HL:
		case ADD_HL_BC:
		case ADD_HL_DE:
		case ADD_HL_HL:
		case ADD_HL_SP:
		case ADD_A_n:
		case ADD_A_HL:
		case ADC_A_n:
		case ADC_A_HL:
		case SUB_n:
		case SUB_HL:
		case SBC_A_n:
		case SBC_A_HL:
		case AND_n:
		case AND_HL:
		case OR_n:
		case OR_HL:
		case XOR_n:
		case XOR_HL:
		case CP_n:
		case CP_HL:
			cycles = 8; break;
		case JR_Z_n:
			if(getFlag(Z)) { cycles = 12; } else { cycles = 8; }; break;
		case JR_C_n:
			if(getFlag(C)) { cycles = 12; } else { cycles = 8; }; break;
		case JR_NZ_n:
			if(!getFlag(Z)) { cycles = 12; } else { cycles = 8; }; break;
		case JR_NC_n:
			if(!getFlag(C)) { cycles = 12; } else { cycles = 8; }; break;
		case RET_Z:
			if(getFlag(Z)) { cycles = 20; } else { cycles = 8; }; break;
		case RET_C:
			if(getFlag(C)) { cycles = 20; } else { cycles = 8; }; break;
		case RET_NZ:
			if(!getFlag(Z)) { cycles = 20; } else { cycles = 8; }; break;
		case RET_NC:
			if(!getFlag(C)) { cycles = 20; } else { cycles = 8; }; break;
		case CALL_C_nn:
			if(getFlag(C)) { cycles = 24; } else { cycles = 12; }; break;
		case CALL_Z_nn:
			if(getFlag(Z)) { cycles = 24; } else { cycles = 12; }; break;
		case CALL_NC_nn:
			if(!getFlag(C)) { cycles = 24; } else { cycles = 12; }; break;
		case CALL_NZ_nn:
			if(!getFlag(Z)) { cycles = 24; } else { cycles = 12; }; break;
		default:
			cycles = 4; break;
		
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

// LCDC
uint16_t GetBackgroundTileMapLocation() {
	uint8_t val = Memory[LCDC];
	return (((val & (1 << 3)) == 0) ? 0x9800 : 0x9c00);
}
uint16_t BGWindowTileLocation() {
	uint8_t val = Memory[LCDC];
	return (((val & (1 << 4)) == 0) ? 0x8800 : 0x8000);
}
uint32_t GetColourFor(uint8_t number) {
	uint8_t palette = Memory[BGP];
	
	switch(number) {
		case 0:
			return GetColourForPaletteNumber(palette & 0x03); break;
		case 1:
			return GetColourForPaletteNumber((palette & 0x0c) >> 2); break;
		case 2:
			return GetColourForPaletteNumber((palette & 0x30) >> 4); break;
		case 3:
			return GetColourForPaletteNumber((palette & 0xc0) >> 6); break;
	}
	return WHITE;
}
uint32_t GetColourForPaletteNumber(uint8_t pNumber) {
	switch(pNumber) {
		case 0:
			return WHITE; break;
		case 1:
			return LT_GRAY; break;
		case 2:
			return DK_GRAY; break;
		case 3:
			return BLACK; break;			
	}
	return WHITE;
}

#ifdef STEPTHROUGH
void DisplayState() {
	printf("Registers:\nAF: %02x%02x\tBC: %02x%02x\tZ N H C\nDE: %02x%02x\tHL: %02x%02x\t%x %x %x %x\nSP: %04x\tPC: %04x\nIF: %02x\tDIV: %02x\trDiv: %x\n\n",
		AF.a, AF.f, BC.b, BC.c, DE.d, DE.e, HL.h, HL.l, getFlag(Z), getFlag(N), getFlag(H), getFlag(C), SP, PC,
		Memory[IF], Memory[DIV], rDiv);
}
#endif