#pragma once

#include <functional>
#include "Ref.h"

template<typename... parameters>
class EventConnection : public RefCounted
{
public:
	EventConnection(const std::function<void(parameters...)>& m_Callback)
		: m_Callback(m_Callback), m_Connected(true) {}

	EventConnection(const EventConnection& other)
		: m_Callback(other.m_Callback), m_Connected(other.m_Connected) {}

	inline void Disconnect() { m_Connected = false; }
	inline bool IsConnected() const { return m_Connected; }

	inline const std::function<void(parameters...)> Callback() const { return m_Callback; }

private:
	std::function<void(parameters...)> m_Callback;
	bool m_Connected;
};