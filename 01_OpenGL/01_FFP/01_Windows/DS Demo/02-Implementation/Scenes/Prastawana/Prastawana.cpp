#include"Prastawana.h"


//texture variable
GLuint TEXTURE_HANUMAN;
GLuint TEXTURE_SITA_RING;
GLuint TEXTURE_RAM_SADHANA;
GLuint TEXTURE_WANERSENA;
GLuint TEXTURE_RAM_BAN;
GLuint TEXTURE_SETU_BUILDING;


BOOL bResult;

//Animation variables

//GLfloat hanunman_Right_Top_X = 1.0f;
//GLfloat hanunman_Right_Top_Z = 0.0f;
//
//GLfloat hanunman_Right_Bottom_X = 1.0f;
//GLfloat hanunman_Right_Bottom_Z = 0.0f;

//Blending variable

//GLfloat blend_Hanuman = 0.0f;
//GLfloat blend_Sita = -0.5f;
//GLfloat blend_Ram_Sadhana = -0.7f;
//GLfloat blend_Wanersena = -0.5f;
//GLfloat blend_Ram_Ban = -0.9f;
//GLfloat blend_Setu_Building = -0.7f;

GLfloat blend[6] = {
						0.0f,  //blending of hanuman image
						-0.5f, //blending of sita ring image
						-0.7f, //blending of Ram Sadhana image
						-0.5f, //blending of waner sena image
						-0.9f, //blending of ram ban image
						-0.7f  //blending of building ram setu image.
				   };

int initializePrastawanaScene(void)
{
	
	bResult = LoadGLPNGTexture(&TEXTURE_HANUMAN, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/LankaDahan.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Hanuman image texture failed !! Exiting now\n");
		return(-1);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_SITA_RING, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/Sita.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of sita image texture failed !! Exiting now\n");
		return(-2);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_RAM_SADHANA, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/RamSadhna2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of ram sadhana image texture failed !! Exiting now\n");
		return(-3);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_WANERSENA, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/WanerSena2.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of wanersena image texture failed !! Exiting now\n");
		return(-4);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_RAM_BAN, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/SagarBan.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Sagar Ban image texture failed !! Exiting now\n");
		return(-5);
	}

	bResult = LoadGLPNGTexture(&TEXTURE_SETU_BUILDING, "C:/MyProjects/RTR-5.0/01_OpenGL/01_FFP/01_Windows/DS Demo/02-Implementation/Assets/Texture/SetuBuild.png");

	if (bResult == FALSE)
	{
		fprintf(gpFile, "Loading of Setu building image texture failed !! Exiting now\n");
		return(-5);
	}





	
	return(TRUE);
	


	
}

void displayPrastawanaScene(void)
{
	


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBindTexture(GL_TEXTURE_2D, TEXTURE_HANUMAN);

	glColor4f(1.0f, 1.0f, 1.0f, blend[0]);
	
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	//glVertex4f(1.0f, 1.0f, 0.0f,1.0f);  //right top
	//glVertex3f(hanunman_Right_Top_X, 1.0f, hanunman_Right_Top_Z);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	//glVertex4f(-1.0f, 1.0f, 0.0f,1.0f);  //left top

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	//glVertex4f(-1.0f, -1.0f, 0.0f,1.0f);  //left bottom

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	//glVertex4f(1.0f, -1.0f, 0.0f,1.0f);  //right bottom
	/*glVertex3f(hanunman_Right_Bottom_X, -1.0f, hanunman_Right_Bottom_Z);*/

	

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	//Sita Ring Image

	if (ELAPSED_TIME > SITA_RING_IMAGE_STARTS)
	{
		glBindTexture(GL_TEXTURE_2D, TEXTURE_SITA_RING);

		//glColor4f(1.0f, 1.0f, 1.0f, blend_Sita);
		glColor4f(1.0f, 1.0f, 1.0f, blend[1]);
		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		//glVertex4f(1.0f, 1.0f, 0.0f,1.0f);  //right top
		

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		//glVertex4f(-1.0f, 1.0f, 0.0f,1.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		//glVertex4f(-1.0f, -1.0f, 0.0f,1.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		//glVertex4f(1.0f, -1.0f, 0.0f,1.0f);  //right bottom
		



		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}



	//Ram and wanersena thinking about the bridge

	if (ELAPSED_TIME > WANERSENA_IMAGE_STARTS)
	{
		glBindTexture(GL_TEXTURE_2D, TEXTURE_WANERSENA);

		//glColor4f(1.0f, 1.0f, 1.0f, blend_Wanersena);

		glColor4f(1.0f, 1.0f, 1.0f, blend[3]);

		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		//glVertex4f(1.0f, 1.0f, 0.0f,1.0f);  //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		//glVertex4f(-1.0f, 1.0f, 0.0f,1.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		//glVertex4f(-1.0f, -1.0f, 0.0f,1.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		//glVertex4f(1.0f, -1.0f, 0.0f,1.0f);  //right bottom




		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}





	// Ram Sadhana Image

	if (ELAPSED_TIME > RAM_SADHANA_IMAGE_STARTS)
	{
		glBindTexture(GL_TEXTURE_2D, TEXTURE_RAM_SADHANA);

		//glColor4f(1.0f, 1.0f, 1.0f, blend_Ram_Sadhana);

		glColor4f(1.0f, 1.0f, 1.0f, blend[2]);

		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		//glVertex4f(1.0f, 1.0f, 0.0f,1.0f);  //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		//glVertex4f(-1.0f, 1.0f, 0.0f,1.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		//glVertex4f(-1.0f, -1.0f, 0.0f,1.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		//glVertex4f(1.0f, -1.0f, 0.0f,1.0f);  //right bottom




		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//ram ban image

	if (ELAPSED_TIME > RAM_BAN_IMAGE_STARTS)
	{
		glBindTexture(GL_TEXTURE_2D, TEXTURE_RAM_BAN);

		//glColor4f(1.0f, 1.0f, 1.0f, blend_Ram_Ban);

		glColor4f(1.0f, 1.0f, 1.0f, blend[4]);

		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		//glVertex4f(1.0f, 1.0f, 0.0f,1.0f);  //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		//glVertex4f(-1.0f, 1.0f, 0.0f,1.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		//glVertex4f(-1.0f, -1.0f, 0.0f,1.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		//glVertex4f(1.0f, -1.0f, 0.0f,1.0f);  //right bottom




		glEnd();

		
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Setu Building image

	if (ELAPSED_TIME > SETU_BUILDING_IMAGE_STARTS)
	{
		glBindTexture(GL_TEXTURE_2D, TEXTURE_SETU_BUILDING);

		glColor4f(1.0f, 1.0f, 1.0f, blend[5]);

		glBegin(GL_QUADS);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		//glVertex4f(1.0f, 1.0f, 0.0f,1.0f);  //right top


		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		//glVertex4f(-1.0f, 1.0f, 0.0f,1.0f);  //left top

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		//glVertex4f(-1.0f, -1.0f, 0.0f,1.0f);  //left bottom

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		//glVertex4f(1.0f, -1.0f, 0.0f,1.0f);  //right bottom




		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

	}


	glDisable(GL_BLEND);

}

void updatePrastawanaScene(void)
{
	//blending of hanuman image

	if (ELAPSED_TIME > TITLE_SCENE_END && blend[0] < 1.0f)
	{
		blend[0] = blend[0] + 0.001;
	}

	//blending of sita image

	if (ELAPSED_TIME > SITA_RING_IMAGE_STARTS && blend[0] >= 1.0f && blend[1] < 1.0f)
	{
																		
		blend[1] = blend[1] + 0.0020f;
		
	}

	// image blending of wanersena

	if (ELAPSED_TIME > WANERSENA_IMAGE_STARTS && blend[1] >= 1.0f && blend[3] < 1.0f)
	{
		blend[3] = blend[3] + 0.0018f;
	}

	//blending of ram sadhana image

	if (ELAPSED_TIME > RAM_SADHANA_IMAGE_STARTS && blend[3] >= 1.0f && blend[2] < 1.0f)
	{
		blend[2] = blend[2] + 0.0018f;
	}

	//blending of ram ban image

	if (ELAPSED_TIME > RAM_BAN_IMAGE_STARTS && blend[2] >= 1.0f && blend[4] < 1.0f)
	{
		blend[4] = blend[4] + 0.0018f;
	}

	//blending of Setu Building image

	if (ELAPSED_TIME > SETU_BUILDING_IMAGE_STARTS && blend[4] >= 1.0f && blend[5] < 1.0f)
	{
		blend[5] = blend[5] + 0.0018f;
	}


	
	
}

void uninitializePrastawanaScene(void)
{
	if (TEXTURE_HANUMAN)
	{
		glDeleteTextures(1, &TEXTURE_HANUMAN);

		TEXTURE_HANUMAN = 0;
	}

	if (TEXTURE_SITA_RING)
	{
		glDeleteTextures(1, &TEXTURE_SITA_RING);

		TEXTURE_SITA_RING = 0;
	}


	if (TEXTURE_RAM_SADHANA)
	{
		glDeleteTextures(1, &TEXTURE_RAM_SADHANA);

		TEXTURE_RAM_SADHANA = 0;
	}

	if (TEXTURE_WANERSENA)
	{
		glDeleteTextures(1, &TEXTURE_WANERSENA);

		TEXTURE_WANERSENA = 0;
	}

	if (TEXTURE_RAM_BAN)
	{
		glDeleteTextures(1, &TEXTURE_RAM_BAN);

		TEXTURE_RAM_BAN = 0;
	}

	if (TEXTURE_SETU_BUILDING)
	{
		glDeleteTextures(1, &TEXTURE_SETU_BUILDING);

		TEXTURE_SETU_BUILDING = 0;
	}

	
}




