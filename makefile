strump: CPU.c Strump.c
	cl /W3 /wd4042 /FAcs  /Fe:Strump.exe /Tc *.c /link 
# /LIBPATH "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\um\x64\OpenGL32.Lib"
	
clean:
	del *.cod *.obj *.exe