// Header files 
#include <GL/freeglut.h>
#include<math.h>

// Global variable declarations
bool bIsFullScreen = false;
float radious = 0.1f;
float x_axis = 0.0f;

// arrays created for animation

float vertex_lefthand_elbow_upper_point[] = { -0.35f, 0.254f, 0.0f };
float vertex_lefthand_elbow_bottom_point[] = { -0.24f, 0.19f, 0.0f };
float vertex_lefthand_wrist_left_point[] = { -0.34f, -0.035f, 0.0f };
float vertex_lefthand_wrist_right_point[] = { -0.26f, -0.035f, 0.0f };
float vertex_lefthand_shoulder_top_point[] = { -0.14f, 0.42f, 0.0f };
float vertex_lefthand_shoulder_bottom_point[] = { -0.14f, 0.25f, 0.0f };




float vertex_righthand_elbow_upper_point[] = { 0.19f, 0.174f, 0.0f };
float vertex_righthand_elbow_lower_point[] = { 0.15f, 0.035f, 0.0f };
float vertex_righthand_wrist_upper_point[] = { 0.32f, 0.214f, 0.0f };
float vertex_righthand_wrist_lower_point[] = { 0.37f, 0.135f, 0.0f };
float vertex_righthand_shoulder_bottom_point[] = { 0.07f, 0.13f, 0.0f };
float vertex_righthand_shoulder_top_point[] = { 0.07f, 0.42f, 0.0f };




float vertex_core_body_top_right_point[] = { 0.07f, 0.42f, 0.0f };
float vertex_core_body_top_left_point[] = { -0.14f, 0.42f, 0.0f };
float vertex_core_body_bottom_right_point[] = { 0.07f, -0.12f, 0.0f };
float vertex_core_body_bottom_left_point[] = { -0.14f, -0.12f, 0.0f };



float vertex_right_leg_Knee_right_point[] = { 0.25f, -0.355f, 0.0f };
float vertex_right_leg_Knee_left_point[] = { 0.14f, -0.41f, 0.0f };
float vertex_right_leg_ankle_left_point[] = { 0.204f, -0.71f, 0.0f };
float vertex_right_leg_ankle_right_point[] = { 0.3f, -0.7f, 0.0f };


float vertex_right_leg_waist_right[] = { 0.07f, -0.12f, 0.0f };
float vertex_right_leg_waist_left[] = { -0.14f, -0.12f, 0.0f };
float vertex_right_leg_knee_right[] = { 0.25f, -0.355f, 0.0f };
float vertex_right_leg_knee_left[] = { 0.14f, -0.41f, 0.0f };




float vertex_left_leg_Knee_left_point[] = { -0.17f, -0.405f, 0.0f };
float vertex_left_leg_Knee_right_point[] = { -0.054f, -0.434f, 0.0f };
float vertex_left_leg_ankle_left_point[] = { -0.3f, -0.665f, 0.0f };
float vertex_left_leg_ankle_right_point[] = { -0.2f, -0.71f, 0.0f };


float vertex_left_leg_waist_right_point[] = { 0.07f, -0.12f, 0.0f };
float vertex_left_leg_waist_left_point[] = { -0.14f, -0.12f, 0.0f };










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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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



	glBegin(GL_QUADS);          // core body ( middle square )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_core_body_top_right_point[0], vertex_core_body_top_right_point[1], vertex_core_body_top_right_point[2]);  //top right

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_core_body_top_left_point[0], vertex_core_body_top_left_point[1], vertex_core_body_top_left_point[2]); // top left

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_core_body_bottom_left_point[0], vertex_core_body_bottom_left_point[1], vertex_core_body_bottom_left_point[2]); // bottom left

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_core_body_bottom_right_point[0], vertex_core_body_bottom_right_point[1], vertex_core_body_bottom_right_point[2]);  // bottom right

	glEnd();




	glBegin(GL_QUADS);  // right arm ( shoulder part )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_elbow_upper_point[0], vertex_righthand_elbow_upper_point[1], vertex_righthand_elbow_upper_point[2]);  // right arm elbow upper point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_shoulder_top_point[0], vertex_righthand_shoulder_top_point[1], vertex_righthand_shoulder_top_point[2]); //right arm shoulder top

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_shoulder_bottom_point[0], vertex_righthand_shoulder_bottom_point[1], vertex_righthand_shoulder_bottom_point[2]);  // right arm shoulder bottom

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_elbow_lower_point[0], vertex_righthand_elbow_lower_point[1], vertex_righthand_elbow_lower_point[2]);//right arm elbow lower point

	glEnd();



	glBegin(GL_QUADS);  //right arm ( forehand part )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_wrist_upper_point[0], vertex_righthand_wrist_upper_point[1], vertex_righthand_wrist_upper_point[2]); // wrist upper point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_elbow_upper_point[0], vertex_righthand_elbow_upper_point[1], vertex_righthand_elbow_upper_point[2]); // elbow upper point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_elbow_lower_point[0], vertex_righthand_elbow_lower_point[1], vertex_righthand_elbow_lower_point[2]);  // elbow lower point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_righthand_wrist_lower_point[0], vertex_righthand_wrist_lower_point[1], vertex_righthand_wrist_lower_point[2]); // wrist lower point



	glEnd();



	glBegin(GL_QUADS);  // left arm ( shoulder part )


	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_elbow_upper_point[0], vertex_lefthand_elbow_upper_point[1], vertex_lefthand_elbow_upper_point[2]);   //  left hand elbow upper co-ordinate

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_elbow_bottom_point[0], vertex_lefthand_elbow_bottom_point[1], vertex_lefthand_elbow_bottom_point[2]);  //  left hand elbow bottom co-ordinate

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_shoulder_bottom_point[0], vertex_lefthand_shoulder_bottom_point[1], vertex_lefthand_shoulder_bottom_point[2]); // left hand shoulder bottom point


	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_shoulder_top_point[0], vertex_lefthand_shoulder_top_point[1], vertex_lefthand_shoulder_top_point[2]); // left hand shoulder top point

	 

	
	glEnd();
	 
	glBegin(GL_QUADS);  //  left arm ( forehand )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_wrist_right_point[0], vertex_lefthand_wrist_right_point[1], vertex_lefthand_wrist_right_point[2]);
	
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_elbow_bottom_point[0], vertex_lefthand_elbow_bottom_point[1], vertex_lefthand_elbow_bottom_point[2]);  // elbow right co-ordinate


	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_elbow_upper_point[0], vertex_lefthand_elbow_upper_point[1], vertex_lefthand_elbow_upper_point[2]);  // elbow left co-ordinate


	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_lefthand_wrist_left_point[0], vertex_lefthand_wrist_left_point[1], vertex_lefthand_wrist_left_point[2]);

	



	

	glEnd();



	

	glBegin(GL_QUADS); // right leg ( thigh )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_waist_right[0], vertex_right_leg_waist_right[1], vertex_right_leg_waist_right[2]);  // waist right point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_waist_left[0], vertex_right_leg_waist_left[1], vertex_right_leg_waist_left[2]);  //waist left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_knee_left[0], vertex_right_leg_knee_left[1], vertex_right_leg_knee_left[2]); // knee left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_knee_right[0], vertex_right_leg_knee_right[1], vertex_right_leg_knee_right[2]); // knee right point





	glEnd();



	glBegin(GL_QUADS);  // right leg ( bottom part )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_knee_right[0], vertex_right_leg_knee_right[1], vertex_right_leg_knee_right[2]);  // knee right point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_knee_left[0], vertex_right_leg_knee_left[1], vertex_right_leg_knee_left[2]);  // knee left point 

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_ankle_left_point[0], vertex_right_leg_ankle_left_point[1], vertex_right_leg_ankle_left_point[2]); // ankle left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_right_leg_ankle_right_point[0], vertex_right_leg_ankle_right_point[1], vertex_right_leg_ankle_right_point[2]); //ankle right point

	glEnd();



	glBegin(GL_QUADS);   // left leg ( thigh )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_waist_right_point[0], vertex_left_leg_waist_right_point[1], vertex_left_leg_waist_right_point[2]); // waist right point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_waist_left_point[0], vertex_left_leg_waist_left_point[1] , vertex_left_leg_waist_left_point[2]);  // waist left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_Knee_left_point[0], vertex_left_leg_Knee_left_point[1], vertex_left_leg_Knee_left_point[2]);  // knee left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_Knee_right_point[0], vertex_left_leg_Knee_right_point[1], vertex_left_leg_Knee_right_point[2]); //knee right point



	glEnd();



	glBegin(GL_QUADS);   // left leg ( bottom )

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_Knee_right_point[0], vertex_left_leg_Knee_right_point[1], vertex_left_leg_Knee_right_point[2]);  // knee right point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_Knee_left_point[0], vertex_left_leg_Knee_left_point[1], vertex_left_leg_Knee_left_point[2]);  // knee left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_ankle_left_point[0], vertex_left_leg_ankle_left_point[1], vertex_left_leg_ankle_left_point[2]); // ankle left point

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(vertex_left_leg_ankle_right_point[0], vertex_left_leg_ankle_right_point[1], vertex_left_leg_ankle_right_point[2]); //ankle right point



	glEnd();



	glBegin(GL_LINES);    // face



	for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)  
	{

		//float radianangle = angle * (M_PI * 180.0f);
		float x = radious * cos(angle) + (-0.024);
		float y = radious * sin(angle) + (0.55);


		
		x = x + x_axis;
		
		
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x_axis, 0.55, 0.0f);  // centre of face

		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);


	}

	glEnd();


//  ************* Animation logic starts below *********************

	//core body movement start

	if (vertex_core_body_top_right_point[0] <= 0.6f)
	{
		vertex_core_body_top_right_point[0] = vertex_core_body_top_right_point[0] + 0.0001;
	}

	if (vertex_core_body_top_left_point[0] <= 0.39f)
	{
		
		vertex_core_body_top_left_point[0] = vertex_core_body_top_left_point[0] + 0.0001;
		
	}

	if (vertex_core_body_bottom_left_point[0] <= 0.39f)
	{
		vertex_core_body_bottom_left_point[0] = vertex_core_body_bottom_left_point[0] + 0.0001;
	}

	if (vertex_core_body_bottom_right_point[0] <= 0.6f)
	{
		vertex_core_body_bottom_right_point[0] = vertex_core_body_bottom_right_point[0] + 0.0001;
	}

	//core body movement end

	// right hand shoulder to elbow movement start

	if (vertex_righthand_shoulder_top_point[0] <= 0.6f)
	{
		vertex_righthand_shoulder_top_point[0] = vertex_righthand_shoulder_top_point[0] + 0.0001;
	}

	if (vertex_righthand_shoulder_bottom_point[0] <= 0.6f)
	{
		vertex_righthand_shoulder_bottom_point[0] = vertex_righthand_shoulder_bottom_point[0] + 0.0001;
	}

	if (vertex_righthand_elbow_upper_point[0] <= 0.72f)
	{
		vertex_righthand_elbow_upper_point[0] = vertex_righthand_elbow_upper_point[0] + 0.0001;
	}

	if (vertex_righthand_elbow_lower_point[0] <= 0.68f)
	{
		vertex_righthand_elbow_lower_point[0] = vertex_righthand_elbow_lower_point[0] + 0.0001;
	}

	// right hand shoulder to elbow movement end

	//right hand elbow to wrist movement start

	if (vertex_righthand_wrist_upper_point[0] <= 0.85f)
	{
		vertex_righthand_wrist_upper_point[0] = vertex_righthand_wrist_upper_point[0] + 0.0001;
	}

	if (vertex_righthand_wrist_lower_point[0] <= 0.9f)
	{
		vertex_righthand_wrist_lower_point[0] = vertex_righthand_wrist_lower_point[0] + 0.0001;
	}

	//right hand elbow to wrist movement end
	
	//left hand shoulder to elbow movement start

	if (vertex_lefthand_shoulder_top_point[0] <= 0.39)
	{
		vertex_lefthand_shoulder_top_point[0] = vertex_lefthand_shoulder_top_point[0] + 0.0001;
	}

	if (vertex_lefthand_shoulder_bottom_point[0] <= 0.39)
	{
		vertex_lefthand_shoulder_bottom_point[0] = vertex_lefthand_shoulder_bottom_point[0] + 0.0001;
	}

	if (vertex_lefthand_elbow_upper_point[0] < 0.18)
	{
		vertex_lefthand_elbow_upper_point[0] = vertex_lefthand_elbow_upper_point[0] + 0.0001;


	}

	if (vertex_lefthand_elbow_bottom_point[0] <= 0.29)
	{
		vertex_lefthand_elbow_bottom_point[0] = vertex_lefthand_elbow_bottom_point[0] + 0.0001;
	}

	//left hand shoulder to elbow movement end

	//left hand elbow to wrist movement start

	if (vertex_lefthand_wrist_right_point[0] <= 0.27f)
	{
		vertex_lefthand_wrist_right_point[0] = vertex_lefthand_wrist_right_point[0] + 0.0001;
	}

	if (vertex_lefthand_wrist_left_point[0] <= 0.19f)
	{
		vertex_lefthand_wrist_left_point[0] = vertex_lefthand_wrist_left_point[0] + 0.0001;
	}

	//left hand elbow to wrist movement end

	// right leg movement start

	//right leg thigh leg movement start

	if (vertex_right_leg_waist_right[0] <= 0.6f)
	{
		vertex_right_leg_waist_right[0] = vertex_right_leg_waist_right[0] + 0.0001;
	}

	if (vertex_right_leg_waist_left[0] <= 0.39f)
	{
		vertex_right_leg_waist_left[0] = vertex_right_leg_waist_left[0] + 0.0001;
	}

	if (vertex_right_leg_knee_right[0] <= 0.78f)
	{
		vertex_right_leg_knee_right[0] = vertex_right_leg_knee_right[0] + 0.0001;
	}

	if (vertex_right_leg_knee_left[0] <= 0.67f)
	{
		vertex_right_leg_knee_left[0] = vertex_right_leg_knee_left[0] + 0.0001;
	}

	//right leg thigh movement end

	//right leg knee to ankle movement start

	if (vertex_right_leg_ankle_left_point[0] <= 0.734f)
	{
		vertex_right_leg_ankle_left_point[0] = vertex_right_leg_ankle_left_point[0] + 0.0001;
	}

	if (vertex_right_leg_ankle_right_point[0] <= 0.83f)
	{
		vertex_right_leg_ankle_right_point[0] = vertex_right_leg_ankle_right_point[0] + 0.0001;
	}


	//right leg knee to ankle movement end

	//right leg movement end

	//left leg movement start

	// left leg thigh movement start

	if (vertex_left_leg_waist_right_point[0] <= 0.6f)
	{
		vertex_left_leg_waist_right_point[0] = vertex_left_leg_waist_right_point[0] + 0.0001;
	}

	if (vertex_left_leg_waist_left_point[0] <= 0.39f)
	{
		vertex_left_leg_waist_left_point[0] = vertex_left_leg_waist_left_point[0] + 0.0001;
	}

	if (vertex_left_leg_Knee_left_point[0] <= 0.36f)
	{
		vertex_left_leg_Knee_left_point[0] = vertex_left_leg_Knee_left_point[0] + 0.0001;
	}

	if (vertex_left_leg_Knee_right_point[0] <= 0.466f)  
	{
		vertex_left_leg_Knee_right_point[0] = vertex_left_leg_Knee_right_point[0] + 0.0001;
	}

	




	// left leg thigh movement end

	//left leg thigh to ankle movement start


	if (vertex_left_leg_ankle_left_point[0] < 0.22f)
	{
		vertex_left_leg_ankle_left_point[0] = vertex_left_leg_ankle_left_point[0] + 0.0001;
	}

	if (vertex_left_leg_ankle_right_point[0] < 0.32f)
	{
		vertex_left_leg_ankle_right_point[0] = vertex_left_leg_ankle_right_point[0] + 0.0001;
	}





	// left leg thigh to ankle movement end
	//left leg movement end


	// face movement start

	if (x_axis <= 0.532)
	{
		x_axis = x_axis + 0.0001;
	}

	//face movement end 

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





