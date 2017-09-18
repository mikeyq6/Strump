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