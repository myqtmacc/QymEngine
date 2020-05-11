#pragma once
#include "Windows.h"
#include "Math/QymMath.h"

#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glew32.lib" )

// 使用方法见QymEngineDemo项目
namespace QymEngine {

	class QymEngineInstance {
	public:
		static bool Initialize(HWND hWnd, int width, int height);
		static void Destroy();

		static void SetVSync(bool enable);
		static void Present();

		static Math::Size2D Resolution();

	private:
		static HWND m_hWnd;
		static HDC m_hDC;
		static HGLRC m_hRC;

		static Math::Size2D m_Resolution;
	};
}