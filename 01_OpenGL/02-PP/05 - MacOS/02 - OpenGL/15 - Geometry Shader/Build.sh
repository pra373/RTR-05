#!/bin/sh

#  Build.sh
#  
#
#  Created by user258815 on 6/16/24.
#

clang++ -Wno-deprecated-declarations -c -o OGL.o OGL.mm
mkdir -p OGL.app/Contents/MacOS
clang++ -o OGL.app/Contents/MacOS/OGL OGL.o -framework Cocoa -framework QuartzCore -framework OpenGL


