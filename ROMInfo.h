#define NO_ROMBANK	0x00
#define MBC1		0x01
#define MBC2		0x02
#define MBC3		0x04
#define MBC5		0x08
#define MM01		0x10

#define NO_RAM		0
#define RAM_1_2		1
#define RAM_1_8		2
#define RAM_4_32	3
#define RAM_16_128	4

typedef struct _cartridgeInfo {
	char title[17];
	uint8_t gbc;
	uint8_t sgb;
	uint8_t type;
	char *typeName;
	uint8_t romType;
	char *romTypeName;
	uint8_t ramType;
	char *ramTypeName;
	uint8_t destinationCode;
	uint8_t licenseeCode;
	uint8_t controllerType;
	uint8_t hasBattery;
	uint8_t hasRumble;
	uint8_t hasTimer;
	uint8_t hasSRAM;
	char *ROMcontrollerName;
	
} CartridgeInfo;

CartridgeInfo *CartInfo;

void InitCartridgeInfo();
void SetCartridgeNames();
void SetCartridgeInfo();
void DisplayCartridgeInfo();
void DestroyCartridgeInfo();