#include <stdio.h>
#include <stdlib.h>
#include "alias.h"
#include "CPU.h"

#include "ROMInfo.h"

void InitCartridgeInfo() {
	CartInfo = malloc(sizeof(CartridgeInfo));
	CartInfo->typeName = malloc(sizeof(char) * 30);
	CartInfo->romTypeName = malloc(sizeof(char) * 50);
	CartInfo->ramTypeName = malloc(sizeof(char) * 50);
	CartInfo->ROMcontrollerName = malloc(sizeof(char) * 20);
}

void DisplayCartridgeInfo() {
	printf("Cartridge Info:\nName: \"%s\"\n", CartInfo->title);
	printf("Type: %s - %s\n", CartInfo->gbc ? "GameBoy Color" : "GameBoy", CartInfo->typeName);
	printf("ROM Type: %s\nRAM Type: %s\n", CartInfo->romTypeName, CartInfo->ramTypeName);
	printf("ROM Controller Type: %s\n\n", CartInfo->ROMcontrollerName);
}

void SetCartridgeInfo() {
	for(uint16_t i = 0x134, j=0; i<= 0x142; i++,j++) {
		if(Cartridge[i] == 0) {
			CartInfo->title[j] = '\0';
			break;
		} else {
			CartInfo->title[j] = Cartridge[i];
		}
	}
	CartInfo->sgb = Cartridge[0x146];
	CartInfo->gbc = Cartridge[0x143];
	CartInfo->type = Cartridge[0x147];
	CartInfo->romType = Cartridge[0x148];
	CartInfo->ramType = Cartridge[0x149];
	CartInfo->destinationCode = Cartridge[0x14a];
	CartInfo->licenseeCode = Cartridge[0x14b];
	CartInfo->hasTimer = 0;
	CartInfo->hasRumble = 0;
	CartInfo->hasBattery = 0;
	CartInfo->hasSRAM = 0;
	SetCartridgeNames();
}
void SetCartridgeNames() {
	switch(CartInfo->type) {
		case 0:
			CartInfo->typeName = "ROM ONLY"; 
			CartInfo->controllerType = NO_ROMBANK; 
			CartInfo->ROMcontrollerName = "No ROM Controller";
			break;
		case 0x1:
			CartInfo->typeName = "ROM+MBC1"; 
			CartInfo->controllerType = MBC1; 
			CartInfo->ROMcontrollerName = "MBC1";
			break;
		case 0x2:
			CartInfo->typeName = "ROM+MBC1+RAM"; 
			CartInfo->controllerType = MBC1; 
			CartInfo->ROMcontrollerName = "MBC1";
			break;
		case 0x3:
			CartInfo->typeName = "ROM+MBC1+RAM+BATT"; 
			CartInfo->controllerType = MBC1; 
			CartInfo->ROMcontrollerName = "MBC1";
			CartInfo->hasBattery = 1;
			break;
		case 0x5:
			CartInfo->typeName = "ROM-MBC2"; 
			CartInfo->controllerType = MBC2; 
			CartInfo->ROMcontrollerName = "MBC2";
			break;
		case 0x6:
			CartInfo->typeName = "ROM-MBC2+BATTERY";
			CartInfo->controllerType = MBC2; 
			CartInfo->ROMcontrollerName = "MBC2";
			CartInfo->hasBattery = 1;
			break;
		case 0x8:
			CartInfo->typeName = "ROM+RAM"; 
			CartInfo->controllerType = NO_ROMBANK; 
			CartInfo->ROMcontrollerName = "No ROM Controller";
			break;
		case 0x9:
			CartInfo->typeName = "ROM+RAM+BATTERY";
			CartInfo->controllerType = NO_ROMBANK; 
			CartInfo->ROMcontrollerName = "No ROM Controller";
			CartInfo->hasBattery = 1;
			break;
		case 0xb:
			CartInfo->typeName = "ROM+MM01";
			CartInfo->controllerType = MM01; 
			CartInfo->ROMcontrollerName = "MM01";
			break;
		case 0xc:
			CartInfo->typeName = "ROM+MM01+SRAM"; 
			CartInfo->controllerType = MM01; 
			CartInfo->ROMcontrollerName = "MM01";
			CartInfo->hasSRAM = 1;
			break;
		case 0xd:
			CartInfo->typeName = "ROM+MM01+SRAM+BATT"; 
			CartInfo->controllerType = MM01; 
			CartInfo->ROMcontrollerName = "MM01";
			CartInfo->hasBattery = 1;
			CartInfo->hasSRAM = 1;
			break;
		case 0xf:
			CartInfo->typeName = "ROM+MBC3+TIMER+BATT";
			CartInfo->controllerType = MBC3; 
			CartInfo->ROMcontrollerName = "MBC3";
			CartInfo->hasBattery = 1;
			break;
		case 0x10:
			CartInfo->typeName = "ROM+MBC3+TIMER+RAM+BATT";
			CartInfo->controllerType = MBC3; 
			CartInfo->ROMcontrollerName = "MBC3";
			CartInfo->hasBattery = 1;
			break;
		case 0x11:
			CartInfo->typeName = "ROM+MBC3";
			CartInfo->controllerType = MBC3; 
			CartInfo->ROMcontrollerName = "MBC3";
			break;
		case 0x12:
			CartInfo->typeName = "ROM+MBC3+RAM";
			CartInfo->controllerType = MBC3; 
			CartInfo->ROMcontrollerName = "MBC3";
			break;
		case 0x13:
			CartInfo->typeName = "ROM+MBC3+RAM+BATT";
			CartInfo->controllerType = MBC3; 
			CartInfo->ROMcontrollerName = "MBC3";
			CartInfo->hasBattery = 1;
			break;
		case 0x19:
			CartInfo->typeName = "ROM+MBC5";
			CartInfo->controllerType = MBC5; 
			CartInfo->ROMcontrollerName = "MBC5";
			break;
		case 0x1a:
			CartInfo->typeName = "ROM+MBC5+RAM";
			CartInfo->controllerType = MBC5; 
			CartInfo->ROMcontrollerName = "MBC5";
			break;
		case 0x1b:
			CartInfo->typeName = "ROM+MBC5+RAM+BATT";
			CartInfo->controllerType = MBC5; 
			CartInfo->ROMcontrollerName = "MBC5";
			CartInfo->hasBattery = 1;
			break;
		case 0x1c:
			CartInfo->typeName = "ROM+MBC5+RUMBLE";
			CartInfo->controllerType = MBC5; 
			CartInfo->ROMcontrollerName = "MBC5";
			CartInfo->hasRumble = 1;
			break;
		case 0x1d:
			CartInfo->typeName = "ROM+MBC5+RUMBLE+SRAM";
			CartInfo->controllerType = MBC5; 
			CartInfo->ROMcontrollerName = "MBC5";
			CartInfo->hasRumble = 1;
			break;
		case 0x1e:
			CartInfo->typeName = "ROM+MBC5+RUMBLE+SRAM+BATT";
			CartInfo->controllerType = MBC5; 
			CartInfo->ROMcontrollerName = "MBC5";
			CartInfo->hasRumble = 1;
			CartInfo->hasBattery = 1;
			CartInfo->hasSRAM = 1;
			break;
		case 0x1f:
			CartInfo->typeName = "Pocket Camera"; break;
		case 0xfd:
			CartInfo->typeName = "Bandai TAMA5"; break;
		case 0xfe:
			CartInfo->typeName = "Hudson HuC-3";
			CartInfo->controllerType = MBC1; 
			CartInfo->ROMcontrollerName = "MBC1";
			break;
		case 0xff:
			CartInfo->typeName = "Hudson HuC-1"; 
			CartInfo->controllerType = MBC1; 
			CartInfo->ROMcontrollerName = "MBC1";
			break;
	}
	
	switch(CartInfo->romType) {
		case 0:
			CartInfo->romTypeName = "256Kbit =  32KByte = 2 banks"; break;
		case 1:
			CartInfo->romTypeName = "512Kbit =  64KByte = 4 banks"; break;
		case 2:
			CartInfo->romTypeName = "  1Mbit = 128KByte = 8 banks"; break;
		case 3:
			CartInfo->romTypeName = "  2Mbit = 256KByte = 16 banks"; break;
		case 4:
			CartInfo->romTypeName = "  4Mbit = 512KByte = 32 banks"; break;
		case 5:
			CartInfo->romTypeName = "  8Mbit =   1MByte = 64 banks"; break;
		case 6:
			CartInfo->romTypeName = " 16Mbit =   2MByte = 128 banks"; break;
		case 0x52:
			CartInfo->romTypeName = "  9Mbit = 1.1MByte = 72 banks"; break;
		case 0x53:
			CartInfo->romTypeName = " 10Mbit = 1.2MByte = 80 banks"; break;
		case 0x54:
			CartInfo->romTypeName = " 12Mbit = 1.5MByte = 96 banks"; break;
	}
	
	switch(CartInfo->ramType) {
		case NO_RAM:
			CartInfo->ramTypeName = "None"; break;
		case RAM_1_2:
			CartInfo->ramTypeName = " 16KBit =   2KByte = 1 bank"; break;
		case RAM_1_8:
			CartInfo->ramTypeName = " 64KBit =   8KByte = 1 bank"; break;
		case RAM_4_32:
			CartInfo->ramTypeName = "256KBit =  32KByte = 4 banks"; break;
		case RAM_16_128:
			CartInfo->ramTypeName = "  1MBit = 128KByte = 16 banks"; break;
	}
}

void DestroyCartridgeInfo() {
	free(CartInfo);
}