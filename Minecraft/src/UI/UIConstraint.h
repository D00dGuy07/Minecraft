#pragma once

#include "UI/UIDock.h"
#include "UI/UITransform.h"

class UIComponent;
class UILayout;

struct UIConstraint
{
public:
	virtual int GetValue(UIComponent* parent, UILayout* layout, DockPosition dock, UIRect rect) = 0;
	virtual void GetDependencies(std::vector<std::string>& list) {}

	bool Resize = false;
};

struct SpringConstraint : public UIConstraint
{
public:
	SpringConstraint(UIDock* dock)
		: m_Dock(dock) {}

	~SpringConstraint();

	virtual int GetValue(UIComponent* parent, UILayout* layout, DockPosition dock, UIRect rect) override;
	virtual void GetDependencies(std::vector<std::string>& list) override;
private:
	UIDock* m_Dock;
};