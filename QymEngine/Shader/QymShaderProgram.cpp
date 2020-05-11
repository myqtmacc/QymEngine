#include "Shader/QymShaderProgram.h"
#include "Render/QymRenderCommon.h"
#include "Utils/pathtools.h"
#include "Utils/IOTools.h"

using namespace QymEngine;

QymShaderProgram::QymShaderProgram() :
	m_uProgram(0),
	m_uVertexShader(0),
	m_uFragmentShader(0),
	m_uMvp(-1),
	m_uModel(-1),
	m_uView(-1),
	m_uProjection(-1),
	m_uColor(-1),
	m_uTexm(-1),
	m_uTexm2(-1)
{
}

QymShaderProgram::QymShaderProgram(const char * vertexStr, const char * fragmentSrc) :
	QymShaderProgram()
{
	std::string errorMsg;
	if (!this->_buildProgram(vertexStr, fragmentSrc, errorMsg)) {
		throw QymShaderFailedException(errorMsg);
	}
}

QymShaderProgram::QymShaderProgram(const std::string & vs_path, const std::string & fs_path, int) :
	QymShaderProgram()
{
	std::string vertexStr = LoadShaderFromFile(vs_path);
	std::string fragmentSrc = LoadShaderFromFile(fs_path);

	std::string errorMsg;
	if (!this->_buildProgram(vertexStr.c_str(), fragmentSrc.c_str(), errorMsg)) {
		throw QymShaderFailedException(errorMsg);
	}
}

QymShaderProgram::~QymShaderProgram() {
	this->_deleteProgram();
}

std::string QymShaderProgram::LoadShaderFromFile(const std::string & path)
{
	std::string fullPath = Path_Join(Path_StripFilename(Path_GetExecutablePath()), RESOURCE_SHADER_PATH, path);

	std::ifstream t(fullPath);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return str;
}

bool QymShaderProgram::_compileShader(GLenum shaderType, const GLuint shader, const char * src, std::string &err)
{
	const int numSources = 1;
	const char * srcs[1];
	srcs[0] = src;

	glShaderSource(shader, numSources, srcs, 0);
	glCompileShader(shader);

	GLint r;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &r);
	if (r == GL_FALSE)
	{
		GLchar msg[4096];
		glGetShaderInfoLog(shader, sizeof(msg), 0, msg);
		err = msg;
		return false;
	}
	return true;
}

bool QymShaderProgram::_buildProgram(const char * vertexSrc, const char * fragmentSrc, std::string &err) {
	this->m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (!this->_compileShader(GL_VERTEX_SHADER, this->m_uVertexShader, vertexSrc, err))
	{
		this->_deleteProgram();
		return false;
	}
	this->m_uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!this->_compileShader(GL_FRAGMENT_SHADER, this->m_uFragmentShader, fragmentSrc, err))
	{
		this->_deleteProgram();
		return false;
	}

	this->m_uProgram = glCreateProgram();
	glAttachShader(this->m_uProgram, this->m_uVertexShader);
	glAttachShader(this->m_uProgram, this->m_uFragmentShader);

	// set attributes before linking
	glBindAttribLocation(this->m_uProgram, VERTEX_ATTRIBUTE_LOCATION_POSITION, "Position");
	glBindAttribLocation(this->m_uProgram, VERTEX_ATTRIBUTE_LOCATION_COLOR, "VertexColor");
	glBindAttribLocation(this->m_uProgram, VERTEX_ATTRIBUTE_LOCATION_UV0, "TexCoord");
	glBindAttribLocation(this->m_uProgram, VERTEX_ATTRIBUTE_LOCATION_UV1, "TexCoord1");

	// link and error check
	glLinkProgram(this->m_uProgram);
	GLint r;
	glGetProgramiv(this->m_uProgram, GL_LINK_STATUS, &r);
	if (r == GL_FALSE)
	{
		GLchar msg[1024];
		glGetProgramInfoLog(this->m_uProgram, sizeof(msg), 0, msg);
		err = msg;
		this->_deleteProgram();
		return false;
	}
	this->m_uMvp = glGetUniformLocation(this->m_uProgram, "Mvpm");
	this->m_uModel = glGetUniformLocation(this->m_uProgram, "Modelm");
	this->m_uView = glGetUniformLocation(this->m_uProgram, "Viewm");
	this->m_uProjection = glGetUniformLocation(this->m_uProgram, "Projectionm");
	this->m_uColor = glGetUniformLocation(this->m_uProgram, "UniformColor");
	this->m_uTexm = glGetUniformLocation(this->m_uProgram, "Texm");
	this->m_uTexm2 = glGetUniformLocation(this->m_uProgram, "Texm2");

	glUseProgram(this->m_uProgram);

	// texture and image_external bindings
	for (int i = 0; i < 16; i++)
	{
		char name[32];
		sprintf_s(name, "Texture%i", i);
		const GLint uTex = glGetUniformLocation(this->m_uProgram, name);
		if (uTex != -1)
		{
			glUniform1i(uTex, i);
		}
	}

	glUseProgram(0);

	return true;
}

void QymShaderProgram::_deleteProgram() {
	glUseProgram(0);
	if (glIsProgram(this->m_uProgram)) {
		glDeleteProgram(this->m_uProgram);
		this->m_uProgram = 0;
	}
	if (glIsShader(this->m_uVertexShader)) {
		glDeleteShader(this->m_uVertexShader);
		this->m_uVertexShader = 0;
	}
	if (glIsShader(this->m_uFragmentShader)) {
		glDeleteShader(this->m_uFragmentShader);
		this->m_uFragmentShader = 0;
	}

	m_uMvp = -1;
	m_uModel = -1;
	m_uView = -1;
	m_uProjection = -1;
	m_uColor = -1;
	m_uTexm = -1;
	m_uTexm2 = -1;
}

int QymShaderProgram::GetUniformLocation(const char * uniformName) {
	if (!glIsProgram(this->m_uProgram)) {
		return -1;
	}

	return glGetUniformLocation(this->m_uProgram, uniformName);
}