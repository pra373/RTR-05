#!/bin/sh

#  Build.sh
#
#
#  Created by user258815 on 6/16/24.
#

clang++ -Wno-deprecated-declarations -c OGL.mm sphere.cpp -std=c++17
mkdir -p OGL.app/Contents/MacOS
clang++ -o OGL.app/Contents/MacOS/OGL OGL.o sphere.o -framework Cocoa -framework QuartzCore -framework OpenGL


