#include "Common//QymEngineInstance.h"
#include <GL/glew.h>
#include <GL/wglew.h>

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

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	GLuint PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		return false;								// Return FALSE
	}


	if (!(m_hRC = wglCreateContext(m_hDC))) {
		return false;
	}

	if (!wglMakeCurrent(m_hDC, m_hRC)) {
		return false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		const char* p = (const char*)glewGetErrorString(err);
		return false;
	}

	return true;
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
