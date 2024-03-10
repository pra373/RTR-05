#include "Logo.h"
#include "../Common.h"

//texture variables

GLuint TEXTURE_LOGO1 = 0;
GLuint TEXTURE_LOGO2 = 0;

//3D variable
GLUquadric* Cquadric = NULL;
GLUquadric* D1quadric = NULL;
GLUquadric* D2quadric = NULL;

//rotation variable

GLfloat angle = 0.0f;

//Translation Variable

GLfloat zPosition = -20.0f;
GLfloat yPosition = 1.5f;

//blend variable

GLfloat opacityIn = 0.0f;
GLfloat opacityOut = 1.0f;

//flag variables

GLint flag = 0;
GLint flagFadeIn = 0;
GLint flagFadeOut = 0;









int logoInitialize(void)
{
	BOOL bResult;

	bResult = LoadGLPNGTexture(&TEXTURE_LOGO1, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Logo2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Logo image texture failed !! Exiting now\n");
		return(-11);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_LOGO2, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Logo4.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Logo image texture failed !! Exiting now\n");
		return(-11);
	}

	Cquadric = gluNewQuadric();
	D1quadric = gluNewQuadric();
	D2quadric = gluNewQuadric();

	return(1);

}



void logoDisplay(void)
{

	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);






	//glTranslatef(0.0f, 0.0f, zPosition);

	//glTranslatef(0.0f, yPosition, 0.0f);

	if (flagFadeIn == 1)
	{
		glColor4f(1.0f, 1.0f, 1.0f, opacityIn);
	}

	
	if (flagFadeOut == 1)
	{
		glColor4f(1.0f, 1.0f, 1.0f, opacityOut);
	}



	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	{
		gluCylinder(Cquadric, 0.4f, 0.4f, 0.1, 30, 30);
	}
	glPopMatrix();


	if (flag == 0)
	{
		glPushMatrix();
		{

			glBindTexture(GL_TEXTURE_2D, TEXTURE_LOGO1);


			gluQuadricTexture(D1quadric, GL_TRUE);




			gluDisk(D1quadric, 0.0f, 0.4, 30, 30);    // draw disk 1

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glPopMatrix();

	}



	if (flag == 1)
	{
		glPushMatrix();

		{
			glBindTexture(GL_TEXTURE_2D, TEXTURE_LOGO2);

			glTranslatef(0.0f, 0.0f, 0.1f);

			gluQuadricTexture(D2quadric, GL_TRUE);

			gluDisk(D2quadric, 0.0f, 0.4, 30, 30);  // draw disk 2

			glBindTexture(GL_TEXTURE_2D, 0);

		}

		glPopMatrix();

	}




	glDisable(GL_LIGHTING);

	glDisable(GL_BLEND);

}

void logoUpdate(void)
{
	/*angle = angle + 0.2f;

	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}*/

	angle = angle + 0.2f;

	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}

	

	if (opacityIn < 1.0f)                      //fade in effect
	{
		opacityIn = opacityIn + 0.0015;
		flagFadeIn = 1;

	}

	if (opacityIn >= 1 && opacityOut > 0.0f)     //fade out effect
	{
		opacityOut = opacityOut - 0.0015;
		flagFadeOut = 1;
	}


	if (angle <= 90.0f || angle > 270.0f)
	{
		flag = 1;
	}


	if (angle > 90.0f && angle <= 270.0f)
	{
		flag = 0;
	}



}


void logoUninitialize(void)
{
	if (TEXTURE_LOGO1)
	{
		glDeleteTextures(1, &TEXTURE_LOGO1);

		TEXTURE_LOGO1 = 0;
	}

	if (TEXTURE_LOGO2)
	{
		glDeleteTextures(1, &TEXTURE_LOGO2);

		TEXTURE_LOGO2 = 0;
	}




}













