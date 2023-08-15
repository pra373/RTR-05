// Header files 
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include<math.h>

// Global variable declarations
bool bIsFullScreen = false;
float x_axis = 0.0f;
float y_axis = 0.0f;

float radious_Face = 0.5f;
float radious_eye1 = 0.1f;
float radious_eye2 = 0.1f;
float radious_sad_face = 0.2f;
float radious_smile = 0.2f;
float radious_mouth = 0.03f;

//Global functions declarations
void Display_Normal_Face(void);
void Display_Sad_face(void);
void Display_Curious_Face(void);
void Extreme_Happy_Face(void);


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
	void Display_Normal_Face(void);
	
	//void smile(void);
	//void sad_face(void);
	//void Expression_less_face(void);

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
	glLineWidth(2.0f);

	glBegin(GL_LINES);                          //face 

	

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Face * cos(angle);
		float y = radious_Face * sin(angle);

		

		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x_axis, y_axis, 0.0f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);                                 // left eye

	

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye1 * cos(angle) + (-0.2f);
		float y = radious_eye1 * sin(angle) + (0.2f);

		

		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);                    //right eye

	

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye2 * cos(angle) + (0.2f);
		float y = radious_eye2 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_POINTS);                    

	

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_smile * cos((angle*M_PI)/180.0f) + (0.0f);
		float y = radious_smile * sin((angle*M_PI)/180.0f) + (-0.20f);

		if (angle >= 180.0f)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0, -0.20, 0.0f);


			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}
		
			

	}

	glEnd();

	glutPostRedisplay();
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

	case 'N':
	case 'n':

		glutDisplayFunc(Display_Normal_Face);

		break;

	case 'H':
	case 'h':

		glutDisplayFunc(display);
		break;

	case 'S':
	case 's':

		glutDisplayFunc(Display_Sad_face);
		break;

	case 'C':
	case 'c':

		glutDisplayFunc(Display_Curious_Face);
		break;

	case 'E':
	case 'e':

		glutDisplayFunc(Extreme_Happy_Face);
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

void Display_Normal_Face(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(2.0f);

	glBegin(GL_LINES);                          //face

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Face * cos(angle);
		float y = radious_Face * sin(angle);



		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x_axis, y_axis, 0.0f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);    // left eye

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye1 * cos(angle) + (-0.2f);
		float y = radious_eye1 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();





	glBegin(GL_LINES);                    //right eye



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye2 * cos(angle) + (0.2f);
		float y = radious_eye2 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);                 // horizontal line for normal face.

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.2, -0.2, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2, -0.2, 0.0f);

	glEnd();

	glutPostRedisplay();
	glutSwapBuffers();
}

void Display_Sad_face(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(2.0f);

	glBegin(GL_LINES);                          //face

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Face * cos(angle);
		float y = radious_Face * sin(angle);



		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x_axis, y_axis, 0.0f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);    // left eye

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye1 * cos(angle) + (-0.2f);
		float y = radious_eye1 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();





	glBegin(GL_LINES);                    //right eye



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye2 * cos(angle) + (0.2f);
		float y = radious_eye2 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glBegin(GL_POINTS);     // sad face semicircle with points

	

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious_sad_face * cos((angle * M_PI) / 180.0f) + (0.0f);
		float y = radious_sad_face * sin((angle * M_PI) / 180.0f) + (-0.32f);

		if (angle <= 180.0f)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0, -0.20, 0.0f);


			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}



	}

	glEnd();

	glutPostRedisplay();
	glutSwapBuffers();
}

void Display_Curious_Face(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(2.0f);

	glBegin(GL_LINES);                          //face

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Face * cos(angle);
		float y = radious_Face * sin(angle);



		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x_axis, y_axis, 0.0f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);    // left eye

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye1 * cos(angle) + (-0.2f);
		float y = radious_eye1 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();





	glBegin(GL_LINES);                    //right eye



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye2 * cos(angle) + (0.2f);
		float y = radious_eye2 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);                    //round mouth



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_mouth * cos(angle) + (0.0f);
		float y = radious_mouth * sin(angle) + (-0.3f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -0.3f, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glutPostRedisplay();
	glutSwapBuffers();


}

void Extreme_Happy_Face(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(2.0f);

	glBegin(GL_LINES);                          //face

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_Face * cos(angle);
		float y = radious_Face * sin(angle);



		// Circle Origin

		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x_axis, y_axis, 0.0f);


		glColor3f(1.0f, 0.9f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);    // left eye

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye1 * cos(angle) + (-0.2f);
		float y = radious_eye1 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();





	glBegin(GL_LINES);                    //right eye



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_eye2 * cos(angle) + (0.2f);
		float y = radious_eye2 * sin(angle) + (0.2f);



		// Circle Origin

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.2, 0.2, 0.0f);


		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();

	glBegin(GL_LINES);               // wide smile

	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
	{

		float radianangle = angle * (M_PI * 180.0f);
		float x = radious_smile * cos((angle*M_PI)/180.0f) + (0.0f);
		float y = radious_smile * sin(angle*M_PI/180.0f) + (-0.2f);



		if (angle >= 180 && angle <= 360)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, -0.2f, 0.0f);


			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(x, y, 0.0f);
		}
		

		


	}

	glEnd();

	glutPostRedisplay();
	glutSwapBuffers();

}




