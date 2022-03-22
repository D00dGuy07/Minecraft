#include "UI/UIDock.h"

#include "UI/UILayout.h"
#include "UI/UIComponent.h"

// ComponentDock

ComponentDock::ComponentDock(std::string componentID, DockPosition dockSide)
	: m_ComponentID(componentID), m_DockSide(dockSide) {}

int ComponentDock::GetValue(UILayout* layout, UIAxis axis)
{
	UIComponent* component = layout->GetComponent(m_ComponentID);
	if (component == NULL) { return 0; }
	UIRect rect = ToRect(component->GetActualTransform());

	switch (axis)
	{
	case UIAxis::X:
		switch (m_DockSide)
		{
		case DockPosition::Left:
			return rect.LeftX;
		case DockPosition::Right:
			return rect.RightX;
		default:
			return 0;
		}
	case UIAxis::Y:
		switch (m_DockSide)
		{
		case DockPosition::Top:
			return rect.TopY;
		case DockPosition::Bottom:
			return rect.BottomY;
		default:
			return 0;
		}
	default:
		return 0;
	}
}

void ComponentDock::GetDependency(std::vector<std::string>& list)
{
	list.push_back(m_ComponentID);
}

// WindowDock

WindowDock::WindowDock(DockPosition dockSide)
	: m_DockSide(dockSide) {}

int WindowDock::GetValue(UILayout* layout, UIAxis axis)
{
	switch (axis)
	{
	case UIAxis::X:
		switch (m_DockSide)
		{
		case DockPosition::Left:
			return 0;
		case DockPosition::Right:
			return layout->GetWidth() - 1;
		}
	case UIAxis::Y:
		switch (m_DockSide)
		{
		case DockPosition::Top:
			return 0;
		case DockPosition::Bottom:
			return layout->GetHeight() - 1;
		}
	default:
		return 0;
	}
}