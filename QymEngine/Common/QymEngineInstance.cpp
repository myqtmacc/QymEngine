#include "Common/QymEngineInstance.h"
#include "Common/QymCommonHeader.h"

using namespace QymEngine;
using namespace QymEngine::Math;


HWND QymEngineInstance::m_hWnd = nullptr;
HDC QymEngineInstance::m_hDC = nullptr;
HGLRC QymEngineInstance::m_hRC = nullptr;;

Size2D QymEngineInstance::m_Resolution(1280, 720);

bool QymEngineInstance::Initialize(HWND hWnd, int width, int height)
{
	m_hWnd = hWnd;
	m_Resolution = Size2D(width, height);

	m_hDC = GetDC(hWnd);
	if (m_hDC == nullptr) {
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	HGLRC tempContext = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, tempContext);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		return false;
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		m_hRC = wglCreateContextAttribsARB(m_hDC, 0, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(m_hDC, m_hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		m_hRC = tempContext;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	return m_hRC != NULL;
}

void QymEngineInstance::Destroy()
{
	if (m_hRC) {
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_hRC);
		m_hRC = nullptr;
	}

	if (m_hDC && m_hWnd) {
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = nullptr;
		m_hWnd = nullptr;
	}
}

Size2D QymEngineInstance::Resolution()
{
	return m_Resolution;
}

void QymEngineInstance::SetVSync(bool enable)
{
	wglSwapIntervalEXT(enable ? 1 : 0);
}

void QymEngineInstance::Present()
{
	SwapBuffers(m_hDC);
}
