#include "CPU.h"
#include "opcodes.h"

#include "Commands.h"

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
			WriteMem(0xff00 + BC.c, AF.a); break;
		case LD_A__C_:
			AF.a = ReadMem(0xff00 + BC.c); break;
		case LDH_n_A:
			WriteMem(0xff00 + param1, AF.a); break;
		case LDH_A_n:
			AF.a = ReadMem(0xff00 + param1); break;
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

void JR(uint8_t opcode, uint8_t param1, uint8_t param2, uint8_t *skipPCInc) {
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
			if(!getFlag(C)) { PC = address; *skipPCInc = 1; }; break;
		case JP_NZ_nn:
			if(!getFlag(Z)) { PC = address; *skipPCInc = 1; }; break;
		case JP_C_nn:
			if(getFlag(C)) { PC = address; *skipPCInc = 1; }; break;
		case JP_Z_nn:
			if(getFlag(Z)) { PC = address; *skipPCInc = 1; }; break;
		case JP_nn:
			PC = address; *skipPCInc = 1; break;
		case JP_HL:
			PC = ReadMem(HL.hl); *skipPCInc = 1; break;
	}
}
void CALL(uint8_t opcode, uint8_t param1, uint8_t param2) {
	uint16_t address = (param2 << 8) + param1;
	uint8_t doCall = 0;
	short i=0;
	//printf("\nparam1=%02x, param2=%02x, address=%04x\n", param1, param2, address);
	
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
		Memory[SP] = (uint8_t)(next >> 8); SP--;
		Memory[SP] = (uint8_t)(next & 0xff); SP--;
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
			PC = address; SP += 2; InterruptsEnabled = 1; break;
		
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
		case CP_n:
			res = AF.a - param; val = param; break;
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
			Memory[SP] = AF.f; Memory[SP-1] = AF.a; SP -= 2; break;
		case PUSH_DE:
			Memory[SP] = DE.e; Memory[SP-1] = DE.d; SP -= 2; break;
		case PUSH_BC:
			Memory[SP] = BC.c; Memory[SP-1] = BC.b; SP -= 2; break;
		case PUSH_HL:
			Memory[SP] = HL.l; Memory[SP-1] = HL.h; SP -= 2; break;
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
 