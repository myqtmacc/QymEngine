#pragma once

#include <string>
#include <GL/glew.h>

namespace QymEngine {

	class QymShaderFailedException : std::exception
	{
	public:
		QymShaderFailedException() = delete;
		QymShaderFailedException(std::string &msg) : errMsg(msg){}
	private:
		std::string errMsg;
	};

	class QymShaderProgram {

		friend class QymMeshRenderer;

	public:
		QymShaderProgram() = delete;
		QymShaderProgram(const QymShaderProgram &) = delete;
		QymShaderProgram(const char * vertexStr, const char * fragmentSrc);
		~QymShaderProgram();

		unsigned int GetProgramID() const { return this->m_uProgram; }

		int GetUniformLocation(const char * uniformName);

	private:

		// These will always be > 0 after a build, any errors will abort()
		unsigned int m_uProgram;
		unsigned int m_uVertexShader;
		unsigned int m_uFragmentShader;

		// Uniforms that aren't found will have a -1 value
		int		m_uMvp;				// uniform Mvpm
		int		m_uModel;				// uniform Modelm
		int		m_uView;				// uniform Viewm
		int		m_uProjection;		// uniform Projectionm
		int		m_uColor;				// uniform UniformColor
		int		m_uTexm;				// uniform Texm
		int		m_uTexm2;				// uniform Texm2

		bool _compileShader(GLenum shaderType, const GLuint shader, const char * src, std::string &err);
		bool _buildProgram(const char * vertexSrc, const char * fragmentSrc, std::string &err);
		void _deleteProgram();
	};
}