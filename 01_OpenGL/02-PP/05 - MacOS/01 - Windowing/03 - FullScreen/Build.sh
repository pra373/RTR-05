#!/bin/sh

#  Build.sh
#  
#
#  Created by user258815 on 6/16/24.
#

clang -c -o Window.o Window.m
mkdir -p Window.app/Contents/MacOS
clang -o Window.app/Contents/MacOS/Window Window.o -framework Cocoa


