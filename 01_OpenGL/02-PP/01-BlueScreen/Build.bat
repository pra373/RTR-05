cls

cl.exe /c /EHsc /I "C:\glew-2.1.0-win32\glew-2.1.0\include" OGL.c  

rc.exe OGL.rc 			   

link.exe OGL.obj OGL.res user32.lib gdi32.lib /LIBPATH:"C:\glew-2.1.0-win32\glew-2.1.0\lib\Release\x64" /SUBSYSTEM:WINDOWS 

OGL.exe

