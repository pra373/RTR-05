#include<string>
#include"Common.h"

#define STB_IMAGE_IMPLEMENTATION
#include"../External/stb_image.h"




BOOL loadGLTexture(GLuint* Texture, TCHAR image_Resource_ID[])
{
	//Local variable declarations
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//work of OS to load image and extract data from image starts here
	//load the image - step5 in notebook
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), image_Resource_ID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap == NULL)
	{
		fprintf(gpFile, "Load image function failed !! Exiting now\n");
		return(FALSE);
	}

	//Get Image Data  - step 6
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	//work of OS to load image and extract data from image ends here
	// work of OPENGL starts
	//step 7 

	glGenTextures(1, Texture); // generate textures

	//step 8
	glBindTexture(GL_TEXTURE_2D, *Texture);

	//step 9

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//step 10

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	//step 11

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)bmp.bmBits);

	//step 12
	glBindTexture(GL_TEXTURE_2D, 0);

	//step 13

	DeleteObject(hBitmap);
	hBitmap = NULL;
	return(TRUE);

}

BOOL initializeAudio(void)
{
	return TRUE;
}
BOOL uninitializeAudio(void)
{
	return TRUE;
}

BOOL LoadGLPNGTexture(GLuint* texture, const char* image)
{
	//variable declarations

	int iWidth, iHeight, iComponent;
	unsigned char* data = stbi_load(image, &iWidth, &iHeight, &iComponent, 0);

	if (!data)
	{
		fprintf(gpFile, "LoadingPNGTexture failed for %s:", image);
		stbi_image_free(data);
	}

	else
	{
		fprintf(gpFile, "\n\n%d channels are present in my PNG image\n\n", iComponent);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (iComponent == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		else if (iComponent == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

		//Delete object

		stbi_image_free(data);
		return(TRUE);
	}

	return FALSE;
}


struct FontDataRequest {
  std::string fontName;
  std::promise<struct fontData*> p;
};

int CALLBACK EnumFontFamiliesProc(_In_ ENUMLOGFONT* lpelf,
	_In_ NEWTEXTMETRIC lpntm,
	_In_ DWORD fontType,
	_In_ LPARAM lParam)
{
	std::string fontName(reinterpret_cast<char*>(lpelf->elfFullName));
  struct FontDataRequest* fontDataRequest =
      reinterpret_cast<struct FontDataRequest*>(lParam);

	//fprintf(gpFile, "%s\n", fontName.c_str());
	//fflush(gpFile);

	if (fontName == fontDataRequest->fontName)
	{
		struct fontData* pfontData = new struct fontData;

		pfontData->fontName = fontName;
		pfontData->pitchAndFamily = lpelf->elfLogFont.lfPitchAndFamily;
		pfontData->clipPrecision = lpelf->elfLogFont.lfClipPrecision;
		pfontData->charSet = lpelf->elfLogFont.lfCharSet;
		pfontData->quality = lpelf->elfLogFont.lfQuality;
		pfontData->outPrecision = lpelf->elfLogFont.lfOutPrecision;

		fontDataRequest->p.set_value(pfontData);

		return(1);
	}

	return 1;
}


std::future<struct fontData*> enumerateFontFamilies(HDC hdc, const std::string& fontName)
{
  struct FontDataRequest* fontDataRequest = new FontDataRequest;
  fontDataRequest->fontName = fontName;
  auto f = fontDataRequest->p.get_future();

  EnumFontFamilies(hdc, nullptr, (FONTENUMPROC)EnumFontFamiliesProc,
                   reinterpret_cast<LPARAM>(fontDataRequest));

	return (f);
}

GLvoid BuildFontEx(HDC ghdc, struct fontData* pfontData)
{
	HFONT font1; // will hold window font ID
	//HFONT font2;

	pfontData->base = glGenLists(256); //storage for 256 
	//base2 = glGenLists(256);

	//create font

	font1 = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, pfontData->charSet, pfontData->outPrecision, pfontData->clipPrecision, pfontData->quality, pfontData->pitchAndFamily, pfontData->fontName.data());

	/*font2 = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_STROKE_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "Constantia");*/



	//select font object

	SelectObject(ghdc, font1);

	wglUseFontOutlines(ghdc, 0, 255, pfontData->base, 0.0f, 0.2f, WGL_FONT_POLYGONS, pfontData->gmf);
}

GLvoid glPrintEx(struct fontData* pfontData, const char* fmt, ...)
{
	float length = 0;
	char text[256];
	va_list ap; //for handeling variable arguments

	if (fmt == NULL)
	{
		return;
	}

	va_start(ap, fmt);  //initialize va_list to start the variable argument processing
	vsprintf(text, fmt, ap);  // formatting variable argument according to fmt

	va_end(ap);  //cleans the list

	for (unsigned int loop = 0; loop < (strlen(text)); loop++)
	{
		length += pfontData->gmf[text[loop]].gmfCellIncX;
	}



	glTranslatef(-length / 2.0f, 0.0f, 0.0f);



	glPushAttrib(GL_LIST_BIT);

	glListBase(pfontData->base);

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

	glPopAttrib();
}




