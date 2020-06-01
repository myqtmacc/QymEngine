#pragma once

#include <memory>
#include <vector>

#include "Common/QymCommonHeader.h"
#include "Common/QymEngineInstance.h"
#include "Scene/QymGameObject.h"
#include "Texture/QymRenderTexture.h"
#include "Render/QymRenderCommon.h"
#include "Utils/EventUtil.h"

namespace QymEngine {

	class DLL_EXPORT QymCamera : public QymObjectBase
	{
	public:
		QymCamera();
		~QymCamera();

		void RenderScene(const QymScene & _scene);
		void RenderScene(const QymScene & _scene, const std::vector<GLenum> & drawBuffers);

		std::shared_ptr<QymRenderTexture> GetRenderTexture() const { return this->m_pRT; }
		void SetRenderTexture(const std::shared_ptr<QymRenderTexture> & _rt) { this->m_pRT = _rt; }

		Math::Matrix4x4f GetViewM();

		void SetProjM(const Math::Matrix4x4f & _proj) { this->m_mCustomProj = _proj; this->m_bCustomProj = true; SetProjDirty(); }
		void ResetProjM() { this->m_bCustomProj = false; SetProjDirty(); }
		Math::Matrix4x4f GetProjM();

		bool GetOrthogonal() const { return this->m_bOrthogonalProj; }
		void SetOrthogonal(bool ortho) { this->m_bOrthogonalProj = ortho; SetProjDirty(); }

		Math::Vector2f GetNearFar() const { return this->m_vNearFar; }
		void SetNearFar(Math::Vector2f nearFar) { this->m_vNearFar = nearFar; SetProjDirty(); }

		float GetOrthoSize() const { return this->m_fOrthoSize; }
		void SetOrthoSize(float size) { this->m_fOrthoSize = size; SetProjDirty(); }

		float GetFOV() const { return this->m_fFov; }
		void SetFOV(float fov) { this->m_fFov = fov; SetProjDirty(); }

		float GetRatio();
		void SetRatio(float ratio) { this->m_fRatio = ratio; this->m_bCustomRatio = true; SetProjDirty(); }
		void ResetRatio() { this->m_bCustomRatio = false; SetProjDirty(); }

		void SetProjDirty() { this->m_bProjDirty = true; }

		void SetViewport(const Math::RectF & _viewport) { this->m_rViewport = _viewport; }

		void ActivateCamera() { m_pCurrentCam = this; }

		bool FrustrumIntersected(QymGameObject & obj, E_INTERSECTION_DETECTION_MODE mode);

		static QymCamera * GetCurrentCamera() { return m_pCurrentCam; }

	private:
		void OnResize(const Math::Size2D resolution);

		static QymCamera * m_pCurrentCam;

		const Math::Vector3f vEyePos = Math::Vector3f(0.0f, 0.0f, 0.0f);
		const Math::Vector3f vTargetPos = Math::Vector3f(0.0f, 0.0f, 1.0f);
		const Math::Vector3f vUpDir = Math::Vector3f(0.0f, 1.0f, 0.0f);

		std::shared_ptr<QymRenderTexture> m_pRT;

		Math::Matrix4x4f m_mView;
		Math::Matrix4x4f m_mProj;
		Math::Matrix4x4f m_mCustomProj;

		bool m_bOrthogonalProj;
		Math::Vector2f m_vNearFar;
		float m_fOrthoSize;
		float m_fFov;
		float m_fRatio;
		float m_fCustomRatio;

		bool m_bCustomProj;
		bool m_bCustomRatio;

		bool m_bProjDirty;

		Math::RectF m_rViewport;

		EventID m_ResizeHandler;
	};
}
