 
#ifndef ALIAS_H
#define ALIAS_H

typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#define TRUE 1
#define FALSE 0

/* Memory Location Shortcuts */
#define SB			0xff01
#define SC			0xff02
#define DIV			0xff04
#define TIMA		0xff05
#define TMA			0xff06
#define TAC			0xff07
#define IF			0xff0f
#define NR10		0xff10
#define NR11		0xff11
#define NR12		0xff12
#define NR13		0xff13
#define NR14		0xff14
#define NR21		0xff16
#define NR22		0xff17
#define NR23		0xff18
#define NR24		0xff19
#define NR30		0xff1a
#define NR31		0xff1b
#define NR32		0xff1c
#define NR33		0xff1d
#define NR34		0xff1e
#define NR41		0xff20
#define NR42		0xff21
#define NR43		0xff22
#define NR44		0xff23
#define NR50		0xff24
#define NR51		0xff25
#define NR52		0xff26

#define LCDC		0xff40
#define STAT		0xff41
#define SCY			0xff42
#define SCX			0xff43
#define LY			0xff44
#define LYC			0xff45
#define DMA			0xff46
#define BGP			0xff47
#define OBP0		0xff48
#define OBP1		0xff49
#define WY			0xff4a
#define WX			0xff4b
#define ENDSTART	0xff50
#define IE			0xffff

#endif