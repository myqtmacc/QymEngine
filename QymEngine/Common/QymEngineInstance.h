#pragma once
#include "Windows.h"
#include "Common/QymCommonHeader.h"
#include "Math/QymMath.h"
#include <vector>

// 使用方法见QymEngineDemo项目
namespace QymEngine {

	class DLL_EXPORT QymEngineInstance {
	public:
		static bool Initialize(HWND hWnd, int width, int height);
		static void Destroy();

		static void SetVSync(bool enable);
		static void Present();

		static void SetResolution(Math::Size2D newSize);
		static Math::Size2D GetResolution();

		static void HandleResize(Math::Size2D newSize);

	private:
		static HWND m_hWnd;
		static HDC m_hDC;
		static HGLRC m_hRC;

		static Math::Size2D m_Resolution;
	};
}