#include "Windows.h"
#include <atlimage.h>
#include "Texture/QymTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace QymEngine;
using namespace QymEngine::Math;

QymTexture::QymTexture() :
m_iTex(0),
m_iWidth(0),
m_iHeight(0),
m_bExternal(false),
m_Texm(Identity<Matrix4x4f>()),
m_bIsValid(false)
{
	//this->_initTexture(512, 512, GL_RGBA);
}

QymTexture::QymTexture(GLuint texID, Matrix4x4f mat) :
m_iTex(texID),
m_iWidth(0),
m_iHeight(0),
m_bExternal(true),
m_Texm(mat),
m_bIsValid(false)
{
	if (glIsTexture(m_iTex)) {
		glBindTexture(GL_TEXTURE_2D, m_iTex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->m_iWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->m_iHeight);
		glBindTexture(GL_TEXTURE_2D, 0);

		this->m_bIsValid = true;
	}
	else {
		this->m_bIsValid = false;
	}
}

QymTexture::QymTexture(GLint width, GLint height, const GLubyte * buffer, GLenum format, Matrix4x4f mat) :
m_iTex(0),
m_iWidth(0),
m_iHeight(0),
m_bExternal(false),
m_Texm(mat),
m_bIsValid(false)
{
	this->_initTexture(width, height, format);
	this->UpdateTexture(buffer, width, height, format);
}

QymTexture::~QymTexture() {
	if (!this->m_bExternal) {
		this->_destroyTexture();
	}
}

void QymTexture::UpdateTexture(const GLubyte * buffer, GLint width, GLint height, GLenum format) {
	if (buffer == nullptr) {
		return;
	}

	if (this->m_iWidth != width || this->m_iHeight != height) {
		this->_initTexture(width, height, format);
	}

	glBindTexture(GL_TEXTURE_2D, this->m_iTex);

	glTexSubImage2D(GL_TEXTURE_2D, 0,
		0, 0, width, height,
		format, GL_UNSIGNED_BYTE, buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void QymTexture::SetFilterMode(GLfloat mode) {
	if (!glIsTexture(this->m_iTex)) {
		return;
	}

	glBindTexture(GL_TEXTURE_2D, this->m_iTex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
}

void QymTexture::_initTexture(GLint width, GLint height, GLenum format) {
	this->_destroyTexture();

	glGenTextures(1, &(this->m_iTex));

	glBindTexture(GL_TEXTURE_2D, this->m_iTex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->m_iWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->m_iHeight);

	glBindTexture(GL_TEXTURE_2D, 0);

	this->m_bIsValid = true;
}

void QymTexture::_destroyTexture() {
	if (glIsTexture(this->m_iTex)) {
		glDeleteTextures(1, &this->m_iTex);
		this->m_iTex = 0;
	}

	this->m_iWidth = 0;
	this->m_iHeight = 0;

	this->m_bIsValid = false;
}

void QymTexture::SaveTextureImage(GLuint tex, const wchar_t *filename, GLenum textureType)
{
	glEnable(textureType);
	glBindTexture(textureType, tex);

	// Get size of the desktop. Usually this is the same values as returned by GetSystemMetrics(SM_CXSCREEN)
	// and GetSystemMetrics(SM_CYSCREEN). In some cases it might differ, e.g. if a rotated desktop is used.
	int mw, mh;
	glGetTexLevelParameteriv(textureType, 0, GL_TEXTURE_WIDTH, (GLint*)&mw);
	glGetTexLevelParameteriv(textureType, 0, GL_TEXTURE_HEIGHT, (GLint*)&mh);
	
	GLubyte *pixelbuffer = new GLubyte[mw*mh * 4 * sizeof(GLubyte)];

	glGetTexImage(textureType, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, pixelbuffer);
	QymTexture::SaveImage(mw, mh, pixelbuffer, filename);

	if (pixelbuffer != nullptr) {
		delete[] pixelbuffer;
		pixelbuffer = nullptr;
	}

	glBindTexture(textureType, 0);

}

int QymTexture::SaveImage(int widthImg, int heightImg, BYTE * pBuffer, const wchar_t * filePath)
{
	CImage img;

	BOOL re = img.Create(widthImg, heightImg, 24);
	//int i, j;
	byte r, g, b;// , c, a;

	for (int y = 0; y< heightImg; y++)
	{
		int offset = y * widthImg;
		for (int x = 0; x < widthImg; ++x)
		{
			b = pBuffer[4 * (offset + x) + 0];
			g = pBuffer[4 * (offset + x) + 1];
			r = pBuffer[4 * (offset + x) + 2];
			//a=imageDataFe[4*(offset + x)+3];
			img.SetPixelRGB(x, y, r, g, b);

		}
	}

	int width = img.GetWidth();
	int height = img.GetHeight();
	HRESULT hr = img.Save(filePath);
	if (hr == S_OK)
	{
		return 0;
	}

	return -1;

}

std::shared_ptr<QymTexture> QymTexture::LoadTexture(const std::string& path)
{
	GLuint t_tex;
	glGenTextures(1, &t_tex);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
	else{
		GLubyte temp[4 * 4];
		memset(temp, 0, 4 * 4);
		temp[3] = 255;
		temp[7] = 255;
		temp[11] = 255;
		temp[15] = 255;
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(pixels);

	return std::make_shared<QymTexture>(t_tex);
}
