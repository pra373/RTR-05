
gcc -c -o OGL.o -I /usr/include -I /usr/include/AL OGL.c
   
gcc -o OGL -L /usr/lib/x86_64-linux-gnu OGL.o -lX11 -lGL -lGLU -lm -lalut -lopenal


   
   
