#pragma once

#include "glm/glm.hpp"

// Forward decl
class UILayout;
class UIRenderer;
class Shader;
class Mesh;
struct GLFWwindow;

class Game
{
public:
	Game(GLFWwindow* window);
	~Game();
	void run();
private:
	GLFWwindow* m_WindowPointer;
	UILayout* m_UI;
	UIRenderer* m_UIRenderer;
	Shader* m_RenderShader;
	Mesh* m_RenderSurface;

	void Draw();
};