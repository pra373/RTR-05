// Header files 
#include <GL/freeglut.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include<math.h>

// Global variable declarations
bool bIsFullScreen = false;

// Entry-point function
int main(int argc, char* argv[])
{
	// Function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialized(void);
	

	// Code
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);


	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(150, 20);
	glutCreateWindow("RTR-05 (freeglut) Texture group Introduction demo");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialized);

	glutMainLoop();   // heart of application

	return 0;

}

void initialize(void)
{
	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
	// code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height); glLineWidth(3);

}

void display(void)
{
	// code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_LINES);

	//Outer shape
	
	glVertex2f(0.8f, 0.6f);
	glVertex2f(-0.6f, 0.6f);

	glVertex2f(-0.6f, 0.6f);
	glVertex2f(-0.8f, 0.4f);

	glVertex2f(-0.8f, 0.4f);
	glVertex2f(-0.8f, -0.6f);

	glVertex2f(-0.8f, -0.6f);
	glVertex2f(0.6f, -0.6f);

	glVertex2f(0.6f, -0.6f);
	glVertex2f(0.8f, -0.4f);

	glVertex2f(0.8f, -0.4f);
	glVertex2f(0.8f, 0.6f);

	//First rectangle

	glVertex2f(-0.3f, 0.4f);
	glVertex2f(-0.6f, 0.4f);

	glVertex2f(-0.6f, 0.4f);
	glVertex2f(-0.6f, 0.3f);

	glVertex2f(-0.6f, 0.3f);
	glVertex2f(-0.3f, 0.3f);

	glVertex2f(-0.3f, 0.3f);
	glVertex2f(-0.3f, 0.4f);

	//Second rectangle

	glVertex2f(0.1f, 0.4f);
	glVertex2f(-0.2f, 0.4f);

	glVertex2f(-0.2f, 0.4f);
	glVertex2f(-0.2f, 0.3f);

	glVertex2f(-0.2f, 0.3f);
	glVertex2f(0.1f, 0.3f);

	glVertex2f(0.1f, 0.3f);
	glVertex2f(0.1f, 0.4f);

	//Third rectangle

	glVertex2f(0.5f, 0.4f);
	glVertex2f(0.2f, 0.4f);

	glVertex2f(0.2f, 0.4f);
	glVertex2f(0.2f, 0.3f);

	glVertex2f(0.2f, 0.3f);
	glVertex2f(0.5f, 0.3f);

	glVertex2f(0.5f, 0.3f);
	glVertex2f(0.5f, 0.4f);

	

	
	//Shape in which all the quotes are written.

	glVertex2f(0.7f, 0.2f);
	glVertex2f(0.0f, 0.2f);

	glVertex2f(0.0f, 0.2f);
	glVertex2f(0.05f, 0.15f);

	glVertex2f(0.05f, 0.15f);
	glVertex2f(0.0f, 0.1f);

	glVertex2f(0.0f, 0.1f);
	glVertex2f(0.0f, -0.3f);

	glVertex2f(0.0f, -0.3f);
	glVertex2f(0.05f, -0.35f);

	glVertex2f(0.05f, -0.35f);
	glVertex2f(0.0f, -0.4f);

	glVertex2f(0.0f, -0.4f);
	glVertex2f(0.7f, -0.4f);

	glVertex2f(0.7f, -0.4f);
	glVertex2f(0.7f, 0.2f);




	glEnd();

	//First white polygon
	glBegin(GL_POLYGON);

	glVertex2f(-0.4f, 0.2f);
	glVertex2f(-0.7f, 0.2f);
	glVertex2f(-0.7f, 0.15f);
	glVertex2f(-0.4f, 0.15f);

	glEnd();

	//Second white polygon

	glBegin(GL_POLYGON);

	glVertex2f(-0.55f, 0.0f);
	glVertex2f(-0.7f, 0.0f);
	glVertex2f(-0.7f, -0.05f);
	glVertex2f(-0.55f, -0.05f);

	glEnd();

	//Third white polygon


	glBegin(GL_POLYGON);

	glVertex2f(-0.5f, -0.2f);
	glVertex2f(-0.7f, -0.2f);
	glVertex2f(-0.7f, -0.25f);
	glVertex2f(-0.5f, -0.25f);


	glEnd();
	
	//Forth white polygon

	glBegin(GL_POLYGON);

	glVertex2f(-0.4f, -0.4f);
	glVertex2f(-0.7f, -0.4f);
	glVertex2f(-0.7f, -0.45f);
	glVertex2f(-0.4f, -0.45f);

	glEnd();

	glBegin(GL_POLYGON);       // Middle Shapes (First rectangle)

	glVertex2f(-0.175f, 0.2f);
	glVertex2f(-0.19f, 0.2f);
	glVertex2f(-0.19f, 0.1f);
	glVertex2f(-0.175f, 0.1f);

	glEnd();

	glBegin(GL_POLYGON);      // Middle Shapes (Second rectangle)

	glVertex2f(-0.21f, 0.2f);
	glVertex2f(-0.225f, 0.2f);
	glVertex2f(-0.225f, 0.1f);
	glVertex2f(-0.21f, 0.1f);

	glEnd();

	glBegin(GL_LINES);          // Middle Shapes (Third Ploygon)

	glVertex2f(-0.2f, 0.0f);
	glVertex2f(-0.225f, -0.05f);
	glVertex2f(-0.225f, -0.05f);
	glVertex2f(-0.2f, -0.1f);
	glVertex2f(-0.2f, -0.1f);
	glVertex2f(-0.175f, -0.05f);
	glVertex2f(-0.175f, -0.05f);
	glVertex2f(-0.2f, 0.0f);

	glEnd();

	float radious1 = 0.05f;
	float radious2 = 0.01f;

	glBegin(GL_POINTS);         //Middle shapes (Outer circle)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious1 * cos(angle) + (-0.2);
		float y = radious1 * sin(angle) + (-0.25);

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		//glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_POINTS);              //Middle shapes (Inner circle)

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f) 
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious2 * cos(angle) + (-0.2);
		float y = radious2 * sin(angle) + (-0.25);

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		//glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);  //Middle shapes (square)

	glVertex2f(-0.15f, -0.4f);
	glVertex2f(-0.25f, -0.4f);

	glVertex2f(-0.25f, -0.4f);
	glVertex2f(-0.25f, -0.5f);

	glVertex2f(-0.25f, -0.5f);
	glVertex2f(-0.15f, -0.5f);

	glVertex2f(-0.15f, -0.5f);
	glVertex2f(-0.15f, -0.4f);

	glEnd();

    //First quote

	float x1 = 0.1f;
	float y1 = 0.1f;
	char* str1 = "Knowledge is Interrelated.";
	char* temp1;

	glRasterPos2f(x1, y1);

	for(temp1=str1;*temp1!='\0';temp1++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *temp1);
	}

	//Second quote

	float x2 = 0.1f;
	float y2 = 0.02f;
	char* temp2;
	char* str2 = "The highest truth is always the simplest.";
	glRasterPos2f(x2, y2);

	for (temp2 = str2; *temp2 != '\0'; temp2++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *temp2);
	}

	//Third quote

	float x3 = 0.1f;
	float y3 = -0.06f;
	char* temp3;
	char* str3 = "Dependence is misery, Independence is happyness.";
	glRasterPos2f(x3, y3);

	for (temp3 = str3; *temp3 != '\0'; temp3++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *temp3);
	}

	//Fourth Quote

	float x4 = 0.1f;
	float y4 = -0.14f;
	char* temp4;
	char* str4 = "All troubles come to an end when the ego dies.";
	glRasterPos2f(x4, y4);

	for (temp4 = str4; *temp4 != '\0'; temp4++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *temp4);
	}

	//Fifth quote

	float x5 = 0.1f;
	float y5 = -0.22f;
	char* temp5;
	char* str5 = "A day without laughter is a day wasted.";
	glRasterPos2f(x5, y5);

	for (temp5 = str5; *temp5 != '\0'; temp5++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *temp5);
	}

	//Sixth quote

	float x6 = 0.1f;
	float y6 = -0.3f;
	char* temp6;
	char* str6 = "Long-term consistency trumps short-term intensity.";
	glRasterPos2f(x6, y6);

	for (temp6 = str6; *temp6 != '\0'; temp6++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *temp6);
	}


	glutSwapBuffers();


}

void keyboard(unsigned char key, int x, int y)
{
	// code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bIsFullScreen == false)
		{
			glutFullScreen();
			bIsFullScreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bIsFullScreen = false;
		}
		break;

	default:
		break;

	}

}

void mouse(int button, int state, int x, int y)
{
	// code
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

}

void uninitialized(void)
{
	// code

}






