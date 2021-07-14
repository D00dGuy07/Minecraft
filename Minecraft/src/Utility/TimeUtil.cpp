#include "TimeUtil.h"
#include <GLFW/glfw3.h>

double Time::m_CurrentFrame = 0;
double Time::m_LastFrame = 0;
double Time::DeltaTime = 0;

void Time::Update()
{
	Time::m_CurrentFrame = glfwGetTime();
	Time::DeltaTime = m_CurrentFrame - m_LastFrame;
	Time::m_LastFrame = m_CurrentFrame;
}
