#pragma once

#include "UI/UITransform.h"

#include <vector>
#include <string>

class UILayout;

struct UIDock
{
public:
	virtual int GetValue(UILayout* layout, UIAxis axis) = 0;
	virtual void GetDependency(std::vector<std::string>& list) {}
};

struct ComponentDock : public UIDock
{
public:
	ComponentDock(std::string componentID, DockPosition dockSide);

	virtual int GetValue(UILayout* layout, UIAxis axis) override;
	virtual void GetDependency(std::vector<std::string>& list) override;
private:
	DockPosition m_DockSide;
	std::string m_ComponentID;
};

struct WindowDock : public UIDock
{
public:
	WindowDock(DockPosition dockSide);

	virtual int GetValue(UILayout* layout, UIAxis axis) override;
private:
	DockPosition m_DockSide;
};