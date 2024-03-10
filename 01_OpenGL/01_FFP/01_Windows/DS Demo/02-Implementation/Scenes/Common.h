#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

//OpenGL Headers

#include<gl/GL.h>
#include<gl/GLU.h>
#include"../OGLMain.h"


#include <future>


enum SCENE
{
	TITLE_SCENE = 0,
	LOGO_SCENE,
	PRASTAWANA_SCENE,
	ENDCREDITS,
	SETU_BANDHA_RE,
	CHIMNI_SCENE,
	ANIMALS_SCENE,
	BRIDGE_SCENE
	
};

//SCENE TIMING MACROS

#define LOGO_SCENE_END 8.0f
#define TITLE_SCENE_END 12.0f

//PRASTAWANA SCENE

#define PRASTAWANA_SCENE_START 12.0f
#define PRASTAWANA_SCENE_END 48.0f


//prastawana scene local timings

//#define HANUMAN_IMAGE_END 17.0f

#define HANUMAN_IMAGE_END 24.0f

//#define SITA_RING_IMAGE_STARTS 17.0f
//#define SITA_RING_IMAGE_END 22.0f

#define SITA_RING_IMAGE_STARTS 24.0f
#define SITA_RING_IMAGE_END 33.0f

#define WANERSENA_IMAGE_STARTS 33.0f
#define WANERSENA_IMAGE_ENDS 44.0f

#define RAM_SADHANA_IMAGE_STARTS 44.0f
#define RAM_SADHANA_IMAGE_ENDS 53.0f

#define RAM_BAN_IMAGE_STARTS 53.0f
#define RAM_BAN_IMAGE_ENDS 79.0f

#define SETU_BUILDING_IMAGE_STARTS 79.0f
#define SETU_BUILDING_IMAGE_ENDS 89.0f


//waner scene macro

//#define WANER_SCENE_START 48.0f
//#define WANER_SCENE_END 63.0f

#define WANER_SCENE_START 89.0f
#define WANER_SCENE_END 115.0f

//animal scenes timing macros

#define ANIMALS_SCENE_START 115.0f
#define ANIMALS_SCENE_END 147.0f

//chimney scene timing macros

#define CHIMNI_SCENE_START 147.0f
#define CHIMNI_SCENE_END 154.0f

//Ram Setu Scene

#define RAMSETU_SCENE_START 154.0f
#define RAMSETU_SCENE_END 177.0f

//end credits macro

//#define END_CREDIT_STARTS 48.0f 

//#define END_CREDIT_STARTS 128.0f 
//#define END_CREDIT_END 168.0f 

#define END_CREDIT_STARTS 177.0f 
#define END_CREDIT_END 219.0f 



//#define SADARKARTA_START 48.0f
//#define SADARKARTA_HALF 50.0f
//#define SADARKARTA_END 52.0f

//#define SADARKARTA_START 111.0f
//#define SADARKARTA_HALF 113.0f
//#define SADARKARTA_END 115.0f

#define SADARKARTA_START 177.0f
#define SADARKARTA_HALF 179.0f
#define SADARKARTA_END 181.0f  //


//#define SANGIT_START 52.0f
//#define SANGIT_HALF 55.5f
//#define SANGIT_END 59.0f

//#define SANGIT_START 115.0f
//#define SANGIT_HALF 118.5f
//#define SANGIT_END 122.0f

#define SANGIT_START 181.0f
#define SANGIT_HALF 184.5f
#define SANGIT_END 188.0f

//#define TECHNOLOGY_START 59.0f
//#define TECHNOLOGY_HALF 62.0f
//#define TECHNOLOGY_END 65.0f

//#define TECHNOLOGY_START 122.0f
//#define TECHNOLOGY_HALF 125.0f
//#define TECHNOLOGY_END 128.0f

#define TECHNOLOGY_START 188.0f
#define TECHNOLOGY_HALF 191.0f
#define TECHNOLOGY_END 194.0f

//#define DATASTRUCTURE_START 65.0f
//#define DATASTRUCTURE_HALF 67.5f
//#define DATASTRUCTURE_END 70.0f

//#define DATASTRUCTURE_START 128.0f
//#define DATASTRUCTURE_HALF 130.5f
//#define DATASTRUCTURE_END 133.0f

#define DATASTRUCTURE_START 194.0f
#define DATASTRUCTURE_HALF 196.5f
#define DATASTRUCTURE_END 199.0f

//#define EFFECT_START 70.0f
//#define EFFECT_HALF 72.0f
//#define EFFECT_END 74.0f

//#define EFFECT_START 133.0f
//#define EFFECT_HALF 135.0f
//#define EFFECT_END 137.0f

#define EFFECT_START 199.0f
#define EFFECT_HALF 201.0f
#define EFFECT_END 203.0f



//#define TOOLS_START 74.0f
//#define TOOLS_HALF 76.0f
//#define TOOLS_END 78.0f

//#define TOOLS_START 137.0f
//#define TOOLS_HALF 139.0f
//#define TOOLS_END 141.0f

#define TOOLS_START 203.0f
#define TOOLS_HALF 205.0f
#define TOOLS_END 207.0f

//#define MARGADARSHAK_START 78.0f
//#define MARGADARSHAK_HALF 80.0f
//#define MARGADARSHAK_END 82.0f

//#define MARGADARSHAK_START 141.0f
//#define MARGADARSHAK_HALF 143.0f
//#define MARGADARSHAK_END 145.0f

#define MARGADARSHAK_START 207.0f
#define MARGADARSHAK_HALF 209.0f
#define MARGADARSHAK_END 211.0f

//#define ABHAR_START 82.0f
//#define ABHAR_HALF 84.0f
//#define ABHAR_END 86.0f

//#define ABHAR_START 145.0f
//#define ABHAR_HALF 147.0f
//#define ABHAR_END 149.0f

#define ABHAR_START 211.0f
#define ABHAR_HALF 213.0f
#define ABHAR_END 215.0f

//#define SAMAPT_START 86.0f
//#define SAMAPT_HALF 88.5f
//#define SAMAPT_END 86.0f

//#define SAMAPT_START 149.0f
//#define SAMAPT_HALF 151.5f

#define SAMAPT_START 215.0f
#define SAMAPT_HALF 217.5f
//#define SAMAPT_END 86.0f


















//global variable

extern FILE* gpFile;
extern int selected_Scene;     
extern float ELAPSED_TIME;



struct fontData
{
	std::string fontName;

	BYTE charSet;
	BYTE outPrecision;
	BYTE clipPrecision;
	BYTE quality;
	BYTE pitchAndFamily;

	//font variables

	GLuint base;

	GLYPHMETRICSFLOAT gmf[256];

	
};


//float ELAPSED_TIME;

//Function Declaration

BOOL loadGLTexture(GLuint* Texture, TCHAR image_Resource_ID[]);

BOOL LoadGLPNGTexture(GLuint* texture, const char* image); //for PNG loading

BOOL initializeAudio(void);
BOOL uninitializeAudio(void);

std::future<struct fontData*> enumerateFontFamilies(HDC hdc,
                                            const std::string& fontName);

GLvoid BuildFontEx(HDC ghdc, struct fontData* pfontData);

GLvoid glPrintEx(struct fontData* pfontData, const char* fmt,...);



