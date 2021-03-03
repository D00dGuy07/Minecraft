#include "PlayingState.h"

#include <iostream>

// Renderer Includes
#include "Shader.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Renderer.h"

#include "Mesh.h"
#include "Chunk.h"
#include "ChunkMeshGenerator.h"
#include "RenderInfo.h"

void PlayingState::load(GLFWwindow* window)
{
	// Set GLFW window pointer and key callback
	glfwSetWindowUserPointer(window, (this));
	glfwSetKeyCallback(window, PlayingState::key_callback);

	// Setup Camera
	m_Camera = new Camera(window, glm::vec3(0, 0, 2), glm::vec3(0, 0, -1), 10, 75.0f, 0.1f, 100.0f);

	// Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable Culling
	glEnable(GL_CULL_FACE);

	// Setup OpenGL data for quad
	
	m_Shader = std::shared_ptr<Shader>(
		new Shader("res/shaders/Cube.shader")
	);

	m_Texture = new Texture("res/textures/texture-atlas.png", GL_NEAREST);

	m_Shader->SetUniform1i("u_Texture", 0);

	// Build chunk mesh

	m_Chunk = new Chunk();

	for (int y = 0; y < 16; y++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				if (y == 15)
				{
					m_Chunk->setBlock(1, x, y, z);
				}
				else if (y > 11 && y < 15)
				{
					m_Chunk->setBlock(2, x, y, z);
				}
				else if (y < 12)
				{
					m_Chunk->setBlock(3, x, y, z);
				}
			}
		}
	}

	std::shared_ptr<Mesh> mesh = ChunkMeshGenerator::generateChunkMesh(*m_Chunk);

	m_RenderInfo = new RenderInfo(mesh, m_Shader);

	m_Model = new glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));

	std::cout << "Playing state has been loaded!" << std::endl;
}

GameStateMessage PlayingState::loop(GLFWwindow* window)
{
	Renderer::Clear();
	Renderer renderer = Renderer();
	
	glm::mat4 view = m_Camera->UpdateCamera();
	
	m_Texture->Bind();

	m_Shader->SetUniformMat4f("u_MVP", view * *m_Model);

	renderer.Draw(*m_RenderInfo);

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

	delete m_Chunk;
	delete m_Model;
	delete m_RenderInfo;

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
		m_ShouldCrash = true;
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
