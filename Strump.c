#include <stdio.h> 
#include <conio.h>
#include "Strump.h"
#include "CPU.h"
#include "ROMInfo.h"
#include "Draw.h"

int main(int argc, char* argv[]) {
	//printf("Test %i, %i\n", argc, sizeof(argv)/sizeof(argv[0]));
	
	if(argc < 2) {
		printf("Usage: %s [gameboy rom]\n", argv[0]);
		return 1;
	}
	
	// Try to open ROM file
	FILE *fp;
	int err = fopen_s(&fp, argv[1], "r");
	if(err > 0) {
		printf("Coulnd't open file '%s'\n", argv[1]);
		return 2;
	}
	uint16_t size = (uint16_t)fread(Cartridge, sizeof(uint8_t), CARTRIDGE_SIZE, fp);
	/* printf("Size: %x, B5=%x, ac880=%x, bd330=%x", size, Cartridge[0xb5], Cartridge[0xac880], Cartridge[0xbd330]);
	_getch();
	return 0;*/
	initCPU();
	SetCartridgeInfo();
	DisplayCartridgeInfo();
	_getch();

	//drawInit(argc, argv);
	Start();
	 
	//printf("a=%u, f=%u, af=%u\n", AF.a, AF.f, AF.af);
	
	printf("\nAny key to quit");
	_getch();
	Quit();
	 
	return 0;
}