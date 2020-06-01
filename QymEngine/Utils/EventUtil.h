#pragma once
#include <map>
#include <vector>
#include <functional>

namespace QymEngine {

	typedef size_t EventID;
	static EventID INVALID_EVENTID = -1;

	template<typename T>
	class QymEventUtil
	{
	public:
		typedef std::function<void(const T &)> EventHandler;

		static EventID Listen(const EventHandler & h) {
			auto id = _getNextId();
			observers_[id] = h;
			return id;
		}

		static void Remove(EventID e) {
			observers_.erase(e);
		}

		static void Send(const T & t)
		{
			for (auto & it : observers_)
			{
				it.second(t);
			}
		}

	private:

		static EventID _getNextId()
		{
			static size_t s_id = 0;
			return s_id++;
		}
		static std::map<EventID, EventHandler> observers_;
	};

	template<typename T>
	std::map<EventID, typename QymEventUtil<T>::EventHandler> QymEventUtil<T>::observers_;
}