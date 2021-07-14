#include "Game.h"

#include "GLFW/glfw3.h"

// Data Includes
#include "DataLoader.h"

// State Includes
#include "StateManager.h"

// Utility Includes
#include "TimeUtil.h"

// Renderer Includes
#include "TextureAtlas.h"

Game::Game(GLFWwindow* window)
{
	m_WindowPointer = window;
}

void Game::run()
{
	TextureAtlas::SetAtlasSize(8.0f, 8.0f);
	DataLoader::LoadBlocks();

	// Setup State
	StateManager manager(m_WindowPointer);

	while (!glfwWindowShouldClose(m_WindowPointer))
	{
		Time::Update();

		manager.Update();

		glfwSwapBuffers(m_WindowPointer);
		glfwPollEvents();
	}
}
