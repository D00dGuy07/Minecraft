#include "UI/UILayout.h"

#include "UI/UIComponent.h"

#include "Util/VectorUtil.h"

#include <stdexcept>

UILayout::~UILayout()
{
	for (auto pair : m_Components)
		delete pair.second.Component;
}

// If a component already exists with that idea then it is destroyed and replaced
void UILayout::AddComponent(UIComponent* component)
{
	UIComponentData& data = m_Components[component->GetID()];
	if (data.Component != NULL)
		delete data.Component;

	data.Component = component;
	data.Updated = false;
	component->GetDependencies(data.Dependencies);

	m_ForceRender = true;
}

UIComponent* UILayout::GetComponent(const std::string& id)
{
	UIComponentData* data = GetComponentData(id);
	if (data != NULL)
		return data->Component;

	return NULL;
}

void UILayout::Resize(int width, int height)
{
	m_Width = width;
	m_Height = height;

	for (auto& pair : m_Components)
		if (!pair.second.Updated)
			RecursiveUpdate(pair.first);

	for (auto& pair : m_Components)
		pair.second.Updated = false;

	m_ForceRender = true;
}

void UILayout::Verify()
{
	for (auto pair : m_Components)
		RecursiveVerify(pair.first, std::vector<std::string>());
}

bool UILayout::FillRenderQueues(UIRenderer* renderer)
{
	// Ability to force a render is nice
	if (!m_ForceRender)
	{
		// Check if anything has changed
		bool shouldSkip = true;
		for (auto& pair : m_Components)
		{
			if (pair.second.Component->m_Updated)
			{
				shouldSkip = false;
				break;
			}
		}

		if (shouldSkip)
			return false;
	}
	m_ForceRender = false;

	// Since something has changed we should rerender the frame
	for (auto& pair : m_Components)
	{
		pair.second.Component->AddRenderInstances(renderer);
		pair.second.Component->m_Updated = false;
	}

	return true;
}


UIComponentData* UILayout::GetComponentData(const std::string& id)
{
	if (m_Components.find(id) == m_Components.end())
		return NULL;
	return &m_Components[id];
}

void UILayout::RecursiveVerify(const std::string& id, std::vector<std::string> dependencies)
{
	UIComponentData* data = GetComponentData(id);
	if (data == NULL)
		throw NonexistentComponentError("Couldn't find a component with the specified id!", id);

	for (std::string& depID : data->Dependencies)
	{
		if (VectorLinearSearch<std::string>(depID, dependencies) != -1)
			throw LayoutCircularDependency("Circular dependency found in layout!");

		dependencies.push_back(depID);

		RecursiveVerify(depID, dependencies);
	}
}

void UILayout::RecursiveUpdate(const std::string& id)
{
	UIComponentData* data = GetComponentData(id);
	if (data == NULL)
		throw NonexistentComponentError("Couldn't find a component with the specified id!", id);

	for (std::string& depID : data->Dependencies)
		if (!GetComponentData(depID)->Updated)
			RecursiveUpdate(depID);

	data->Component->UpdateTransform(this, m_Width, m_Height);
	data->Updated = true;
}
