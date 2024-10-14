cls

cl.exe /c /EHsc /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.5\include" /I "C:\glew-2.1.0-win32\glew-2.1.0\include" OGL.cpp  

nvcc.exe -c -o kernel.obj kernel.cu

rc.exe OGL.rc 			   

link.exe OGL.obj kernel.obj OGL.res user32.lib gdi32.lib /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.5\lib\x64" /LIBPATH:"C:\glew-2.1.0-win32\glew-2.1.0\lib\Release\x64" /SUBSYSTEM:WINDOWS 

OGL.exe

