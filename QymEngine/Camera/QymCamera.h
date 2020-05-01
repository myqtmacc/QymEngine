#pragma once

#include <memory>
#include <vector>
#include <vmath.h>

#include "Common/QymCommonBase.h"
#include "Scene/QymGameObject.h"
#include "Texture/QymRenderTexture.h"
#include "Render/QymRenderCommon.h"

using namespace vmath;

namespace QymEngine {

	class QymCamera : public QymObjectBase
	{
	public:
		QymCamera();
		~QymCamera() = default;

		void RenderScene(const QymScene & _scene);
		void RenderScene(const QymScene & _scene, const std::vector<GLenum> & drawBuffers);

		std::shared_ptr<QymRenderTexture> GetRenderTexture() const { return this->m_pRT; }
		void SetRenderTexture(const std::shared_ptr<QymRenderTexture> & _rt) { this->m_pRT = _rt; }

		mat4 GetViewM();
		void SetProjM(const mat4 & _proj) { this->m_mProj = _proj; }
		mat4 GetProjM() const { return this->m_mProj; }
		void SetViewport(const RectF & _viewport) { this->m_rViewport = _viewport; }

		void ActivateCamera() { m_pCurrentCam = this; }

		bool FrustrumIntersected(QymGameObject & obj, E_INTERSECTION_DETECTION_MODE mode);

		static QymCamera * GetCurrentCamera() { return m_pCurrentCam; }

	private:
		static QymCamera * m_pCurrentCam;

		const vec3 vEyePos = vec3(0.0f, 0.0f, 0.0f);
		const vec3 vTargetPos = vec3(0.0f, 0.0f, -1.0f);
		const vec3 vUpDir = vec3(0.0f, 1.0f, 0.0f);

		std::shared_ptr<QymRenderTexture> m_pRT;

		mat4 m_mView;
		mat4 m_mProj;

		RectF m_rViewport;
	};
}