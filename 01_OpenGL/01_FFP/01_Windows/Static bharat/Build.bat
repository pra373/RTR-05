cls 

cl.exe /c /EHsc StaticBharat.c

rc.exe OGL.rc

link.exe StaticBharat.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

StaticBharat.exe