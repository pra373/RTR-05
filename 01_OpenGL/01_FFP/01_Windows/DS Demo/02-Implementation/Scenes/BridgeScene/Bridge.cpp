#include"Bridge.h"
#include<iostream>
#include<vector>

#define _USE_MATH_DEFINES
#include<math.h>

//global variables

int indexupdate = 0;

using namespace std;

vector <GLfloat> xcPoints;
vector <GLfloat> ycPoints;

//texture variables

GLuint TEXTURE_BRIDGE = 0;

GLint initializeBridgeScene(GLvoid)
{
	BOOL bResult;

	bResult = LoadGLPNGTexture(&TEXTURE_BRIDGE, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Setu.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of setu image texture failed !! Exiting now\n");
		return(-60);
	}

	//initialize sine wave

	initializeSineWave(0.4f, 4.5f);

	return(TRUE);

}
GLvoid displayBridgeScene(GLvoid)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	{
		glScalef(1.5f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_BRIDGE);

		glBegin(GL_QUADS);



		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
	

	glPushMatrix();
	{
		glTranslatef(-0.027f, 0.1f, 1.1f);
		//glScalef(0.27f, 0.3f, 1.0f);

		glScalef(0.4f, 0.3f, 1.0f);
		glRotatef(45.8, 0.0f, 0.0f, 1.0f);
		//drawSineWave(0.4f, 4.5f);
		drawSineWave();
	}
	glPopMatrix();

}
GLvoid updateBridgeScene(GLvoid)
{
	if (indexupdate < xcPoints.size())
	{
		indexupdate += 1;
	}
}
GLvoid uninitializeBridgeScene(GLvoid)
{
	if (TEXTURE_BRIDGE)
	{
		glDeleteTextures(1, &TEXTURE_BRIDGE);
		TEXTURE_BRIDGE = 0;
	}
}

GLvoid initializeSineWave(GLfloat amplitude, GLfloat wavelength)
{
	GLfloat x, y;

	for (x = 0.0f; x <= 3.0f; x = x + 0.001)
	{
		y = (amplitude * (sin((2 * M_PI * x) / wavelength)));

		xcPoints.push_back(x);
		ycPoints.push_back(y);
	}
}

GLvoid drawSineWave(GLvoid)   // by default translate the wave z = 0.0f;
{
	GLint i = 0;

	glTranslatef(-1.5f, 0.0f, 0.0f);

	glPointSize(11.0f);

	glBegin(GL_POINTS);

	//glColor3f((189.0f/255.0f),(124.0f/255.0f),(89.0f/255.0f));   //brown coor

	glColor3f((110.0f / 255.0f), (110.0f / 255.0f), (110.0f / 255.0f));

	/*for (i = 0; i < xcPoints.size(); i++)
	{
		glVertex2f(xcPoints[i], ycPoints[i]);
	}*/



	for (i = 0; i < indexupdate; i++)
	{

		glVertex2f(xcPoints[i], ycPoints[i]);

	}

	//glPointSize(1.0f);


	glEnd();
}