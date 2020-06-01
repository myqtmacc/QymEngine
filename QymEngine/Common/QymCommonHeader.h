#pragma once
#include <string>

#include "..\3rd-party\glew\glew-1.9.0-win64\glew-1.9.0\include\GL/glew.h"
#include "..\3rd-party\glew\glew-1.9.0-win64\glew-1.9.0\include\GL\wglew.h"

#ifdef QYM_ENGINE_DLL
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glew32.lib" )
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "..\3rd-party\glm/glm/glm.hpp"
#include "..\3rd-party\glm/glm/gtc/matrix_transform.hpp"
#include "..\3rd-party\glm/glm/gtx/hash.hpp"

#ifdef QYM_ENGINE_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport) 
#endif

#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma warning(disable:4305)

namespace QymEngine {

#pragma region äÖÈ¾×é¼þ
	class QymCamera;
	class QymGameObject;
	class QymMesh;
	class QymMeshRenderer;
	class QymObjectBase;
	class QymRenderTexture;
	class QymScene;
	class QymShaderProgram;
	class QymTexture;
#pragma endregion

	class QymRendering;
}
