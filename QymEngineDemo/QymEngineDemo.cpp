// QymEngineDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <algorithm>
#include "QymEngineDemo.h"
#include "QymEngine.h"

using namespace QymEngine;

const int g_WindowWidth = 1920;
const int g_WindowHeight = 1080;

Math::Vector3f g_CameraPos(0, 1, -5);

static void
fatal_error(char *msg)
{
	MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);
	exit(EXIT_FAILURE);
}

static LRESULT CALLBACK
window_callback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

	switch (msg) {

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_UP:
			g_CameraPos[2] += 0.1f;
			break;
		case VK_DOWN:
			g_CameraPos[2] -= 0.1f;
			break;
		case VK_LEFT:
			g_CameraPos[0] -= 0.1f;
			break;
		case VK_RIGHT:
			g_CameraPos[0] += 0.1f;
			break;
		}
		break;
	case WM_SIZE:
	{
		int width = LOWORD(lparam);
		int height = HIWORD(lparam);
		QymEngineInstance::HandleResize({ width, height });
	}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		result = DefWindowProcA(window, msg, wparam, lparam);
		break;
	}

	return result;
}

static HWND
create_window(HINSTANCE inst)
{
	WNDCLASSA window_class = {};

	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = window_callback;
	window_class.hInstance = inst;
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.hbrBackground = 0;
	window_class.lpszClassName = "WGL_fdjhsklf";

	if (!RegisterClassA(&window_class)) {
		fatal_error("Failed to register window.");
	}

	// Specify a desired width and height, then adjust the rect so the window's client area will be
	// that size.
	RECT rect = {};
	rect.right = g_WindowWidth;
	rect.bottom = g_WindowHeight;

	DWORD window_style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rect, window_style, false);

	HWND window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"QymEngineDemo",
		window_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		inst,
		0);

	if (!window) {
		fatal_error("Failed to create window.");
	}

	return window;
}

int WINAPI
WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd_line, int show)
{
	HWND window = create_window(inst);
	HDC gldc = GetDC(window);
	//HGLRC glrc = init_opengl(gldc);

	ShowWindow(window, show);
	UpdateWindow(window);


	if (!QymEngineInstance::Initialize(window, g_WindowWidth, g_WindowHeight))
	{
		return FALSE;
	}

	QymEngineInstance::SetResolution({640, 360});

	QymEngineInstance::SetVSync(true);        

	QSM_MAKE_OBJECT(QymCamera, camera);
	// auto proj = Math::Perspective(60, 1.5f, 0.1f, 100.0f);
	// camera->SetProjM(proj);
	camera->SetOrthogonal(false);
	camera->SetNearFar({ 0.1f, 100.0f });
	camera->SetFOV(60);

	auto tex = QymTexture::LoadTexture(Path_Join(Path_StripFilename(Path_GetExecutablePath()), RESOURCE_TEXTURE_PATH, "chalet.jpg"));
	   
	QSM_MAKE_OBJECT(QymShaderProgram, program, normal_shader_vs, normal_shader_fs, 1);
	QSM_MAKE_OBJECT(QymMeshRenderer, mr, program);
	mr->AddTexture(0, tex);

	auto mesh = QymMesh::LoadModel(Path_Join(Path_StripFilename(Path_GetExecutablePath()), RESOURCE_MODEL_PATH, "chalet.obj"));
	//auto mesh = QymMesh::BuildGlobe();
	QSM_MAKE_OBJECT(QymGameObject, go, mesh, mr);
	//go->SetLocalModelM(Math::Rotate(90.0f, Math::Vector3f(1.0f, 0, 0)));

	QSM_MAKE_OBJECT(QymScene, scene);
	scene->AddGameObject(go);

	bool running = true;
	while (running) {
		MSG msg;
		while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				running = false;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}

		static float angle = 0;
		go->SetLocalModelM(Math::Rotate(angle, Math::Vector3f(0, 1, 0)) * Math::Rotate(-90.0f, Math::Vector3f(1.0f, 0, 0)));
		angle += 1;;
		camera->SetLocalModelM(Math::Translate(g_CameraPos));
		camera->RenderScene(*scene);
		QymEngineInstance::Present();
	}

	QymEngineInstance::Destroy();

	return 0;
}