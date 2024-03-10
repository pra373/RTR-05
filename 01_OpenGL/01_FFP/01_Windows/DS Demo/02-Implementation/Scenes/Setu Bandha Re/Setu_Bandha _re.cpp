#include"Setu_Bandha_Re.h"

//movement variables


GLfloat rightMove[11] = { -3.0f, -3.7f, -4.1f, -4.7f, -5.0f, -5.5f, -5.94f, -6.64f, -7.09f, -7.84f, -8.44f };

//Texture variable

GLuint TEXTURE_WANER = 0;
GLuint TEXTURE_WANER_BLUE = 0;
GLuint TEXTURE_WANER_ORANGE = 0;
GLuint TEXTURE_WANER_RED = 0;
GLuint TEXTURE_WANER_TWOCOLOR = 0;
GLuint TEXTURE_WANER_OLIVE = 0;

GLuint TEXTURE_BEACH = 0;

//iteration variable

GLint i;

//function declaration

int initializeWanerSena(void)
{

	BOOL bResult;

	bResult = LoadGLPNGTexture(&TEXTURE_WANER, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Waner.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Waner texture failed !! Exiting now\n");
		return(-20);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_BEACH, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/BeachBackground2.png");  //
	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Beach Background texture failed !! Exiting now\n");
		return(-21);
	}



	bResult = LoadGLPNGTexture(&TEXTURE_WANER_BLUE, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Waner_Blue.png");
	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Waner Blue image loading texture failed !! Exiting now\n");
		return(-22);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_WANER_ORANGE, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Waner_Orange.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Waner Orange image loading texture failed !! Exiting now\n");
		return(-23);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_WANER_RED, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Waner_Red.png");
	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Waner Red image loading texture failed !! Exiting now\n");
		return(-24);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_WANER_TWOCOLOR, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Waner_TwoColors.png");
	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Waner TwoColors image loading texture failed !! Exiting now\n");
		return(-25);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_WANER_OLIVE, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Waner_Olive.png");
	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Waner Olive image loading texture failed !! Exiting now\n");
		return(-26);
	}

	return(0);
}
void displayWanerSena(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	background();
	Waner1();
	Waner2();
	Waner3();
	Waner4();
	Waner5();
	Waner6();
	Waner7();
	Waner8();
	Waner9();
	Waner10();
	Waner11();



	glDisable(GL_BLEND);

}
void updateWanerSena(void)
{

	for (i = 0; i < 11; i++)
	{
		if (rightMove[i] > 3.0f)
		{
			rightMove[i] = -3.0f;
		}

		else
		{
			rightMove[i] += 0.002;
		}
		
	}
	/*rightMove1 = rightMove1 + 0.002;

	if (rightMove1 > 3.0f)
	{
		rightMove1 = -3.0f;
	}

	rightMove2 = rightMove2 + 0.002;

	if (rightMove2 > 3.0f)
	{
		rightMove2 = -3.0f;
	}

	rightMove3 = rightMove3 + 0.002;

	if (rightMove3 > 3.0f)
	{
		rightMove3 = -3.0f;
	}

	rightMove4 = rightMove4 + 0.002;

	if (rightMove4 > 3.0f)
	{
		rightMove4 = -3.0f;
	}

	rightMove5 = rightMove5 + 0.002;

	if (rightMove5 > 3.0f)
	{
		rightMove5 = -3.0f;
	}

	rightMove6 = rightMove6 + 0.002;

	if (rightMove6 > 3.0f)
	{
		rightMove6 = -3.0f;
	}

	rightMove7 = rightMove7 + 0.002;

	if (rightMove7 > 3.0f)
	{
		rightMove7 = -3.0f;
	}

	rightMove8 = rightMove8 + 0.002;

	if (rightMove8 > 3.0f)
	{
		rightMove8 = -3.0f;
	}

	rightMove9 = rightMove9 + 0.002;

	if (rightMove9 > 3.0f)
	{
		rightMove9 = -3.0f;
	}

	rightMove10 = rightMove10 + 0.002;

	if (rightMove10 > 3.0f)
	{
		rightMove10 = -3.0f;
	}

	rightMove11 = rightMove11 + 0.002;

	if (rightMove11 > 3.0f)
	{
		rightMove11 = -3.0f;
	}*/


}
void uninitializeWanerSena(void)
{
	if (TEXTURE_WANER)
	{
		glDeleteTextures(1, &TEXTURE_WANER);
		TEXTURE_WANER = 0;
	}

	if (TEXTURE_WANER_BLUE)
	{
		glDeleteTextures(1, &TEXTURE_WANER_BLUE);
		TEXTURE_WANER_BLUE = 0;
	}

	if (TEXTURE_BEACH)
	{
		glDeleteTextures(1, &TEXTURE_BEACH);
		TEXTURE_BEACH = 0;
	}

	if (TEXTURE_WANER_ORANGE)
	{
		glDeleteTextures(1, &TEXTURE_WANER_ORANGE);
		TEXTURE_WANER_ORANGE = 0;
	}

	if (TEXTURE_WANER_RED)
	{
		glDeleteTextures(1, &TEXTURE_WANER_RED);
		TEXTURE_WANER_RED = 0;
	}

	if (TEXTURE_WANER_TWOCOLOR)
	{
		glDeleteTextures(1, &TEXTURE_WANER_TWOCOLOR);
		TEXTURE_WANER_TWOCOLOR = 0;
	}

	if (TEXTURE_WANER_OLIVE)
	{
		glDeleteTextures(1, &TEXTURE_WANER_OLIVE);
		TEXTURE_WANER_OLIVE = 0;
	}
	
}

void background(void)
{
	glPushMatrix();                                 //beach background

	{

		glTranslatef(0.0f, 0.0f, 0.58f);

		glScalef(2.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_BEACH);

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
}
void Waner1(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_OLIVE);

	glPushMatrix();
	{

		glTranslatef(rightMove[0], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);   //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner2(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_RED);

	glPushMatrix();
	{

		glTranslatef(rightMove[1], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);   //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner3(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_TWOCOLOR);

	glPushMatrix();
	{

		glTranslatef(rightMove[2], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);   //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner4(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER);

	glPushMatrix();
	{

		glTranslatef(rightMove[3], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner5(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_ORANGE);

	glPushMatrix();
	{

		glTranslatef(rightMove[4], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner6(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_RED);

	glPushMatrix();
	{

		glTranslatef(rightMove[5], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner7(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_TWOCOLOR);

	glPushMatrix();
	{

		glTranslatef(rightMove[6], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();

}
void Waner8(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_BLUE);

	glPushMatrix();
	{

		glTranslatef(rightMove[7], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();

}
void Waner9(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_ORANGE);

	glPushMatrix();
	{

		glTranslatef(rightMove[8], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner10(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_OLIVE);

	glPushMatrix();
	{

		glTranslatef(rightMove[9], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}
void Waner11(void)
{
	glBindTexture(GL_TEXTURE_2D, TEXTURE_WANER_RED);

	glPushMatrix();
	{

		glTranslatef(rightMove[10], -0.3f, 0.59f);
		glScalef(0.4f, 0.4f, 1.0f);

		glBegin(GL_QUADS);


		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);   //right top

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);  //right bottom



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();


}