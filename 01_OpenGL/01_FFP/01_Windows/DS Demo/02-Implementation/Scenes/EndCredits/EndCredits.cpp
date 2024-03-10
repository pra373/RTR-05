#include"EndCredits.h"

GLuint TEXTURE_RAMSETU;
GLuint TEXTURE_SADARKARTA;

//GLfloat blendInSadarkarta = 0.0f;  //sadarkarta
GLfloat blendInSadarkarta = -0.3f;
GLfloat blendOutSadarkarta = 1.0f;

/*GLfloat blendInSangit = 0.0f;*/     //sangit

GLfloat blendInSangit = -0.3f;
GLfloat blendOutSangit = 1.0f;

GLfloat blendInTechnology = 0.0f;     //Technology
GLfloat blendOutTechnology = 1.0f;

GLfloat blendInDataStructure = 0.0f;     //Data Structure
GLfloat blendOutDataStructure = 1.0f;

GLfloat blendInEffects = 0.0f;     //effects
GLfloat blendOutEffects = 1.0f;

GLfloat blendInTools = 0.0f;     //effects
GLfloat blendOutTools = 1.0f;

GLfloat blendInMargadarshak = 0.0f;     //margadarshak
GLfloat blendOutMargadarshak = 1.0f;

GLfloat blendInAbhar = 0.0f;     //visheh abhar
GLfloat blendOutAbhar = 1.0f;

GLfloat blendInSamapt = 0.0f;     //visheh abhar
GLfloat blendOutSamapt = 1.0f;


struct fontData* pEnglishFontData;

struct fontData* pMarathiFontData;

int initializeEndCredits(HDC ghdc)
{

	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);

	
	//BuildFont(ghdc);

	fprintf(gpFile, "English font data creating");

	std::future<struct fontData*> f = enumerateFontFamilies(ghdc, "Constantia");

	pEnglishFontData = f.get();

	fprintf(gpFile, "English font data created");

	BuildFontEx(ghdc, pEnglishFontData);


	fprintf(gpFile, "Marathi font data creating");
	std::future<struct fontData*> f1 = enumerateFontFamilies(ghdc, "AMS Calligraphy 1");

	pMarathiFontData = f1.get();

	fprintf(gpFile, "Marathi font data created");

	BuildFontEx(ghdc, pMarathiFontData);

	
	BOOL bResult;

	bResult = LoadGLPNGTexture(&TEXTURE_RAMSETU, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/RamSetu6.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Ramsetu image texture failed !! Exiting now\n");
		return(-14);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_SADARKARTA, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/SadarKarta.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Sadarkarta image texture failed !! Exiting now\n");
		return(-14);
	}

	//glEnable(GL_BLEND);       //enable blending for font scenes
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return(TRUE);
}

void displayEndCredits(void)
{
	//code

	//enable blending

	
	//function declarations
	GLvoid glPrint(const char* fmt, ...);
	//void sadarKarta(void);

	//background image

	glDisable(GL_LIGHTING);   //disable lighting to draw image

	glPushMatrix();

	{

		glTranslatef(0.0f, -0.2f, 0.5f);

		glScalef(2.2f, 1.4f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_RAMSETU);

		glBegin(GL_QUADS);
		{
			//glTranslatef(0.0f, -2.0f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			//glVertex3f(2.2f, 2.2f, 0.0f); //right top

			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			//glVertex3f(-2.2f, 2.2f, 0.0f); //left top
			glVertex3f(-1.0f, 1.0f, 0.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom


		}

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();

	glEnable(GL_LIGHTING);   //Enable lighting for font

	glEnable(GL_BLEND);       //enable blending for font scenes
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/*if (ELAPSED_TIME > 0.0f && ELAPSED_TIME <= 4.0f)
	{
		sadarkarta();
	}*/

	if (ELAPSED_TIME > END_CREDIT_STARTS && ELAPSED_TIME <= SADARKARTA_END)
	{
		sadarkarta();
	}

	

	/*else if(ELAPSED_TIME > 4.0f && ELAPSED_TIME <= 9.0f)
	{
		sangit();
	}*/

	else if (ELAPSED_TIME > SANGIT_START && ELAPSED_TIME <= SANGIT_END)
	{
		sangit();
	}

	/*else if (ELAPSED_TIME > 9.0f && ELAPSED_TIME <= 14.0f)
	{
		technology();
	}*/

	else if (ELAPSED_TIME > TECHNOLOGY_START && ELAPSED_TIME <= TECHNOLOGY_END)
	{
		technology();
	}

	/*else if (ELAPSED_TIME > 14.0f && ELAPSED_TIME < 18.5f)
	{
		dataStructure();
	}*/

	else if (ELAPSED_TIME > DATASTRUCTURE_START && ELAPSED_TIME < DATASTRUCTURE_END)
	{
		dataStructure();
	}

	/*else if (ELAPSED_TIME > 18.5f && ELAPSED_TIME <= 22.5f)
	{
		effects();
	}*/

	else if (ELAPSED_TIME > EFFECT_START && ELAPSED_TIME <= EFFECT_END)
	{
		effects();
	}

	/*else if (ELAPSED_TIME > 22.5f && ELAPSED_TIME < 26.5f)
	{
		tools();
	}*/

	else if (ELAPSED_TIME > TOOLS_START && ELAPSED_TIME < TOOLS_END)
	{
		tools();
	}

	/*else if (ELAPSED_TIME > 26.5f && ELAPSED_TIME < 30.5f)
	{
		margadarshak();
	}*/

	else if (ELAPSED_TIME > MARGADARSHAK_START && ELAPSED_TIME < MARGADARSHAK_END)
	{
		margadarshak();
	}

	/*else if (ELAPSED_TIME > 30.5f && ELAPSED_TIME < 34.5f)
	{
		abhar();
	}*/

	else if (ELAPSED_TIME > ABHAR_START && ELAPSED_TIME < ABHAR_END)
	{
		abhar();
	}

	else if (ELAPSED_TIME > SAMAPT_START/*&& ELAPSED_TIME < 39.5f*/)
	{
		samapta();
	}


	glDisable(GL_BLEND);   //disable blending for font scenes
	
	
	

	
	

	
}

void updateEndCredits(void)
{
	//code

	//blendIn Sasarkarta

	if (blendInSadarkarta < 1.0f)            
	{
		blendInSadarkarta = blendInSadarkarta + 0.005f;
		//blendInSadarkarta = blendInSadarkarta + 0.0035f;
	}

	if (blendInSadarkarta >= 1.0f)    //blendOut Sasarkarta
	{

		if (blendOutSadarkarta > 0.0f && ELAPSED_TIME >= SADARKARTA_HALF)
		{
			blendOutSadarkarta = blendOutSadarkarta - 0.005f;
			//blendOutSadarkarta = blendOutSadarkarta - 0.0035f;
		}
	}

	//blend in sangit

	if (blendInSangit < 1.0f && blendOutSadarkarta <=0.0f && ELAPSED_TIME > SANGIT_START)
	{
		blendInSangit = blendInSangit + 0.0035f;
		//blendInSangit = blendInSangit + 0.0025f;
	}

	if (blendInSangit >= 1.0f)    //blendOut sangit 
	{

		if (blendOutSangit > 0.0f && ELAPSED_TIME >= SANGIT_HALF)
		{
			blendOutSangit = blendOutSangit - 0.0035f;
			//blendOutSangit = blendOutSangit - 0.0025f;
		}
	}

	//blend in Technology

	if (blendInTechnology < 1.0f && blendOutSangit <= 0.0f && ELAPSED_TIME > TECHNOLOGY_START)
	{
		//blendInTechnology = blendInTechnology + 0.005f;
		blendInTechnology = blendInTechnology + 0.005f;
	}

	if (blendInTechnology >= 1.0f)    //blendOut Technology
	{

		if (blendOutTechnology > 0.0f && ELAPSED_TIME >= TECHNOLOGY_HALF)
		{
			//blendOutTechnology = blendOutTechnology - 0.005f;
			blendOutTechnology = blendOutTechnology - 0.005f;
		}
	}

	//blend in datastructure

	if (blendInDataStructure < 1.0f && blendOutTechnology <= 0.0f && ELAPSED_TIME > DATASTRUCTURE_START)
	{
		blendInDataStructure = blendInDataStructure + 0.005f;
	}

	if (blendInDataStructure >= 1.0f)
	{

		if (blendOutDataStructure > 0.0f && ELAPSED_TIME >= DATASTRUCTURE_HALF)
		{
			blendOutDataStructure = blendOutDataStructure - 0.005f;
		}
	}

	//blend in Effects

	if (blendInEffects < 1.0f && blendOutDataStructure <= 0.0f && ELAPSED_TIME > EFFECT_START)
	{
		blendInEffects = blendInEffects + 0.005f;
	}

	if (blendInEffects >= 1.0f)
	{

		if (blendOutEffects > 0.0f && ELAPSED_TIME >= EFFECT_HALF)
		{
			blendOutEffects = blendOutEffects - 0.005f;
		}
	}

	//blend in Tools

	if (blendInTools < 1.0f && blendOutEffects <= 0.0f && ELAPSED_TIME > TOOLS_START)
	{
		blendInTools = blendInTools + 0.005f;
	}

	if (blendInTools >= 1.0f)
	{

		if (blendOutTools > 0.0f && ELAPSED_TIME >= TOOLS_HALF)
		{
			blendOutTools = blendOutTools - 0.005f;
		}
	}

	//blend in Margadarshak

	if (blendInMargadarshak < 1.0f && blendOutTools <= 0.0f && ELAPSED_TIME > MARGADARSHAK_START)
	{
		blendInMargadarshak = blendInMargadarshak + 0.005f;
	}

	if (blendInMargadarshak >= 1.0f)
	{

		if (blendOutMargadarshak > 0.0f && ELAPSED_TIME >= MARGADARSHAK_HALF)
		{
			blendOutMargadarshak = blendOutMargadarshak - 0.005f;
		}
	}

	//blend in Abhar

	if (blendInAbhar < 1.0f && blendOutMargadarshak <= 0.0f && ELAPSED_TIME > ABHAR_START)
	{
		blendInAbhar = blendInAbhar + 0.005f;
	}

	if (blendInAbhar >= 1.0f)
	{

		if (blendOutAbhar > 0.0f && ELAPSED_TIME >= ABHAR_HALF)
		{
			blendOutAbhar = blendOutAbhar - 0.005f;
		}
	}

	//blend in Samapt

	if (blendInSamapt < 1.0f && blendOutAbhar <= 0.0f && ELAPSED_TIME > SAMAPT_START)
	{
		blendInSamapt = blendInSamapt + 0.005f;
	}

	if (blendInSamapt >= 1.0f)
	{

		if (blendOutSamapt > 0.0f && ELAPSED_TIME >= SAMAPT_HALF)
		{
			blendOutSamapt = blendOutSamapt - 0.005f;
		}
	}


	
}

void uninitializeEndCredits(void)
{
	//code

	if (TEXTURE_RAMSETU)
	{
		glDeleteTextures(1, &TEXTURE_RAMSETU);

		TEXTURE_RAMSETU = 0;
	}

	if (TEXTURE_SADARKARTA)
	{
		glDeleteTextures(1, &TEXTURE_SADARKARTA);

		TEXTURE_SADARKARTA = 0;
	}

	KillFont();
}

void sadarkarta(void)
{
	
	

	if (blendInSadarkarta >= 1.0f)
	{


		glColor4f(1.0f, 1.0f, 1.0f, blendOutSadarkarta);  //blendOut of Prathamesh Paropkari starts


	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInSadarkarta);   //blendIn of Prathamesh Paropkari starts
	}


	glPushMatrix();        //draw Prathamesh Paropkari
	{
		glTranslatef(0.0f, -0.1f, 0.6f);
		glScalef(0.3f, 0.3f, 1.0f);
		glPrintEx(pMarathiFontData,"paXTamaeSa paraepakarI");
	}
	glPopMatrix();

	glDisable(GL_LIGHTING);

	//glColor4f(1.0f, 1.0f, 1.0f, Blend1);   //blending of horizontal line starts

	glPushMatrix();                //draw horizontal line
	{
		glTranslatef(0.0f, 0.13f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if (blendInSadarkarta >= 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendOutSadarkarta);  //blendOut of Horizontal Line starts
			}

			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendInSadarkarta);   //blendIn of Horizontal Line starts
			}
			//glColor4f(1.0f, 1.0f, 1.0f, blendIn);
			glVertex3f(-0.5f, 0.0f, 0.0f);
			glVertex3f(0.5f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);
	}
	glPopMatrix();



	//glColor4f(1.0f, 1.0f, 1.0f, blendIn);  //blending of sadarkarta starts
	if (blendInSadarkarta >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutSadarkarta);  //blendOut of Sadarkarta starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInSadarkarta);   //blendIn of Sadarkarta starts
	}

	glEnable(GL_LIGHTING);

	glPushMatrix();              //Sadarkarta drawing
	{
		glTranslatef(0.0f, 0.2f, 0.6f);
		glScalef(0.3f, 0.3f, 1.0f);
		glPrintEx(pMarathiFontData,"saadrktaaQ");
	}
	glPopMatrix();

	

	//glDisable(GL_LIGHTING);

	

}
void sangit(void)
{
	//buildfont

	

	if (blendInSangit >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutSangit);  //blendOut of Sangit starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);   //blendIn of Sangit starts
	}


	glPushMatrix();              //Sangit drawing
	{
		glTranslatef(0.0f, 0.3f, 0.6f);
		glScalef(0.3f, 0.3f, 1.0f);
		glPrintEx(pMarathiFontData,"sa/gaIta");

		
	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line

	glPushMatrix();                //draw horizontal line
	{
		
		glTranslatef(0.0f, 0.23f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if (blendInSangit >= 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendOutSangit);  //blendOut of Horizontal Line starts
			}

			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);   //blendIn of Horizontal Line starts
			}
			//glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);
			
			glVertex3f(-0.3f, 0.0f, 0.0f);
			glVertex3f(0.3f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();



	glPushMatrix();              //geet drawing
	{

		if (blendInSangit >= 1.0f)
		{
			glColor4f(1.0f, 1.0f, 1.0f, blendOutSangit);  //blendOut of geet starts
		}

		else
		{
			glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);   //blendIn of geet starts
		}
		glTranslatef(-0.01f, 0.0f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData, "gaIta - saetaU baa/Yaa re saagarI.");


	}
	glPopMatrix();

	glPushMatrix();              //gitkar drawing
	{

		if (blendInSangit >= 1.0f)
		{
			glColor4f(1.0f, 1.0f, 1.0f, blendOutSangit);  //blendOut of geetkar starts
		}

		else
		{
			glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);   //blendIn of geetkar starts
		}
		glTranslatef(-0.01f, -0.2f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData, "gaItakar - ga. id. maaDgaULkr");


	}
	glPopMatrix();

	glPushMatrix();              //Lekhak drawing
	{

		if (blendInSangit >= 1.0f)
		{
			glColor4f(1.0f, 1.0f, 1.0f, blendOutSangit);  //blendOut of geet starts
		}

		else
		{
			glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);   //blendIn of geet starts
		}
		glTranslatef(-0.01f, -0.41f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData,"gaayak - sauYaIr fDke");


	}
	glPopMatrix();

	glEnable(GL_LIGHTING); //enable line to draw further fonts

	

}
void technology(void)
{
	
	
	if (blendInTechnology >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutTechnology);  //blendOut of Sangit starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInTechnology);   //blendIn of Sangit starts
	}


	glPushMatrix();              //Technology drawing
	{
		glTranslatef(0.0f, 0.3f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pEnglishFontData,"Technology");


	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line

	glPushMatrix();                //draw horizontal line
	{

		glTranslatef(0.0f, 0.23f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if (blendInTechnology >= 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendOutTechnology);  //blendOut of Horizontal Line starts
			}

			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendInTechnology);   //blendIn of Horizontal Line starts
			}
			//glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);
			
			glVertex3f(-0.45f, 0.0f, 0.0f);
			glVertex3f(0.45f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (blendInTechnology >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutTechnology);  //blendOut of Sangit starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInTechnology);   //blendIn of Sangit starts
	}

	glPushMatrix();              //Win32 drawing
	{
		glTranslatef(0.0f, 0.05f, 0.6f);
		glScalef(0.1f, 0.1f, 1.0f);
		glPrintEx(pEnglishFontData, "Win32 SDK");


	}
	glPopMatrix();

	if (blendInTechnology >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutTechnology);  //blendOut of Sangit starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInTechnology);   //blendIn of Sangit starts
	}

	glPushMatrix();              //OpenGL drawing
	{
		glTranslatef(0.0f, -0.1f, 0.6f);
		glScalef(0.1f, 0.1f, 1.0f);
		glPrintEx(pEnglishFontData, "OpenGL FFP");


	}
	glPopMatrix();

	if (blendInTechnology >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutTechnology);  //blendOut of Sangit starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInTechnology);   //blendIn of Sangit starts
	}

	glPushMatrix();              //C language drawing
	{
		glTranslatef(0.0f, -0.25f, 0.6f);
		glScalef(0.1f, 0.1f, 1.0f);
		glPrintEx(pEnglishFontData, "C Programming Language");


	}
	glPopMatrix();

	

}
void dataStructure(void)
{
	if (blendInDataStructure >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutDataStructure);  //blendOut of DataStructure starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInDataStructure);   //blendIn of DataStructure starts
	}
	glPushMatrix();              //Data Structure drawing
	{
		glTranslatef(0.0f, 0.3f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pEnglishFontData, "Data Structures");


	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line



	glPushMatrix();                //draw horizontal line
	{

		glTranslatef(0.0f, 0.23f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if(blendInDataStructure >= 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendOutDataStructure);  //blendOut of Horizontal Line starts
			}

			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendInDataStructure);   //blendIn of Horizontal Line starts
			}
			//glColor4f(1.0f, 1.0f, 1.0f, blendInSangit);

			glVertex3f(-0.65f, 0.0f, 0.0f);
			glVertex3f(0.65f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	

	//glPushMatrix();                     //Array drawing
	//{
	//	glTranslatef(0.0f, 0.05f, 0.6f);
	//	glScalef(0.12f, 0.12f, 1.0f);
	//	glPrintEx(pEnglishFontData, "Array");


	//}
	//glPopMatrix();

	glPushMatrix();                     //Vector drawing
	{
		//glTranslatef(0.0f, -0.1f, 0.6f);
		glTranslatef(0.0f, 0.05f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "Vector");


	}
	glPopMatrix();


}
void effects(void)
{

	if (blendInEffects >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutEffects);  //blendOut of Effects starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInEffects);   //blendIn of Effects starts
	}
	

	glPushMatrix();              //Effects drawing
	{
		glTranslatef(0.0f, 0.3f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pEnglishFontData, "Effects");


	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line

	glPushMatrix();                //draw horizontal line
	{

		glTranslatef(0.0f, 0.23f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if (blendInEffects >= 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendOutEffects);  //blendOut of Effects starts
			}

			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendInEffects);   //blendIn of Effects starts
			}
			
			glVertex3f(-0.4f, 0.0f, 0.0f);
			glVertex3f(0.4f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (blendInEffects >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutEffects);  //blendOut of Effects starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInEffects);   //blendIn of Effects starts
	}

	glPushMatrix();                     //Texture drawing
	{
		glTranslatef(0.0f, 0.05f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "Texture");


	}
	glPopMatrix();

	glPushMatrix();                     //Blending drawing
	{
		glTranslatef(0.0f, -0.1f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "Blending");


	}
	glPopMatrix();

	glPushMatrix();                     //Lights drawing
	{
		glTranslatef(0.0f, -0.25f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "Lights");


	}
	glPopMatrix();



}
void tools(void)
{

	if (blendInTools >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutTools);  //blendOut of Tools starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInTools);   //blendIn of Tools starts
	}
	glPushMatrix();              //Tools drawing
	{
		glTranslatef(0.0f, 0.3f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pEnglishFontData, "Tools");


	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line

	glPushMatrix();                //draw horizontal line
	{

		glTranslatef(0.0f, 0.23f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if (blendInTools >= 1.0f)
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendOutTools);  //blendOut of Tools starts
			}

			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, blendInTools);   //blendIn of Tools starts
			}
			
			glVertex3f(-0.35f, 0.0f, 0.0f);
			glVertex3f(0.35f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (blendInTools >= 1.0f)
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendOutTools);  //blendOut of Tools starts
	}

	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, blendInTools);   //blendIn of Tools starts
	}

	glPushMatrix();                     //Audacity drawing
	{
		glTranslatef(0.0f, 0.05f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "Audacity");


	}

	glPopMatrix();

	glPushMatrix();                     //MS Paint drawing
	{
		glTranslatef(0.0f, -0.1f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "MS Paint");


	}
	glPopMatrix();

	glPushMatrix();                     //Obs Studio drawing
	{
		glTranslatef(0.0f, -0.25f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "OBS Studio");


	}
	glPopMatrix();

	glPushMatrix();                     //MS Paint drawing
	{
		glTranslatef(0.0f, -0.4f, 0.6f);
		glScalef(0.12f, 0.12f, 1.0f);
		glPrintEx(pEnglishFontData, "Visual Studio IDE");


	}
	glPopMatrix();
}
void margadarshak(void)
{

	if (blendInMargadarshak >= 1.0f)
	{
		//blendOut of Margadarshak starts
		glColor4f(1.0f, 1.0f, 1.0f, blendOutMargadarshak);  
	}

	else
	{
		//blendIn of Margadarshak starts
		glColor4f(1.0f, 1.0f, 1.0f, blendInMargadarshak);   
	}

	glPushMatrix();        //draw Margadarshak
	{
		glTranslatef(0.0f, 0.3f, 0.6f);
		glScalef(0.3f, 0.3f, 1.0f);
		glPrintEx(pMarathiFontData, "maagaQdSaQk");
	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line

	glPushMatrix();                //draw horizontal line
	{

		glTranslatef(0.0f, 0.25f, 0.6f);

		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{

			if (blendInMargadarshak >= 1.0f)
			{
				//blendOut of Margadarshak starts
				glColor4f(1.0f, 1.0f, 1.0f, blendOutMargadarshak);
			}

			else
			{
				//blendIn of Margadarshak starts
				glColor4f(1.0f, 1.0f, 1.0f, blendInMargadarshak);
			}
			//glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.35f, 0.0f, 0.0f);
			glVertex3f(0.35f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (blendInMargadarshak >= 1.0f)
	{
		//blendOut of Margadarshak starts
		glColor4f(1.0f, 1.0f, 1.0f, blendOutMargadarshak);
	}

	else
	{
		//blendIn of Margadarshak starts
		glColor4f(1.0f, 1.0f, 1.0f, blendInMargadarshak);
	}

	glPushMatrix();                     //Gohale sir drawing 
	{
		glTranslatef(0.0f, 0.0f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData, "Da`. ivajaya gaaeKalae sar.");


	}
	glPopMatrix();

	glPushMatrix();                     //Rama gokhle maam drawing
	{
		glTranslatef(0.0f, -0.2f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData, "Da`. rmaa gaaeKalae ma`Dma.");


	}
	glPopMatrix();

	glPushMatrix();                     //Pratik drawing
	{
		glTranslatef(0.0f, -0.4f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData, "paXzaa gaaeKalae ma`Dma.");


	}
	glPopMatrix();

}
void abhar(void)
{
	if (blendInAbhar >= 1.0f)
	{
		//blendOut of Abhar starts
		glColor4f(1.0f, 1.0f, 1.0f, blendOutAbhar);
	}

	else
	{
		//blendIn of Abhar starts
		glColor4f(1.0f, 1.0f, 1.0f, blendInAbhar);
	}

	glPushMatrix();        //draw abhar
	{
		glTranslatef(0.0f, 0.2f, 0.6f);
		glScalef(0.3f, 0.3f, 1.0f);
		glPrintEx(pMarathiFontData, "ivaSaeVa AaBaar");
	}
	glPopMatrix();

	glDisable(GL_LIGHTING); //disable llighting to draw white horizontal line

	glPushMatrix();                //draw horizontal line
	{

		glTranslatef(0.0f, 0.15f, 0.6f);

		glLineWidth(5.0f);

		glBegin(GL_LINES);
		{

			if (blendInAbhar >= 1.0f)
			{
				//blendOut of Abhar starts
				glColor4f(1.0f, 1.0f, 1.0f, blendOutAbhar);
			}

			else
			{
				//blendIn of Abhar starts
				glColor4f(1.0f, 1.0f, 1.0f, blendInAbhar);
			}
			glVertex3f(-0.55f, 0.0f, 0.0f);
			glVertex3f(0.55f, 0.0f, 0.0f);
		}
		glEnd();
		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (blendInAbhar >= 1.0f)
	{
		//blendOut of Abhar starts
		glColor4f(1.0f, 1.0f, 1.0f, blendOutAbhar);
	}

	else
	{
		//blendIn of Abhar starts
		glColor4f(1.0f, 1.0f, 1.0f, blendInAbhar);
	}

	glPushMatrix();                     //Pratik jagdale drawing
	{
		glTranslatef(0.0f, -0.1f, 0.6f);
		glScalef(0.2f, 0.2f, 1.0f);
		glPrintEx(pMarathiFontData, "paXitak jagadaLe AaiNa gao");


	}
	glPopMatrix();
}
void samapta(void)
{
	if (blendInSamapt >= 1.0f)
	{
		//blendOut of Abhar starts
		glColor4f(1.0f, 1.0f, 1.0f, blendOutSamapt);
	}

	else
	{
		//blendIn of Abhar starts
		glColor4f(1.0f, 1.0f, 1.0f, blendInSamapt);
	}

	glPushMatrix();        //draw abhar
	{
		glTranslatef(0.0f, 0.0f, 0.6f);
		glScalef(0.4f, 0.4f, 1.0f);
		glPrintEx(pMarathiFontData, "samaapta");
	}
	glPopMatrix();
}

