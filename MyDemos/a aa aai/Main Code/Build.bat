cls 

cl.exe /c /EHsc OGL.cpp

rc.exe OGL.rc

link.exe OGL.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

