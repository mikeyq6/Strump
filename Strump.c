#include <stdio.h> 
#include <conio.h>
#include "CPU.h"

 
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
	
	initCPU();
	Start();
	 
	//printf("a=%u, f=%u, af=%u\n", AF.a, AF.f, AF.af);
	
	printf("\nAny key to quit");
	_getch();
	 
	return 0;
}