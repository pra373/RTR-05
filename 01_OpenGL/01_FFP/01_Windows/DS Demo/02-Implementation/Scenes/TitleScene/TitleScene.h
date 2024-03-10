#pragma once
#include"../Common.h"

//function declarations

int titleInitialize(HDC);
void titleDisplay(void);
void titleUpdate(void);
void titleUninitialize(void);
GLvoid BuildFont(HDC);
GLvoid KillFont(GLvoid);
GLvoid glPrint(const char* fmt, ...);


