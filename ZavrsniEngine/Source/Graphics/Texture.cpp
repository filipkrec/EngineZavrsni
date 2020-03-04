#include "Texture.h"

namespace graphics {
	Texture::Texture(const std::string& filename, const bool& repeat)
	{
		if (load(filename.c_str(), repeat) == -1)
		{
			_textureId = 0;
		}
	}

	Texture::Texture(Font* font)
		: _textureId(font->getId()) 
	{}

	Texture::Texture(unsigned int textureId)
		: _textureId(textureId)
	{}


	Texture::~Texture()
	{
	}

	GLuint Texture::load(const char* filename, const bool& repeat)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = nullptr;
		BYTE* loadedPixels;
		BYTE* pixels; 

		GLsizei width;
		GLsizei height;

		fif = FreeImage_GetFileType(filename, 0); //free image format

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return -1;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return -1;

		loadedPixels = FreeImage_GetBits(dib);
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);

		int size = width * height * (bits / 8);
		pixels = new BYTE[size];
		memcpy(pixels, loadedPixels, size);
		FreeImage_Unload(dib);

		glGenTextures(1, &_textureId);

		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //omogucuje minificiranje (smanjenje kvalitete ukoliko je textura manja u prikazu od originala)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //omogucuje magnificiranje (povecanje kvalitete ukoliko je textura veca u prikazu od originala)
		if (repeat == false)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //onemogucuje ponavljanje ucitanih tekstura
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //onemogucuje ponavljanje ucitanih tekstura
		}
		if(bits == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels); //32 bitne slike imaju alpha kanal
		else if(bits == 24)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels); //24 bitne slike bez alpha kanala
		else
		{
			delete[] pixels;
			std::cout << "invalid image color depth" << std::endl;
			return -1;
		}


		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] pixels;
	}
}