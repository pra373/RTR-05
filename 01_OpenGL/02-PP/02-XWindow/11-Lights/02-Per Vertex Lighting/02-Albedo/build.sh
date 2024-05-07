
# Compile OGL.cpp to create OGL.o object file
gcc -c OGL.cpp -o OGL.o

# Link OGL.o with sphere.a and other necessary libraries to create the executable OGL
gcc -o OGL OGL.o -L. -L/home/prathamesh/MyProjects/RTR-05/01_OpenGL/02-PP/XWindows/09-WhiteShpere -lX11 -lGL -lGLEW -lm -lSphere

   
   
