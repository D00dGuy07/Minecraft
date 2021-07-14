#pragma once

#include <memory>

#include "GameState.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Renderer Classes
//class Mesh;
class Shader;
class Texture;
class ChunkManager;
//struct Chunk;
//struct RenderInfo;


class PlayingState : public GameState 
{
public:
	void load(GLFWwindow* window) override;
	GameStateMessage loop(GLFWwindow* window) override;
	void close(GLFWwindow* window) override;

	void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	Camera* m_Camera;
	Camera* m_ResetCamera;
	
	Shader* m_Shader;
	Texture* m_Texture;

	glm::ivec3 m_LastChunk;

	ChunkManager* m_ChunkManager;

	bool m_ShouldCrash = false;
	bool m_WireFrame = false;
	bool m_UpdateChunks = true;

};