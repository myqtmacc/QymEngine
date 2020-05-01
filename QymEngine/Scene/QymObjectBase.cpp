#include "Scene/QymObjectBase.h"

using namespace QymEngine;

QymObjectBase::QymObjectBase() :
m_mLocalModel(mat4::identity())
{
	this->_updateGlobalM();
}

void QymObjectBase::UpdateTransform() {
	this->_updateGlobalM();
}

void QymObjectBase::SetLocalModelM(const mat4 & _local) {
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