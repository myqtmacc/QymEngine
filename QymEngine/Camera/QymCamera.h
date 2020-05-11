#pragma once

#include <memory>
#include <vector>

#include "Common/QymCommonBase.h"
#include "Scene/QymGameObject.h"
#include "Texture/QymRenderTexture.h"
#include "Render/QymRenderCommon.h"

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

		Math::Matrix4x4f GetViewM();
		void SetProjM(const Math::Matrix4x4f & _proj) { this->m_mProj = _proj; }
		Math::Matrix4x4f GetProjM() const { return this->m_mProj; }
		void SetViewport(const Math::RectF & _viewport) { this->m_rViewport = _viewport; }

		void ActivateCamera() { m_pCurrentCam = this; }

		bool FrustrumIntersected(QymGameObject & obj, E_INTERSECTION_DETECTION_MODE mode);

		static QymCamera * GetCurrentCamera() { return m_pCurrentCam; }

	private:
		static QymCamera * m_pCurrentCam;

		const Math::Vector3f vEyePos = Math::Vector3f(0.0f, 0.0f, 0.0f);
		const Math::Vector3f vTargetPos = Math::Vector3f(0.0f, 0.0f, 1.0f);
		const Math::Vector3f vUpDir = Math::Vector3f(0.0f, 1.0f, 0.0f);

		std::shared_ptr<QymRenderTexture> m_pRT;

		Math::Matrix4x4f m_mView;
		Math::Matrix4x4f m_mProj;

		Math::RectF m_rViewport;
	};
}