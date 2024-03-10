#include "Chimni.h"

//texture variable
GLuint TEXTURE_CLOUD1 = 0;
GLuint TEXTURE_CLOUD2 = 0;

GLuint TEXTURE_CLOUD3 = 0;

GLuint TEXTURE_SPARROW = 0;

GLuint TEXTURE_SPARROW2 = 0;

GLuint TEXTURE_SPARROW3 = 0;


GLuint TEXTURE_BEACH2 = 0;




//movement variables

GLfloat zMovement = 2.0f;

GLfloat xMovementSparrow1 = -5.0f;

GLfloat xMovementSparrow2 = -4.0f;

GLfloat xMovementSparrow3 = -6.0f;


int initializeChimini(void)
{

	BOOL bResult;

	

	bResult = LoadGLPNGTexture(&TEXTURE_CLOUD2, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Cloud2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of cloud2 texture failed !! Exiting now\n");
		return(-47);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_CLOUD1, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Cloud1.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of cloud2 texture failed !! Exiting now\n");
		return(-47);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_CLOUD3, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Cloud3.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of cloud2 texture failed !! Exiting now\n");
		return(-47);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_SPARROW, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/sparrow.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Sparrow texture failed !! Exiting now\n");
		return(-47);
	}


	

	bResult = LoadGLPNGTexture(&TEXTURE_SPARROW2, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Sparrow3.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Beach texture failed !! Exiting now\n");
		return(-48);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_SPARROW3, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Sparrow6.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Beach texture failed !! Exiting now\n");
		return(-48);
	}

	return TRUE;
}
void displayChimini(void)
{
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	{
		glScalef(2.5f, 2.0f, 1.0f);
		glBegin(GL_QUADS);                      //sky buildup
		{
			glColor3f((135.0f / 256.0f), (206.0f / 256.0f), (235.0f / 256.0f));

			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);

			glColor3f(1.0f, 1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);



		}
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();                
	{
		glTranslatef(-1.6f, 0.7f,0.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_CLOUD2);  //cloud 1 left most
		
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

	glPushMatrix();        //cloud 2 middle
	{
		glTranslatef(-0.1f, 0.7f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_CLOUD1);

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

	glPushMatrix();        //cloud 3 right most
	{
		glTranslatef(1.4f, 0.7f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_CLOUD3);

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

	glPushMatrix();        //Sparrow
	{
		glScalef(0.7f, 0.7f, 1.0f);
		//glTranslatef(-1.4f, 0.0f, 0.0f);

		glTranslatef(xMovementSparrow1, 0.1f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_SPARROW);

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

	glPushMatrix();        //Sparrow2
	{
		glScalef(0.6f, 0.6f, 1.0f);
		//glTranslatef(-0.3f, -0.3f, 0.0f);

		glTranslatef(xMovementSparrow2, -0.2f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_SPARROW2);

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

	glPushMatrix();        //Sparrow3
	{
		glScalef(0.6f, 0.6f, 1.0f);
		//glTranslatef(-2.0f, -0.9f, 0.0f);

		glTranslatef(xMovementSparrow3, -0.8f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_SPARROW3);

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


	


	

	

	glDisable(GL_BLEND);
}
void updateChimini(void)
{

	if (xMovementSparrow1 < 5.0f)
	{
		xMovementSparrow1 = xMovementSparrow1 + 0.0086f;
	}

	if (xMovementSparrow2 < 5.0f)
	{
		xMovementSparrow2 = xMovementSparrow2 + 0.0093f;
	}

	if (xMovementSparrow3 < 5.0f)
	{
		xMovementSparrow3 = xMovementSparrow3 + 0.0091f;
	}

}
void uninitializeChimini(void)
{
	if (TEXTURE_CLOUD1)
	{
		glDeleteTextures(1, &TEXTURE_CLOUD1);
		TEXTURE_CLOUD1 = 0;
	}

	if (TEXTURE_CLOUD2)
	{
		glDeleteTextures(1, &TEXTURE_CLOUD2);
		TEXTURE_CLOUD2 = 0;
	}

	if (TEXTURE_CLOUD3)
	{
		glDeleteTextures(1, &TEXTURE_CLOUD3);
		TEXTURE_CLOUD3 = 0;
	}

	if (TEXTURE_SPARROW)
	{
		glDeleteTextures(1, &TEXTURE_SPARROW);
		TEXTURE_SPARROW = 0;
	}

	if (TEXTURE_SPARROW2)
	{
		glDeleteTextures(1, &TEXTURE_SPARROW2);
		TEXTURE_SPARROW2 = 0;
	}

	if (TEXTURE_SPARROW3)
	{
		glDeleteTextures(1, &TEXTURE_SPARROW3);
		TEXTURE_SPARROW3 = 0;
	}
}



