#pragma once

#include <memory>
#include "vmath.h"

namespace QymEngine {

	using namespace vmath;

#define QSM_OBJECT_PTR(type)				typedef std::shared_ptr<type> P##type
#define QSM_MAKE_OBJECT(type, name, ...)	std::shared_ptr<type> name = std::make_shared<type>(__VA_ARGS__)

	class QymObjectBase {

		QSM_OBJECT_PTR(QymObjectBase);

	public:
		QymObjectBase();
		virtual ~QymObjectBase() = default;

		void SetParent(const PQymObjectBase & _parent) { this->m_goParent = _parent; }
		PQymObjectBase GetParent() const { return this->m_goParent; }

		void SetLocalModelM(const mat4 & _local);

		mat4 GetGlobalModelM() const { return this->m_mGlobalModel; }

		virtual void UpdateTransform();

	protected:
		PQymObjectBase m_goParent;

		mat4 m_mLocalModel;
		mat4 m_mGlobalModel;

		//void _updateLocalM();
		void _updateGlobalM();
	};
}