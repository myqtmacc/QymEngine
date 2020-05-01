#pragma once

#include "windows.h"

#include "vmath.h"
#include <memory>
#include <string>
#include "GL/glew.h"

using namespace vmath;

namespace QymEngine {

	class QymTexture
	{
	public:
		QymTexture();
		QymTexture(const QymTexture &) = delete;
		QymTexture & operator = (const QymTexture &) = delete;
		QymTexture(GLuint texID, mat4 mat = mat4::identity());
		QymTexture(GLint width, GLint height, const GLubyte * buffer = nullptr, GLenum format = GL_RGBA, mat4 mat = mat4::identity());
		virtual ~QymTexture();

		GLint GetWidth() const { return  this->m_iWidth; }
		GLint GetHeight() const { return this->m_iHeight; }

		void SetFilterMode(GLfloat mode);
		void UpdateTexture(const GLubyte * buffer, GLint width, GLint height, GLenum format);

		virtual GLuint GetTexID() { return this->m_iTex; }

		mat4 GetTexm() const { return this->m_Texm; }
		void SetTexm(const mat4 & mat) { this->m_Texm = mat; }

		static void SaveTextureImage(GLuint tex, const wchar_t *filename, GLenum textureType);
		static int SaveImage(int widthImg, int heightImg, BYTE * pBuffer, const wchar_t * filePath);
		static std::shared_ptr<QymTexture> LoadTexture(const std::string & path);

	protected:
		GLuint m_iTex;
		GLint m_iWidth;
		GLint m_iHeight;

		bool m_bExternal;

		bool m_bIsValid;

		mat4 m_Texm;

		void _initTexture(GLint width, GLint height, GLenum format);
		void _destroyTexture();
	};
}