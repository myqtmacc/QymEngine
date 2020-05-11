#pragma once

#include <memory>
#include "Common/QymCommonHeader.h"
#include "Scene/QymObjectBase.h"
#include "Mesh//QymMesh.h"
#include "Math/QymMath.h"

namespace QymEngine {

	class DLL_EXPORT QymGameObject : public QymObjectBase {

		QSM_OBJECT_PTR(QymMeshRenderer);
		QSM_OBJECT_PTR(QymMesh);

	public:
		QymGameObject();
		QymGameObject(const PQymMesh & _mesh);
		QymGameObject(const PQymMesh & _mesh, const PQymMeshRenderer & _renderer);
		QymGameObject(const PQymMesh & _mesh, const PQymMeshRenderer & _renderer, const Math::Matrix4x4f & obbTransform);
		~QymGameObject() = default;

		void DrawGameObject();

		void SetMeshRenderer(const PQymMeshRenderer & _renderer) { this->m_pMeshRenderer = _renderer; }
		PQymMeshRenderer GetMeshRenderer() const { return this->m_pMeshRenderer; }
		void SetMesh(const PQymMesh & _mesh) { this->m_pMesh = _mesh; }
		PQymMesh GetMesh() const { return this->m_pMesh; }

		Math::QymAABB GetAABB() const { return this->m_AABB; }
		Math::QymOBB GetOBB() const { return this->m_OBB; }

	private:

		PQymMesh m_pMesh;
		PQymMeshRenderer m_pMeshRenderer;

		Math::QymAABB m_AABB;
		Math::Matrix4x4f m_OBBTransform;
		Math::QymOBB m_OBB;
	};
};