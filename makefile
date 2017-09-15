strump: CPU.c Strump.c
	cl /W3 /wd4042 /FAcs  /Fe:Strump.exe /Tc *.c /link /LIBPATH "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\um\x64\OpenGL32.Lib"

ogi-strump: CPU.c Strump.c
	cl /W3 /wd4042 /FAcs  /Fe:Strump.exe /Tc *.c /DFREEGLUT_STATIC /I "C:\Program Files (x86)\Windows Kits\8.1\Include\um" /I ".\freeglut.2.8.1.15\include"  /link /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" /LIBPATH:"C:\Users\mdann\Documents\Programming\c\Strump\freeglut.2.8.1.15\lib\v110\Win32\Release\static" legacy_stdio_definitions.lib

clean:
	del *.cod *.obj *.exe