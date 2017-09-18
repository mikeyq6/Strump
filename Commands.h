#include "alias.h"

void XOR(uint8_t opcode, uint8_t param);
void OR(uint8_t opcode, uint8_t param);
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