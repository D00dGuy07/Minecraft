#pragma once

#include <unordered_map>
#include <string>

#include <GLFW\glfw3.h>

#include "GameState.h"

class StateManager
{
public:
	StateManager(GLFWwindow* window);

	void Update();

	~StateManager();
private:
	GLFWwindow* m_WindowPointer;
	std::string m_LoadedState = "";
	GameState* m_CurrentState;
	std::unordered_map<std::string, GameState*> m_States;
};