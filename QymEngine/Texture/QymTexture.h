#pragma once

#include <memory>
#include <string>

#include "Math/QymMath.h"

namespace QymEngine {

	class DLL_EXPORT QymTexture
	{
	public:
		QymTexture();
		QymTexture(const QymTexture &) = delete;
		QymTexture & operator = (const QymTexture &) = delete;
		QymTexture(GLuint texID, Math::Matrix4x4f mat = Math::Identity<Math::Matrix4x4f>());
		QymTexture(GLint width, GLint height, const GLubyte * buffer = nullptr, GLenum format = GL_RGBA, Math::Matrix4x4f mat = Math::Identity<Math::Matrix4x4f>());
		virtual ~QymTexture();

		GLint GetWidth() const { return  this->m_iWidth; }
		GLint GetHeight() const { return this->m_iHeight; }

		void SetFilterMode(GLfloat mode);
		void UpdateTexture(const GLubyte * buffer, GLint width, GLint height, GLenum format);

		virtual GLuint GetTexID() { return this->m_iTex; }

		Math::Matrix4x4f GetTexm() const { return this->m_Texm; }
		void SetTexm(const Math::Matrix4x4f & mat) { this->m_Texm = mat; }

		static void SaveTextureImage(GLuint tex, const wchar_t *filename, GLenum textureType);
		static int SaveImage(int widthImg, int heightImg, BYTE * pBuffer, const wchar_t * filePath);
		static std::shared_ptr<QymTexture> LoadTexture(const std::string & path);

	protected:
		GLuint m_iTex;
		GLint m_iWidth;
		GLint m_iHeight;

		bool m_bExternal;

		bool m_bIsValid;

		Math::Matrix4x4f m_Texm;

		void _initTexture(GLint width, GLint height, GLenum format);
		void _destroyTexture();
	};
}