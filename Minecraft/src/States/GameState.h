#pragma once

#include <iostream>
#include <String>

#include <GLFW\glfw3.h>

enum class GameStateStatus 
{
	OKAY,
	EXIT,
	SWITCH,
	CRASH
};

struct GameStateMessage
{
	GameStateStatus Status;
	std::string Message;
};

class GameState
{
public:
	virtual void load(GLFWwindow* window) { std::cout << "Load state method has not been overriden!" << std::endl; }

	virtual GameStateMessage loop(GLFWwindow* window) {
		std::cout << "Loop state method has not been overriden!" << std::endl;
		return { GameStateStatus::CRASH, "Base method has been called" };
	};

	virtual void close(GLFWwindow* window) { std::cout << "Close state method has not been overriden!" << std::endl; };

	virtual void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) { std::cout << "keyEvent state method has not been overriden!" << std::endl; };
};