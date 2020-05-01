#pragma once

#include <memory>
#include "Common/QymCommonBase.h"
#include "Scene/QymObjectBase.h"
#include "Mesh//QymMesh.h"
#include "Math/QymMath.h"

namespace QymEngine {

	using namespace vmath;

	class QymGameObject : public QymObjectBase {

		QSM_OBJECT_PTR(QymMeshRenderer);
		QSM_OBJECT_PTR(QymMesh);

	public:
		QymGameObject();
		QymGameObject(const PQymMesh & _mesh);
		QymGameObject(const PQymMesh & _mesh, const PQymMeshRenderer & _renderer);
		QymGameObject(const PQymMesh & _mesh, const PQymMeshRenderer & _renderer, const mat4 & obbTransform);
		~QymGameObject() = default;

		void DrawGameObject();

		void SetMeshRenderer(const PQymMeshRenderer & _renderer) { this->m_pMeshRenderer = _renderer; }
		PQymMeshRenderer GetMeshRenderer() const { return this->m_pMeshRenderer; }
		void SetMesh(const PQymMesh & _mesh) { this->m_pMesh = _mesh; }
		PQymMesh GetMesh() const { return this->m_pMesh; }

		QymAABB GetAABB() const { return this->m_AABB; }
		QymOBB GetOBB() const { return this->m_OBB; }

	private:

		PQymMesh m_pMesh;
		PQymMeshRenderer m_pMeshRenderer;

		QymAABB m_AABB;
		mat4 m_OBBTransform;
		QymOBB m_OBB;
	};
};