strump: Draw.c CPU.c Commands.c ROMInfo.c Strump.c
	cl /W3 /wd4042 /FAcs /Fe:Strump.exe /Tc *.c /DFREEGLUT_STATIC /I "C:\Program Files (x86)\Windows Kits\8.1\Include\um" /I ".\freeglut.2.8.1.15\include" /link /LTCG /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" /LIBPATH:"D:\Source\C\Strump\freeglut.2.8.1.15\lib\v110\Win32\Release\static" legacy_stdio_definitions.lib

ogi-strump: Draw.c CPU.c Commands.c ROMInfo.c Strump.c
	cl /W3 /wd4042 /FAcs /Fe:Strump.exe /Tc *.c /sdl /DFREEGLUT_STATIC /DHAS_SDL /DSDL_MAIN_HANDLED /SUBSYSTEM:CONSOLE /I ".\SDL2-2.0.5\include" /I "C:\Program Files (x86)\Windows Kits\8.1\Include\um"  /link /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" ".\SDL2-2.0.5\lib\win32\SDL2.lib" ".\SDL2-2.0.5\lib\win32\SDL2main.lib" legacy_stdio_definitions.lib 

clean:
	del *.cod *.obj *.exe *.bin