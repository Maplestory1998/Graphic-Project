#include <iostream>
#include "texture.h"
#include "bitmap.h"

std::vector<GLuint> setup_texture(std::vector<std::string>& filename)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	int n = filename.size();
	std::vector<GLuint> texObject(n);

	glGenTextures(n, &texObject[0]);
	for (int i = 0; i < n; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, texObject[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		unsigned char* pxls = NULL;
		BITMAPINFOHEADER info;
		BITMAPFILEHEADER file;
		loadbitmap(filename[i].c_str(), pxls, &info, &file);

		if (pxls != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.biWidth, info.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		delete[]pxls;

	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}

GLuint setup_mipmaps(const char* filename[], int n)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	unsigned char* pxls[16];
	BITMAPINFOHEADER info[16];
	BITMAPFILEHEADER file[16];
	for (int c = 0; c < n; c++)
	{
		loadbitmap(filename[c], pxls[c], &info[c], &file[c]);
		if (pxls != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, c, GL_RGB, info[c].biWidth, info[c].biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[c]);
		}
		delete[] pxls[c];
	}



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}