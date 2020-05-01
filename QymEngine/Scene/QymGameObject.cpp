#include "Scene/QymGameObject.h"
#include "Render/QymMeshRenderer.h"

using namespace QymEngine;

QymGameObject::QymGameObject()
{
}

QymGameObject::QymGameObject(const PQymMesh & _mesh) :
m_pMesh(_mesh),
m_AABB(_mesh->BuildAABB()),
m_OBBTransform(mat4::identity())
{
	this->m_OBB = _mesh->BuildOBB(this->m_OBBTransform);
}

QymGameObject::QymGameObject(const PQymMesh & _mesh, const PQymMeshRenderer & _renderer) :
m_pMesh(_mesh),
m_pMeshRenderer(_renderer),
m_AABB(_mesh->BuildAABB()),
m_OBBTransform(mat4::identity())
{
	this->m_OBB = _mesh->BuildOBB(this->m_OBBTransform);
}

QymGameObject::QymGameObject(const PQymMesh & _mesh, const PQymMeshRenderer & _renderer, const mat4 & obbTransform) :
m_pMesh(_mesh),
m_pMeshRenderer(_renderer),
m_AABB(_mesh->BuildAABB()),
m_OBBTransform(obbTransform)
{
	this->m_OBB = _mesh->BuildOBB(this->m_OBBTransform);
}

void QymGameObject::DrawGameObject() {
	this->m_pMeshRenderer->DrawMesh(*this->m_pMesh, this->m_mGlobalModel);
}