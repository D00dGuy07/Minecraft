#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Renderer Includes
#include "TextureAtlas.h"

// Game
#include "Game.h"

// Enable Nvidia
#include <windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << message << std::endl;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	int windowWidth = 1280;// 1280;
	int windowHeight = 720;// 720;

	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
		std::cout << "Failed to load Glad!" << std::endl;

	glDebugMessageCallback(openglErrorCallback, nullptr);

	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << glGetString(GL_RENDERER) << "\n\n";

	glfwSetWindowSizeLimits(window, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

	Game game = Game(window);

	game.run();
	
	glfwTerminate();
	return 0;
}