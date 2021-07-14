#pragma once

class Time
{
private:
	static double m_CurrentFrame;
	static double m_LastFrame;
public:
	static double DeltaTime;

	static void Update();
};