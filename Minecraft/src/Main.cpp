#include <iostream>

#include "Util/GLFWLoad.h"

#include "Game.h"

// Enable Nvidia high performace graphics
#include <windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(void)
{
	// Load OpenGL/GLFW
	GLFWwindow* window = LoadGLFW(1280, 720);

	if (window == NULL)
		return -1;

	// Run application
	Game* game = new Game(window);
	game->run();
	delete game;

	// Clean stuff up
	glfwTerminate();

	return 0;
}