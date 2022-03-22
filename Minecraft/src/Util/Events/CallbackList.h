#pragma once

#include <vector>
#include <tuple>
#include <functional>

#include "EventConnection.h"

template<typename... arguments> class CallbackList;

template<typename... arguments>
class CallbackList
{
public:
	Ref<EventConnection<arguments...>>& Connect(std::function<void(arguments...)> callback)
	{
		m_CallbackList.emplace_back(Ref<EventConnection<arguments...>>::Create(callback));
		return *(m_CallbackList.end() - 1);
	}

	void Call(arguments... args)
	{
		std::vector<Ref<EventConnection<arguments...>>> activeCallbacks;
		activeCallbacks.reserve(m_CallbackList.size());
		for (auto& callback : m_CallbackList)
		{
			if (callback->IsConnected())
			{
				callback->Callback()(args...);
				if (callback->IsConnected())
					activeCallbacks.push_back(callback);
			}
		}

		m_CallbackList = std::move(activeCallbacks);
	}

private:
	std::vector<Ref<EventConnection<arguments...>>> m_CallbackList;
};