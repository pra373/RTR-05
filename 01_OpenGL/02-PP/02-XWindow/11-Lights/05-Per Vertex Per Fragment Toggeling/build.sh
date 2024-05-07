
# Compile OGL.cpp to create OGL.o object file
gcc -c OGL.cpp -o OGL.o

# Link OGL.o with sphere.a and other necessary libraries to create the executable OGL
gcc -o OGL OGL.o -L. -lX11 -lGL -lGLEW -lm -lSphere

   
   
