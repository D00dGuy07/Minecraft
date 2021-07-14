#include "PlayingState.h"

#include <iostream>

// Renderer Includes
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Chunk.h"
#include <vector>

#include "UsefulMath.h"

#include "ChunkManager.h"

void PlayingState::load(GLFWwindow* window)
{
	// Set GLFW window pointer and key callback
	glfwSetWindowUserPointer(window, (this));
	glfwSetKeyCallback(window, PlayingState::key_callback);

	// Setup Camera
	m_Camera = new Camera(window, glm::vec3(0, 145, 0), glm::vec3(0.0f, 0.0f, 1.0f), 50, 75.0f, 0.1f, 500.0f);

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
	glm::ivec3 pos = UsefulMath::getChunkFromPosition(glm::vec3(0, 145, 0));
	m_LastChunk = pos;
	m_ChunkManager = new ChunkManager(pos.x, pos.y, pos.z);

	std::cout << "Playing state has been loaded!" << std::endl;
}

GameStateMessage PlayingState::loop(GLFWwindow* window)
{
	Renderer::Clear();
	Renderer renderer = Renderer();
	
	glm::mat4 view = m_Camera->UpdateCamera();
	
	m_Texture->Bind();

	m_Shader->SetUniformMat4f("u_MVP", view);
	
	glm::ivec3 chunk = UsefulMath::getChunkFromPosition(m_Camera->GetPos());

	
	if (m_UpdateChunks)
	{
		if (chunk != m_LastChunk)
		{
			m_ChunkManager->SetMiddleChunk(chunk);
			m_LastChunk = chunk;
		}
	}
	
	m_ChunkManager->GenerateChunk();

	glm::vec3 centerPos = m_ChunkManager->GetCenterPos();
	glm::vec3 minBound = glm::vec3(centerPos.x - 4, centerPos.y - 4, centerPos.z - 4);
	glm::vec3 maxBound = glm::vec3(centerPos.x + 4, centerPos.y + 4, centerPos.z + 4);

	for (int y = (int)minBound.y; y <= maxBound.y; y++)
	{
		for (int z = (int)minBound.z; z <= maxBound.z; z++)
		{
			for (int x = (int)minBound.x; x <= maxBound.x; x++)
			{
				Mesh* mesh = m_ChunkManager->getMesh(x, y, z);
				if (mesh != nullptr && mesh->isVAOBuilt())
					renderer.Draw(*mesh, *m_Shader);
			}
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

	if (m_ResetCamera)
		delete m_ResetCamera;

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
	else if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		m_UpdateChunks = !m_UpdateChunks;
		if (!m_UpdateChunks)
		{
			m_ResetCamera = new Camera(*m_Camera);
		}
		else
		{
			delete m_Camera;
			m_Camera = new Camera(*m_ResetCamera);
			delete m_ResetCamera;
		}
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		m_Camera->SetSpeed(m_Camera->GetSpeed() + 5);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		m_Camera->SetSpeed(m_Camera->GetSpeed() - 5);
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
