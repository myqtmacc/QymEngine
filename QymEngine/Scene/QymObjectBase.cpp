#include "Scene/QymObjectBase.h"

using namespace QymEngine;
using namespace QymEngine::Math;

QymObjectBase::QymObjectBase() :
m_mLocalModel(Identity<Matrix4x4f>())
{
	this->_updateGlobalM();
}

void QymObjectBase::UpdateTransform() {
	this->_updateGlobalM();
}

void QymObjectBase::SetLocalModelM(const Matrix4x4f & _local) {
	this->m_mLocalModel = _local;
	this->_updateGlobalM();
}

void QymObjectBase::_updateGlobalM() {
	if (this->m_goParent) {
		this->m_mGlobalModel = this->m_goParent->m_mGlobalModel * this->m_mLocalModel;
	}
	else {
		this->m_mGlobalModel = this->m_mLocalModel;
	}
}