// Header files 
#include <GL/freeglut.h>

// Global variable declarations
bool bIsFullScreen = false;

// Entry-point function
int main(int argc, char *argv[])
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

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("My First RTR5 Programe : Prathamesh Laxmikant Paropkari");

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

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

}

void display(void)
{
	// code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.66f, 0.8f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.44f, 0.8f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.685f, -0.785f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.45f, -0.785f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.38f, 0.8f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.62f, 0.8f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.445f, -0.785f, 0.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.685f, -0.785f, 0.0f);

	glEnd();
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





