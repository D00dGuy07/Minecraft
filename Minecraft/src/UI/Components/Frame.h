#pragma once

#include "UI/UIComponent.h"

#include "glm/glm.hpp"

#include <string>

class Frame : public UIComponent
{
public:
	Frame(std::string id);

	inline void SetFill(const glm::vec4& fill) { m_Fill = fill; }
	inline void SetRoundness(float roundness) { m_Roundness = roundness; }

protected:

	virtual void AddRenderInstances(UIRenderer* renderer) override;

private:
	glm::vec4 m_Fill = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	float m_Roundness = 0.0f;
	
	friend class UILayout;
};