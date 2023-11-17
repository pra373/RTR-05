cls 

cl.exe /c /EHsc DynamicBharat.c

rc.exe OGL.rc

link.exe DynamicBharat.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

DynamicBharat.exe