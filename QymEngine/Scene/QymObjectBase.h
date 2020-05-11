#pragma once

#include <memory>
#include "Math/QymMath.h"

namespace QymEngine {

#define QSM_OBJECT_PTR(type)				typedef std::shared_ptr<type> P##type
#define QSM_MAKE_OBJECT(type, name, ...)	std::shared_ptr<type> name = std::make_shared<type>(__VA_ARGS__)

	class QymObjectBase {

		QSM_OBJECT_PTR(QymObjectBase);

	public:
		QymObjectBase();
		virtual ~QymObjectBase() = default;

		void SetParent(const PQymObjectBase & _parent) { this->m_goParent = _parent; }
		PQymObjectBase GetParent() const { return this->m_goParent; }

		void SetLocalModelM(const Math::Matrix4x4f & _local);

		Math::Matrix4x4f GetGlobalModelM() const { return this->m_mGlobalModel; }

		virtual void UpdateTransform();

	protected:
		PQymObjectBase m_goParent;

		Math::Matrix4x4f m_mLocalModel;
		Math::Matrix4x4f m_mGlobalModel;

		//void _updateLocalM();
		void _updateGlobalM();
	};
}