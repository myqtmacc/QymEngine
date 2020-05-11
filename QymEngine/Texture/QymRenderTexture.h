#pragma once

#include <memory>
#include <vector>
#include "Common/QymCommonHeader.h"
#include "Texture/QymTexture.h"

namespace QymEngine {

	class DLL_EXPORT QymMRTUnsupportedException
	{
	public:
		QymMRTUnsupportedException() = delete;
		QymMRTUnsupportedException(const std::string &msg, GLint _max) : errMsg(msg), maxMRT(_max){}
	private:
		std::string errMsg;
		GLint maxMRT;
	};

	class QymRenderTexture : public QymTexture
	{
	public:
		QymRenderTexture(const QymRenderTexture &) = delete;
		QymRenderTexture & operator = (const QymRenderTexture &) = delete;
		QymRenderTexture(GLint w, GLint h, GLint depthCount);
		QymRenderTexture(GLint w, GLint h, GLint depthCount, int MRTs);
		~QymRenderTexture();

		GLuint GetFBO() const { return this->m_iFBO; }
		//GLuint GetTexID() { return this->m_iColorTex; }

		std::vector<std::shared_ptr<QymTexture> > & GetMRTTexList() { return this->m_vMRTTexList; }

	private:
		GLuint m_iFBO;
		GLuint m_iRBO; //depth buffer
		GLint m_iDepthCount;
		//GLuint m_iColorTex; //color buffer

		bool m_bUseMRT;
		int m_iMRTCount;
		std::vector<std::shared_ptr<QymTexture> > m_vMRTTexList;

		//GLint m_iWidth;
		//GLint m_iHeight;

		bool m_bIsValid;

		void _initRT();
		void _deleteRT();

		GLenum _getDepthComponent()const;
	};
}