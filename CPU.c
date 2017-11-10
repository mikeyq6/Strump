#include <stdio.h>
#include <string.h>
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
long long int iCounter = 0;
#endif

void readInitialProgram();
void setDefaults();
void runInitialProgram();
short GetParameters(uint8_t opcode, uint8_t *param1, uint8_t *param2);
uint8_t GetCycles(uint8_t opcode);
short ShouldPrint();
void PushPCOntoStack();
void UpdateGraphics(uint8_t opcode);
uint32_t GetColourForPaletteNumber(uint8_t pNumber);

// Internal startup ROM
uint8_t InternalRom[INTERNAL_ROM_SIZE] = { 0x31,0xFE,0xFF,0xAF,0x21,0xFF,0x9F,0x32,0xCB,0x7C,0x20,0xFB,0x21,0x26,0xFF,0x0E,0x11,0x3E,0x80,0x32,0xE2,0x0C,0x3E,0xF3,0xE2,0x32,0x3E,0x77,0x77,0x3E,0xFC,0xE0,0x47,0x11,0x04,0x01,0x21,0x10,0x80,0x1A,0xCD,0x95,0x00,0xCD,0x96,0x00,0x13,0x7B,0xFE,0x34,0x20,0xF3,0x11,0xD8,0x00,0x06,0x08,0x1A,0x13,0x22,0x23,0x05,0x20,0xF9,0x3E,0x19,0xEA,0x10,0x99,0x21,0x2F,0x99,0x0E,0x0C,0x3D,0x28,0x08,0x32,0x0D,0x20,0xF9,0x2E,0x0F,0x18,0xF3,0x67,0x3E,0x64,0x57,0xE0,0x42,0x3E,0x91,0xE0,0x40,0x04,0x1E,0x02,0x0E,0x0C,0xF0,0x44,0xFE,0x90,0x20,0xFA,0x0D,0x20,0xF7,0x1D,0x20,0xF2,0x0E,0x13,0x24,0x7C,0x1E,0x83,0xFE,0x62,0x28,0x06,0x1E,0xC1,0xFE,0x64,0x20,0x06,0x7B,0xE2,0x0C,0x3E,0x87,0xE2,0xF0,0x42,0x90,0xE0,0x42,0x15,0x20,0xD2,0x05,0x20,0x4F,0x16,0x20,0x18,0xCB,0x4F,0x06,0x04,0xC5,0xCB,0x11,0x17,0xC1,0xCB,0x11,0x17,0x05,0x20,0xF5,0x22,0x23,0x22,0x23,0xC9,0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E,0x3C,0x42,0xB9,0xA5,0xB9,0xA5,0x42,0x3C,0x21,0x04,0x01,0x11,0xA8,0x00,0x1A,0x13,0xBE,0x20,0xFE,0x23,0x7D,0xFE,0x34,0x20,0xF5,0x06,0x19,0x78,0x86,0x23,0x05,0x20,0xFB,0x86,0x20,0xFE,0x3E,0x01,0xE0,0x50 };

void initCPU() {
	Halted = Stopped = WillEnableInterrupts = WillDisableInterrupts = InterruptsEnabled = 0;
	Startup = TRUE;
	RomBank = 0;
	RamBank = 0;
	RamEnabled = FALSE;
	RomBanking = TRUE;
	sCounter = 0;
	PC = 0x100;
	
	memset(Memory, 0, sizeof(Memory));
	memset(RamBankData, 0, sizeof(RamBankData));
	memset(InstructionStats, 0, sizeof(InstructionStats));
	
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
	AF.af = 0x100;
	BC.bc = 0x14;
	DE.de = 0;
	HL.hl = 0xc060;
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
	WriteMem(IF, 0xe1); // IE
	WriteMem(STAT, 0x85); // IE
}

void Start() {
	//readInitialProgram();
	//runInitialProgram();
	uint8_t param1 = 0;
	uint8_t param2 = 0;
	setDefaults();
	
#ifdef DEBUG_TIMER
		WriteMem(TAC, 0x07);
		InterruptsEnabled = 1;
#endif

	for(;;) {
		uint8_t inst = GetNextInstruction();
		short params = GetParameters(inst, &param1, &param2);
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
					printf(CodeToString(inst), CBCodeToString(param1));
				} else {
					printf(CodeToString(inst), param1);
				}
			}
#endif
			Run(inst, param1, 0);
		} else if(params == 2) {
#ifdef STEPTHROUGH
			if(ShouldPrint()) { printf(CodeToString(inst), param1, param2); }
#endif
			Run(inst, param1, param2);
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
			else if(x == 'p') {
				printf("Break when SP reaches: ");
				unsigned int input;
				scanf_s("%x", &input);
				
			}
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
			} else if(x == 'c') {
				printf("Check cartridge ROM at address: ");
				unsigned int input;
				scanf_s("%x", &input);
				printf("Cartridge[%05x] = %02x\n\n", (uint16_t)input, Cartridge[input]);
				goto debug_routine;
			} else if(x == 'i') {
				char buffer[11];
				scanf_s("%s", buffer, 10);
				if(strcmp(buffer, "s") == 0) {
					printf("Instructions used so far:\n");
					for(int s=0; s<512; s++) {
						if(s == 0x100) {
							printf("\n\nCB Instructions used so far:\n");
						}
						if(InstructionStats[s] > 0) {
							if(s > 0xff) {
								printf("%02x(%x) ", s-0xff, InstructionStats[s]);
							} else {
								printf("%02x(%x) ", s, InstructionStats[s]);
							}
						}
					}
				} else if(strcmp(buffer, "r") == 0) {
					// reset
					memset(InstructionStats, 0, sizeof(InstructionStats));
				} else if(strcmp(buffer, "f") == 0) {
					// output to file
					int err = fopen_s(&fp, "InsData.csv", "w");
					fprintf(fp, "Instructions used so far:,\n");
					for(int s=0; s<512; s++) {
						if(s == 0x100) {
							fprintf(fp, "CB Instructions used so far:,\n");
						}
						if(InstructionStats[s] > 0) {
							if(s > 0xff) {
								fprintf(fp, "%02x,%u\n", s-0xff, InstructionStats[s]);
							} else {
								fprintf(fp, "%02x,%u\n", s, InstructionStats[s]);
							}
						}
					}
					fclose(fp);
					goto debug_routine;
				} else {
					printf("Unknown option");
				}				
				printf("\n\n");
				goto debug_routine;
			} else if(x == 'r') {
				// Set register
				unsigned int address;
				printf("Set Register. Which?\n");
				char buffer[11];
				scanf_s("%s", buffer, 10);
				
				if(strcmp(buffer, "PC") == 0) {
					printf("\nValue: ");
					scanf_s("%x", &address);
					PC = (uint16_t)address;
					printf("\n\n");
					goto debug_routine;
				} else {
					printf("\n\n");
					goto debug_routine;
				}
			}
		}
#endif
		
		if(InterruptsEnabled) {
			CheckInterrupts();
		}
		UpdateTimer(inst);
		UpdateGraphics(inst);
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
	//if(location == LY) { return (Startup ? 0x90 : 0x91); }
	
	if(location < 0x100 && Startup) {
		return InternalRom[location];
	} else if(location >= 0 && location <= 0x14d) {
		return Cartridge[location];
	} else if(location >= 0xa000 && location < 0xc000) {
		location -= 0xa000;
		return RamBankData[location + (RamBank * 0x2000)];
	} else if(location >= 0xe000 && location < 0xfe00) { // Allow for the mirrored internal RAM
		return Memory[location - 0x2000];
	} else if(location >= 0x4000 && location <= 0x7fff) {
		return GetValueAt(location);
	} else {		
		return Memory[location];
	}
	return 0;
}
void WriteMem(uint16_t location, uint8_t value) {
	
	if(location == DIV) {
		Memory[location] = 0; // Always set DIV to 0 on write
	} else if(location >= 0 && location < 0x2000) {
		printf("Enabling RAM/ROM: %02x\n", value);
		//_getch();
		if(value == 0x0a) {
			RamEnabled = TRUE;
		} else {
			RamEnabled = FALSE;
		}
	} else if(location >= 0x2000 && location < 0x4000) { // ROM Switching
		RomBank = value;
	} else if(location >= 0x4000 && location < 0x6000) { // ROM/RAM Switching
		printf("ROM/RAM Switching. RomBanking = %s, Bank=%02x\n", (RomBanking ? "true" : "false"), value);
		//_getch();
		if(CartInfo->controllerType == MBC1 || CartInfo->controllerType == MBC3) {
			if(RomBanking) {
				RomBank = value;
			} else {
				RamBank = value;
			}
		}
	} else if (location >= 0x6000 && location < 0x8000) { // Set Rom or Ram banking
		if(CartInfo->controllerType == MBC1) {
			RomBanking = (value == 0) ? TRUE : FALSE;
			RomBank = 0;
		}
	} else if(location >= 0xe000 && location < 0xfe00) { // Allow for the mirrored internal RAM
		Memory[location - 0x2000] = value;
		Memory[location] = value;
	} else if(location >= 0xa000 && location < 0xc000) { // Writing to RAM
		if(RamEnabled) {
			location -= 0xa000;
			RamBankData[location + (RamBank * 0x2000)] = value;
			printf("Writing (%02x) to RAM at address(%04x)\n", value, (location + (RamBank * 0x2000)));
			//_getch();
		} else {
			printf("Trying to write to RAM but it is not enabled");
			//_getch();
		}
	} else if(location == ENDSTART) {;
		Startup = 0;
	} else if(location == LY) {
		Memory[LY] = 0;
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
				//printf("setting interrupt\n");
				//_getch();
				SetInterrupt(I_Timer);
			}
		} else {
			rDiv += cycles;
		}
		
	}
}

static void PushPCOntoStack() {
	Memory[--SP] = (uint8_t)(PC & 0xff);
	Memory[--SP] = (uint8_t)(PC >> 8);
}
 
// Instructions 
void Run(uint8_t opcode, uint8_t param1, uint8_t param2) {
	uint8_t skipPCInc = 0;
	uint8_t p1, p2;
	InstructionStats[opcode]++;
	#ifdef STEPTHROUGH
	iCounter++;
	#endif
	// if(opcode == 0xff) {
		// printf("PC=%04x", PC);
		// _getch();
	// }
	
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
		case AND_A:
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
			RET_(opcode, &skipPCInc); break;
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
	else { PC += (1 + GetParameters(opcode, &p1, &p2)); }
}
void RunCBInstruction(uint8_t opcode) {
	InstructionStats[opcode + 0xff]++;
	
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
	
	return GetValueAt(PC);
}

uint8_t GetValueAt(uint16_t address) {
	uint8_t val = 0;
	uint32_t nAddress = address;
	
	if(address >= 0 && address < 0x4000) {
		return Startup ? InternalRom[address] : Cartridge[address];
	} else if(address >= 0x4000 && address <= 0x7fff) {
		if(RomBank == 0x13) {
			//printf("address=%04x, newAddress=%x\n", address, address + ((RomBank - 1) * 0x4000));
		}
		if(RomBank > 1) {
			nAddress = address + ((RomBank - 1) * 0x4000);
			val = Cartridge[nAddress];
		} else {
			val = Cartridge[address];
		}
	} else if(address >= 0xa000 && address < 0xc000) {
		// External (cartridge) RAM
		
	} else if(address >= 0xc000 && address < 0xe000) {
		// Internal Work RAM
		address -= 0xa000;
		return RamBankData[address + (RamBank * 0x2000)];
	} else if(address >= 0xff80 && address < 0xffff) {
		// High-RAM area
		return Memory[address];
	} else {
		val = Memory[address];
	}
	
	//printf("address=%04x, val=%02x\n", address, val);
	return val;
}

 // Figure out how many paramters the given instruction requires
short GetParameters(uint8_t opcode, uint8_t *param1, uint8_t *param2) {
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
			*param1 = GetValueAt(PC+1);
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
			*param1 = GetValueAt(PC+1);
			*param2 = GetValueAt(PC+2);
			//printf("param1=%02x, param2=%02x, PC=%04x, PC+1=%04x, PC+2=%04x\n", *param1, *param2, PC, PC+1, PC+2);
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

// LCDC
void UpdateGraphics(uint8_t opcode) {
	uint8_t cycles;
	SetLCDStatus();	
		
	if(IsLCDEnabled()) {
		cycles = GetCycles(opcode);
		sCounter -= cycles;
	} else return;
	
	//printf("sCounter=%04x\n", sCounter);
	if(sCounter <= 0) {
		Memory[LY]++;
		uint8_t cLine = Memory[LY];
		
		sCounter = 0x1c8;
		
		if(cLine == 0x90) {
			SetInterrupt(I_VBlank);
		} else if(cLine > 0x99) {
			Memory[LY] = 0;
		} else if(cLine == 143) {
			//printf("cLine=%02x, callRefresh();\n", cLine);
			callRefresh();
		}	
		
	}
	
}

void SetLCDStatus() {
	uint8_t status = Memory[STAT];
	
	if(!IsLCDEnabled()) {
		sCounter = 0x1c8;
		Memory[LY] = 0;
		status &= 0xfc; // Clear the lower 2 bits
		Memory[STAT] = (status | 0x01); // Set the lower 2 bits to '01'
		return;
	}
	
	uint8_t cLine = Memory[LY];
	uint8_t cMode = status & 0x03; // Get lower 2 bits
	
	uint8_t mode = 0;
	uint8_t reqInt = 0;
	//printf("status=%02x, cLine=%02x, cMode=%x, sCounter=%02x\n", status, cLine, cMode, sCounter);
	
	if(cLine >= 0x90) {
		mode = 1;
		status &= 0xfc; // Clear the lower 2 bits
		status |= 0x01; // Set lower 2 bits to '01'
		reqInt = (status & BIT4);
	} else {
		uint8_t mode2bounds = (uint8_t)(0x1c8 - 80);
		uint8_t mode3bounds = mode2bounds - 172; 
		
		// mode 2
		if(sCounter >= mode2bounds) {
			mode = 2;
			status &= 0xfc; // Clear the lower 2 bits
			status |= 0x02; // Set lower 2 bits to '10'
			reqInt = (status & BIT5);		
		// mode 3
		} else if (sCounter >= mode3bounds) { 
			mode = 3;
			status |= 0x03; // Set lower 2 bits to '11'		
		// mode 0
		} else {
			mode = 0;
			status &= 0xfc; // Clear the lower 2 bits
			reqInt = (status & BIT3);					
		}
	}
	
	// just entered a new mode so request interupt
	if (reqInt && (mode != cMode)) {
		SetInterrupt(I_LCDC);
	}
	
	// check the conincidence flag
	if(Memory[LYC] == Memory[LY]) {
		status |= 0x04;
		if(status & BIT6) {
			SetInterrupt(I_LCDC);			
		}
	} else  {
		status &= 0xfb;
	}
	
	Memory[STAT] = status;
}
uint16_t GetBackgroundTileMapLocation() {
	uint8_t val = Memory[LCDC];
	return (((val & (1 << 3)) == 0) ? 0x9800 : 0x9c00);
}
uint16_t BGWindowTileLocation() {
	uint8_t val = Memory[LCDC];
	return (((val & (1 << 4)) == 0) ? 0x8800 : 0x8000);
}
uint8_t IsLCDEnabled() {
	//printf("LCDC=%02x, enabled=%s\n", Memory[LCDC], (Memory[LCDC] & BIT7 ? "true" : "false"));
	return (Memory[LCDC] & BIT7);
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
	printf("Registers:\nAF: %02x%02x\tBC: %02x%02x\tZ N H C\nDE: %02x%02x\tHL: %02x%02x\t%x %x %x %x\nSP: %04x\tPC: %04x\nIE: %02x\tIF: %02x\tLY: %02x\tLYC: %02x\tSTAT: %02x\tRomBank: %02x\nTAC: %02x\tTMA %02x\tTIMA: %02x\tDIV: %02x\trDiv: %x\n\n",
		AF.a, AF.f, BC.b, BC.c, DE.d, DE.e, HL.h, HL.l, getFlag(Z), getFlag(N), getFlag(H), getFlag(C), SP, PC,
		Memory[IF], Memory[IE], Memory[LY], Memory[LYC], Memory[STAT], RomBank, Memory[TAC], Memory[TMA], Memory[TIMA], Memory[DIV], rDiv);
}
#endif