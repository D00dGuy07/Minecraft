#include "UI/Components/Frame.h"

#include "UI/UIRenderer.h"

#include "GLFW/glfw3.h"

#include <iostream>

Frame::Frame(std::string id)
	: UIComponent(id) {}

void Frame::AddRenderInstances(UIRenderer* renderer)
{
	UIRect rect = ToRect(m_ActualTransform);
	UIInstancingSpec spec(
		static_cast<float>(m_ZIndex),
		static_cast<double>(glfwGetTime()),
		glm::vec2(m_ActualTransform.Width, m_ActualTransform.Height), 
		glm::vec2(rect.LeftX, rect.TopY), 
		m_Roundness != 0 || m_Fill.w != 1.0f,
		sizeof(glm::vec4) + sizeof(float)
	);

	*reinterpret_cast<glm::vec4*>(spec.Data) = m_Fill;
	*reinterpret_cast<float*>(spec.Data + sizeof(glm::vec4)) = m_Roundness;

	renderer->AddQuad("Frame.shader", spec);
}
