 #include <stdio.h> 
 #include <conio.h>
 #include "CPU.h"
 
 int main(int argc, char* argv[]) {
	 printf("Test %i, %i\n", argc, sizeof(argv)/sizeof(argv[0]));
	 
	 initCPU();
	Start();
	 
	//printf("a=%u, f=%u, af=%u\n", AF.a, AF.f, AF.af);
	
	 _getch();
	 
	 return 0;
 }