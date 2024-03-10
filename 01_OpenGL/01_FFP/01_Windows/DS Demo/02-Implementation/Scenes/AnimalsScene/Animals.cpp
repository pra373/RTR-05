#include"Animals.h"

GLuint TEXTURE_BACKGROUND = 0;
GLuint TEXTURE_ELEPHANT = 0;
GLuint TEXTURE_SQUIREEL = 0;
GLuint TEXTURE_DEERCART = 0;
GLuint TEXTURE_TURTLE = 0;

//movement variables

GLfloat xElephant = -1.9f;
GLfloat xSquireel = -3.1f;

GLfloat xDeerCart = -2.9f;

GLfloat xTurtle = -3.2f;

int initializeAnimals(void)
{
	BOOL bResult;



	bResult = LoadGLPNGTexture(&TEXTURE_BACKGROUND, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Beach3.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of beach animal scene texture failed !! Exiting now\n");
		return(-54);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_ELEPHANT, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Elephant4.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of beach animal elephant scene texture failed !! Exiting now\n");
		return(-54);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_SQUIREEL, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Squireel.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of beach animal elephant scene texture failed !! Exiting now\n");
		return(-54);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_DEERCART, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/DeerCart2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of beach animal deer cart scene texture failed !! Exiting now\n");
		return(-55);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_TURTLE, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Tortoise.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of beach animal Tortoise texture failed !! Exiting now\n");
		return(-56);
	}

	return(TRUE);
}
void displayAnimals(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	{


		glTranslatef(0.0f, 0.0f, 0.58f);

		glScalef(2.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_BACKGROUND);

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

	displayElephant();
	displaySquirrel();
	displayDeerCart();
	displayTurtle();

	
}




void updateAnimals(void)
{
	if (xElephant <= 2.5f)
	{
		xElephant += 0.0042f;
	}

	if (xElephant > 2.3f && xSquireel < 3.1f)
	{
		xSquireel += 0.0051f;
	}

	if (xSquireel >= 2.9f && xDeerCart <=2.9f)
	{
		xDeerCart += 0.0038f;
	}

	if (xDeerCart >= 2.3f && xTurtle <= 3.1f)
	{
		xTurtle += 0.0045f;
	}
}
void uninitializeAnimals(void)
{
	if (TEXTURE_BACKGROUND)
	{
		glDeleteTextures(1, &TEXTURE_BACKGROUND);
		TEXTURE_BACKGROUND = 0;
	}

	if (TEXTURE_ELEPHANT)
	{
		glDeleteTextures(1, &TEXTURE_ELEPHANT);
		TEXTURE_ELEPHANT = 0;
	}

	if (TEXTURE_SQUIREEL)
	{
		glDeleteTextures(1, &TEXTURE_SQUIREEL);
		TEXTURE_SQUIREEL = 0;
	}
}

void displayElephant(void)
{
	glPushMatrix();        //Elephant
	{
		glScalef(0.9f, 0.9f, 1.0f);
		//glTranslatef(-1.4f, 0.0f, 0.0f);

		//glTranslatef(-1.0f, -0.3f, 1.0f);

		glTranslatef(xElephant, -0.3f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_ELEPHANT);

		glBegin(GL_QUADS);
		{



			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, 0.1f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-0.5f, 0.5f, 0.1f);


			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.1f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.1f);



		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);


	}
	glPopMatrix();
}

void displaySquirrel(void)
{
	glPushMatrix();        //Elephant
	{
		glScalef(0.6f, 0.6f, 1.0f);
		//glTranslatef(-1.4f, 0.0f, 0.0f);

		//glTranslatef(-1.0f, -0.3f, 1.0f);

		//glTranslatef(0.5f, -0.7f, 1.0f);

		glTranslatef(xSquireel, -0.7f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_SQUIREEL);

		glBegin(GL_QUADS);
		{



			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, 0.1f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-0.5f, 0.5f, 0.1f);


			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.1f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.1f);



		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);


	}
	glPopMatrix();
}

void displayDeerCart(void)
{
	glPushMatrix();        //Elephant
	{
		glScalef(1.3f, 1.3f, 1.0f);
		//glTranslatef(-1.4f, 0.0f, 0.0f);

		//glTranslatef(-1.0f, -0.3f, 1.0f);

		//glTranslatef(0.5f, -0.7f, 1.0f);

		//glTranslatef(0.0f, -0.27f, 1.0f);

		glTranslatef(xDeerCart, -0.29f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_DEERCART);

		glBegin(GL_QUADS);
		{



			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, -0.1f);

			glTexCoord2f(0.0f, 1.0f);
			//glVertex3f(-0.5f, 0.5f, 0.1f);
			glVertex3f(-0.5f, 0.5f, -0.1f);


			glTexCoord2f(0.0f, 0.0f);
			/*glVertex3f(-0.5f, -0.5f, 0.1f);*/
			glVertex3f(-0.5f, -0.5f, -0.1f);



			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, -0.1f);



		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);


	}
	glPopMatrix();
}

void displayTurtle(void)
{
	glPushMatrix();        //Elephant
	{
		glScalef(0.6f, 0.6f, 1.0f);
		//glTranslatef(-1.4f, 0.0f, 0.0f);

		//glTranslatef(-1.0f, -0.3f, 1.0f);

		//glTranslatef(0.5f, -0.7f, 1.0f);

		//glTranslatef(0.0f, -0.27f, 1.0f);

		//glTranslatef(xTurtle, -0.29f, 1.0f);

		glTranslatef(xTurtle, -0.9f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_TURTLE);

		glBegin(GL_QUADS);
		{



			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, -0.1f);

			glTexCoord2f(0.0f, 1.0f);
			//glVertex3f(-0.5f, 0.5f, 0.1f);
			glVertex3f(-0.5f, 0.5f, -0.1f);


			glTexCoord2f(0.0f, 0.0f);
			/*glVertex3f(-0.5f, -0.5f, 0.1f);*/
			glVertex3f(-0.5f, -0.5f, -0.1f);



			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, -0.1f);



		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);


	}
	glPopMatrix();
}