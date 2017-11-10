strump: Draw.c CPU.c Commands.c ROMInfo.c test.c Strump.c
	cl /W3 /wd4042 /FAcs /Fe:Strump.exe /Tc $** /DFREEGLUT_STATIC /DHAS_SDL /DSDL_MAIN_HANDLED /I "C:\Program Files (x86)\Windows Kits\8.1\Include\um" /I ".\SDL2-2.0.5\include" /link /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" ".\SDL2-2.0.5\lib\win32\SDL2.lib" ".\SDL2-2.0.5\lib\win32\SDL2main.lib" legacy_stdio_definitions.lib

ogi-strump: Draw.c CPU.c Commands.c ROMInfo.c test.c Strump.c
	cl /W3 /wd4042 /FAcs /Fe:Strump.exe /Tc $** /sdl /DFREEGLUT_STATIC /DHAS_SDL /DSDL_MAIN_HANDLED /I ".\SDL2-2.0.5\include" /I "C:\Program Files (x86)\Windows Kits\8.1\Include\um"  /link /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" ".\SDL2-2.0.5\lib\win32\SDL2.lib" ".\SDL2-2.0.5\lib\win32\SDL2main.lib" legacy_stdio_definitions.lib 

t1: t1.c
	cl /W3 /wd4042 /FAcs /Fe:t1.exe /Tc t1.c /sdl /DFREEGLUT_STATIC /DHAS_SDL /DSDL_MAIN_HANDLED /I ".\SDL2-2.0.5\include" /I "C:\Program Files (x86)\Windows Kits\8.1\Include\um"  /link /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" ".\SDL2-2.0.5\lib\win32\SDL2.lib" ".\SDL2-2.0.5\lib\win32\SDL2main.lib" legacy_stdio_definitions.lib
	
clean:
	del *.cod *.obj *.exe *.bin
	
	