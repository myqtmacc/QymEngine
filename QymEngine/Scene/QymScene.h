#pragma once

#include "QymGameObject.h"
#include <vector>
#include <memory>

namespace QymEngine {

	QSM_OBJECT_PTR(QymGameObject);
	typedef std::vector<PQymGameObject> PGameObjects;

	class QymScene
	{
	public:
		QymScene() = default;
		~QymScene() = default;

		void AddGameObject(const std::shared_ptr<QymGameObject> & _obj) { this->m_vObjList.push_back(_obj); }
		void RemoveGameObject(const std::shared_ptr<QymGameObject> & _obj)
		{
			std::vector<std::shared_ptr<QymGameObject> >::iterator iter;
			for (iter = this->m_vObjList.begin(); iter != this->m_vObjList.end(); ++iter) {
				if (*iter == _obj) {
					break;
				}
			}

			this->m_vObjList.erase(iter);
		}
		void ClearGameObjects() { this->m_vObjList.clear(); }

		PGameObjects & GetGameObjects() { return this->m_vObjList; }

	private:
		std::vector<std::shared_ptr<QymGameObject> > m_vObjList;
	};
}