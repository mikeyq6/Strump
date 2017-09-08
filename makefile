strump: CPU.c Strump.c
	cl /W3 /wd4042 /FAcs  /Fe:Strump.exe /Tc *.c /link 
	
clean:
	del *.cod *.obj *.exe