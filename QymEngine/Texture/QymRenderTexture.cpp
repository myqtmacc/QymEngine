#include "Texture/QymRenderTexture.h"

using namespace QymEngine;

QymRenderTexture::QymRenderTexture(GLint w, GLint h, GLint depthCount) :
QymTexture(w, h),
m_iFBO(0),
m_iRBO(0),
m_bUseMRT(false),
m_iDepthCount(depthCount),
m_iMRTCount(0),
m_bIsValid(false)
{
	this->_initRT();
}

QymRenderTexture::QymRenderTexture(GLint w, GLint h, GLint depthCount, int MRTs) :
QymTexture(w, h),
m_iFBO(0),
m_iRBO(0),
m_bUseMRT(true),
m_iDepthCount(depthCount),
m_iMRTCount(MRTs),
m_bIsValid(false)
{
	GLint maxMRT = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxMRT);
	if (this->m_bUseMRT && this->m_iMRTCount > maxMRT) {
		this->_deleteRT();
		throw QymMRTUnsupportedException(std::string("MRT num too small"), maxMRT);
	}

	this->_initRT();
}

QymRenderTexture::~QymRenderTexture() {
	this->_deleteRT();
}

GLenum QymRenderTexture::_getDepthComponent() const {
	GLenum depth_conponent = GL_DEPTH_COMPONENT24;
	if (this->m_iDepthCount == 16) {
		depth_conponent = GL_DEPTH_COMPONENT16;
	}
	else if (this->m_iDepthCount == 24) {
		depth_conponent = GL_DEPTH_COMPONENT24;
	}
	else if (this->m_iDepthCount == 32) {
		depth_conponent = GL_DEPTH_COMPONENT32;
	}
	return depth_conponent;
}

void QymRenderTexture::_initRT() {

	this->_deleteRT();
	//init color buffer
	/*glGenTextures(1, (GLuint *)&(this->m_iTex));
	glBindTexture(GL_TEXTURE_2D, (GLuint)(this->m_iTex));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->m_iWidth, this->m_iHeight,
		0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	//init depth buffer
	glGenRenderbuffersEXT(1, &this->m_iRBO);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->m_iRBO);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, this->_getDepthComponent(), this->m_iWidth, this->m_iHeight);

	//init frame buffer
	glGenFramebuffersEXT(1, &this->m_iFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->m_iFBO);
	if (this->m_bUseMRT && this->m_iMRTCount > 0) {
		for (int i = 0; i < this->m_iMRTCount; i++) {
			auto tex = std::make_shared<QymTexture>(this->GetWidth(), this->GetHeight());
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, tex->GetTexID(), 0);
			this->m_vMRTTexList.push_back(tex);
		}
	}
	else {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, (GLuint)this->m_iTex, 0);
	}
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->m_iRBO);

	GLenum FBStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	if (FBStatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		this->_deleteRT();
	}
	else
	{
		this->m_bIsValid = true;
	}
}

void QymRenderTexture::_deleteRT() {

	if (glIsFramebuffer(this->m_iFBO)) {
		glDeleteFramebuffersEXT(1, &this->m_iFBO);
		this->m_iFBO = 0;
	}
	if (glIsRenderbuffer(this->m_iRBO)) {
		glDeleteRenderbuffers(1, &this->m_iRBO);
		this->m_iRBO = 0;
	}
	/*if (glIsTexture(this->m_iTex)) {
		glDeleteTextures(1, &this->m_iTex);
		this->m_iTex = 0;
	}*/

	if (this->m_bUseMRT) {
		this->m_vMRTTexList.clear();
		//this->m_vMRTTexList.shrink_to_fit();
	}

	this->m_bIsValid = false;
}