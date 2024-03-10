#include"TitleScene.h"
#include"../Common.h"

//font variables

GLuint base;
//GLuint base2;

GLYPHMETRICSFLOAT gmf[256];

GLfloat R_ANGLE = 0.0f;

//Texture variables

GLuint TEXTURE_OCEAN = 0;

//blending variable

GLfloat Blend = 0.0f;

HFONT font3;
HFONT font4;

struct fontData* pfontData;

//function definitions

int titleInitialize(HDC ghdc)
{
	//local variable declaration
	 
	 BOOL bResult;
	 
	 
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);

	//BuildFont(ghdc);
	fprintf(gpFile, "Intro scene Marathi font data creating");

	fflush(gpFile);

	std::future<struct fontData*> f = enumerateFontFamilies(ghdc,"AMS Calligraphy 1");

	pfontData = f.get();

	fprintf(gpFile, "Intro scene Marathi font data created");

	fflush(gpFile);
	/*while (!IsDebuggerPresent()) {
	}*/

	BuildFontEx(ghdc, pfontData);

	bResult = LoadGLPNGTexture(&TEXTURE_OCEAN, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Ocean12.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Ocean image texture failed !! Exiting now\n");
		return(-1);
	}

	return(TRUE);

	
}

void titleDisplay(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//variable declaration
	


	//function declarations
	GLvoid glPrint(const char* fmt, ...);

	glDisable(GL_LIGHTING);

	

	
	

		

	glBindTexture(GL_TEXTURE_2D, TEXTURE_OCEAN);

	glBegin(GL_QUADS);
	{
		//glTranslatef(0.0f, -2.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.2f, 2.2f, 0.0f); //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-2.2f, 2.2f, 0.0f); //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.2f, -2.2f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.2f, -2.2f, 0.0f);  //right bottom


	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	



	

	

	glEnable(GL_LIGHTING);
	
	

	glPushMatrix();

	{

		glScalef(0.7f, 0.7f, 0.7f);

		glTranslatef(0.0f, 0.0f, 0.0f);

		//glColor3f(1.0f, 0.8f, 0.5f);

		//glColor3f(0.5f, 0.0f, 0.13f);     //burgendy

		//glColor3f(0.21f, 0.27f, 0.31f);   //charcoal gray

		//glColor3f(0.67f, 0.15f, 0.31f);    //deep purple

		//glColor3f(1.0f, 0.5f, 0.31f);    //corol orange

		//glColor3f(1.0f, 0.40f, 0.0f);        // orange

		glColor4f(0.18f, 0.18f, 0.18f,Blend);    //dark grey

		glPrintEx(pfontData, "saetaU baa/Yaa re saagarI");

		
		

		

		glColor3f(1.0f, 1.0f, 1.0f);

	}
	glPopMatrix();

	

	

	glPushMatrix();

	{
		glScalef(0.7f, 0.7f, 0.7f);
		glTranslatef(2.23f, 0.0f, 0.0f);

		glColor4f(0.18f, 0.18f, 0.18f,Blend);    //dark grey
		glPrintEx(pfontData,"/");

		glColor3f(1.0f, 1.0f, 1.0f);
	}

	glPopMatrix();

	glDisable(GL_LIGHTING);

}

void titleUpdate(void)
{
	/*R_ANGLE = R_ANGLE + 0.07f;

	if (R_ANGLE >= 360.0f)
	{
		R_ANGLE = 0.0f;
	}*/

	if (Blend < 1.0f)
	{
		Blend = Blend + 0.002f;
	}

}

void titleUninitialize(void)
{
	if (TEXTURE_OCEAN)
	{
		glDeleteTextures(1, &TEXTURE_OCEAN);

		TEXTURE_OCEAN = 0;
	}
	KillFont();
}

GLvoid BuildFont(HDC ghdc)
{
	HFONT font1; // will hold window font ID
	//HFONT font2;

	base = glGenLists(256); //storage for 256 
	//base2 = glGenLists(256);

	//create font

	font1 = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_STROKE_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "AMS Calligraphy 1");

	/*font2 = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_STROKE_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "Constantia");*/

	

	//select font object

	SelectObject(ghdc, font1);

	wglUseFontOutlines(ghdc, 0, 255, base, 0.0f, 0.2f, WGL_FONT_POLYGONS, gmf);
}

GLvoid KillFont(GLvoid)
{
	glDeleteLists(base, 256);  //delete list
}

GLvoid glPrint(const char* fmt, ...)
{
	float length = 0;
	char text[256];
	va_list ap; //for handeling variable arguments

	if (fmt == NULL)
	{
		return;
	}

	va_start(ap, fmt);  //initialize va_list to start the variable argument processing
	vsprintf(text, fmt, ap);  // formatting variable argument according to fmt

	va_end(ap);  //cleans the list

	for (unsigned int loop = 0; loop < (strlen(text)); loop++)
	{
		length += gmf[text[loop]].gmfCellIncX;
	}

	

	glTranslatef(-length / 2.0f, 0.0f, 0.0f);

	

	glPushAttrib(GL_LIST_BIT);

	glListBase(base);

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

	glPopAttrib();
}


