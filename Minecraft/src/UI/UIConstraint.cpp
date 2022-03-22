#include "UI/UIConstraint.h"

SpringConstraint::~SpringConstraint()
{
	delete m_Dock;
}

int SpringConstraint::GetValue(UIComponent* parent, UILayout* layout, DockPosition dock, UIRect rect)
{
	switch (dock)
	{
	case DockPosition::Top:
		return m_Dock->GetValue(layout, UIAxis::Y) - rect.TopY;
	case DockPosition::Bottom:
		return m_Dock->GetValue(layout, UIAxis::Y) - rect.BottomY;
	case DockPosition::Left:
		return m_Dock->GetValue(layout, UIAxis::X) - rect.LeftX;
	case DockPosition::Right:
		return m_Dock->GetValue(layout, UIAxis::X) - rect.RightX;

	default:
		return 0;
	}
}

void SpringConstraint::GetDependencies(std::vector<std::string>& list)
{
	m_Dock->GetDependency(list);
}
