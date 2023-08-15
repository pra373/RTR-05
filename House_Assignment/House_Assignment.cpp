// Header files 
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include<math.h>


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
	//variable declaration
	 
	float radious_door_handle = 0.01f;

	// code
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 1.0f);                 // base rectangle
	glVertex3f(0.5f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, -0.6f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.5f, -0.6f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);                        // trianglefor roof

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                            // left roof

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(-0.7f, -0.1f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(-0.6f, -0.1f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                          // right roof

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.6f, -0.1f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.7f, -0.1f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.0f, 0.6f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               // smoke outlet

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.4f, 0.4f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.3f, 0.4f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.3f, 0.3f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.4f, 0.2f, 0.0f);

	glEnd();

	glLineWidth(5.0f);

	glBegin(GL_LINES);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.4f, 0.4f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.3f, 0.4f, 0.0f);

	glEnd();

	glLineWidth(1.0f);



	

	glBegin(GL_QUADS);                // Top window

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.1f, 0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.1f, 0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                       //left window

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.2f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.4f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.4f, -0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.2f, -0.3f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);                  //right window

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.4f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.2f, -0.1f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.2f, -0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.4f, -0.3f, 0.0f);

	glEnd();

	glBegin(GL_QUADS);               // door 

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.1f, -0.2f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(-0.1f, -0.2f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(-0.1f, -0.6f, 0.0f);

	glColor3f(0.7f, 0.3f, 0.0f);
	glVertex3f(0.1f, -0.6f, 0.0f);

	glEnd();

	glLineWidth(3.0f);

	glBegin(GL_LINES);                      // vetical line left window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.1f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.3f, -0.3f, 0.0f);

	glEnd();


	glBegin(GL_LINES);                        // horizontal line left window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.4f, -0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                    // vetical line right window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.1f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                     // horizontal line right window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.4f, -0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                     // vertical line top window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.3f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.0f);

	glEnd();

	glBegin(GL_LINES);                     // horizontal line top window

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.2f, 0.0f);

	glEnd();

	glLineWidth(1.0f);

	glBegin(GL_LINES);                          // door handle 



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_door_handle * cos(angle) + (-0.0725);
		float y = radious_door_handle * sin(angle) + (-0.4);



		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(-0.0725, -0.4, 0.0f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

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





