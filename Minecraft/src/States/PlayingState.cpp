#include "PlayingState.h"

#include <iostream>

// Renderer Includes
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Chunk.h"
#include <vector>

#include "ChunkManager.h"

void PlayingState::load(GLFWwindow* window)
{
	// Set GLFW window pointer and key callback
	glfwSetWindowUserPointer(window, (this));
	glfwSetKeyCallback(window, PlayingState::key_callback);

	// Setup Camera
	m_Camera = new Camera(window, glm::vec3(0, 45, 0), glm::vec3(0.5f, 0.0f, 0.5f), 10, 75.0f, 0.1f, 500.0f);

	// Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable Culling
	glEnable(GL_CULL_FACE);

	// Setup OpenGL data for quad
	
	m_Shader = new Shader("res/shaders/Cube.shader");

	m_Texture = new Texture("res/textures/texture-atlas.png", GL_NEAREST);

	m_Shader->SetUniform1i("u_Texture", 0);

	// Build chunk mesh
	m_ChunkManager = new ChunkManager();

	m_ChunkManager->GenerateTerrain();

	std::cout << "Playing state has been loaded!" << std::endl;
}

GameStateMessage PlayingState::loop(GLFWwindow* window)
{
	Renderer::Clear();
	Renderer renderer = Renderer();
	
	glm::mat4 view = m_Camera->UpdateCamera();
	
	m_Texture->Bind();

	m_Shader->SetUniformMat4f("u_MVP", view);

	if (m_ChunkManager->NeedsGenerating())
	{
		m_ChunkManager->GenerateMeshes();
	}

	std::vector<Mesh*> meshes = m_ChunkManager->GetMeshes();

	for (Mesh* mesh : meshes)
	{
		if (mesh->isVAOBuilt())
		{
			renderer.Draw(*mesh, *m_Shader);
		}
	}

	if (m_ShouldCrash) {
		m_ShouldCrash = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return { GameStateStatus::CRASH, "The c key was pressed!" };
	}

	return { GameStateStatus::OKAY, "" };
}

void PlayingState::close(GLFWwindow* window)
{
	// Reset GLFW window pointer and key callback
	glfwSetWindowUserPointer(window, NULL);
	glfwSetKeyCallback(window, NULL);

	delete m_Camera;
	delete m_Texture;

	delete m_Shader;
	delete m_ChunkManager;

    std::cout << "Playing state has been closed!" << std::endl;
}

void PlayingState::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		int mode = glfwGetInputMode(window, GLFW_CURSOR);

		if (mode == GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_Camera->ResetMousePos();
		}
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		m_Camera->SetFOV(20.0f);
	}
	else if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		m_Camera->SetFOV(75.0f);
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		m_WireFrame = !m_WireFrame;
		if (m_WireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
	}
	else
		m_Camera->UpdateKeyStates(key, action);
}

void PlayingState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	PlayingState* state = (PlayingState*)glfwGetWindowUserPointer(window);
	if (state)
		state->keyEvent(window, key, scancode, action, mods);
}
