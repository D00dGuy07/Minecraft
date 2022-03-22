#include "Game.h"

// Rendering includes
#include "Renderer/Renderer.h"

// UI includes
#include "UI/UILoader.h"
#include "UI/UILayout.h"
#include "UI/UISize.h"
#include "UI/UIConstraint.h"
#include "UI/UIRenderer.h"
#include "UI/Components/Frame.h"

// glm includes
#include <glm/ext/matrix_clip_space.hpp>

// Renderer includes
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Texture.h"

// GLFW
#include "GLFW/glfw3.h"
#include "Util/GLFWEventWrapper.h"

#include <iostream>
#include <functional>

#include "UI/Parsers/Parsers.h"

Game::Game(GLFWwindow* window)
	: m_WindowPointer(window),
	m_RenderShader(new Shader("res/shaders/Render.shader", false)),
	m_RenderSurface(new Mesh()), m_UI(NULL), m_UIRenderer(new UIRenderer())
{
	// Setup render shader and render surface

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_RenderSurface->SetVertices(vertices, 4 * 4 * 4);
	m_RenderSurface->SetIndices(indices, 6);
	m_RenderSurface->BufferLayout.Push<float>(2);
	m_RenderSurface->BufferLayout.Push<float>(2);
	m_RenderSurface->Construct();

	m_RenderShader->SetUniform1i("u_Texture", 0);

	// Event
	GLFWEventWrapper* wrapper = GLFWEventWrapper::GetWrapper(window);
	wrapper->ConnectFramebufferSize(std::function<void(GLFWwindow*, int, int)>([&](GLFWwindow*, int width, int height) {
		//std::cout << "Resized: (" << width << ", " << height << ")\n";
		if (m_UI != NULL)
			m_UI->Resize(width, height);
		Renderer::Submit([=]() {
			glViewport(0, 0, width, height);
		});
		Draw();
	}));

	// Loading data
	auto layouts = UILoader::LoadLayouts("res/layouts/layout.xml");

	m_UI = layouts["Main"];
	m_UI->Resize(1280, 720);

	Renderer::Submit([]() {
		// Enable Depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable Culling
		glEnable(GL_CULL_FACE);
	});
}

Game::~Game()
{}

void Game::Draw()
{
	Renderer::Clear();

	//Frame* frame = reinterpret_cast<Frame*>(m_UI->GetComponent("frame1"));
	//UITransform transform = frame->GetActualTransform();
	//std::cout << "Frame Size: (" << transform.Width << ", " << transform.Height << ")\n";
	//std::cout << "Frame Position: (" << transform.PosX << ", " << transform.PosY << ")\n";

	m_UI->ForceRender();
	m_UIRenderer->Render(m_UI);
	static_cast<Texture*>(m_UIRenderer->GetFramebuffer()->GetColorAttachment(0).Buffer)->Bind();
	Renderer::SubmitMesh(*m_RenderSurface, *m_RenderShader);

	Renderer::DrawFrame();

	glfwSwapBuffers(m_WindowPointer);
}

void Game::run()
{
	// Render loop
	while (!glfwWindowShouldClose(m_WindowPointer))
	{
		Draw();
		glfwPollEvents();
	}
}