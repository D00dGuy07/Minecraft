#pragma once

struct GLFWwindow;

class Game
{
public:
	Game(GLFWwindow* window);
	void run();
private:
	GLFWwindow* m_WindowPointer;
};