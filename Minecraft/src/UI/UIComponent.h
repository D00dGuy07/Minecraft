#pragma once

#include "UI/UITransform.h"

#include <string>
#include <vector>

struct UISize;
struct UIConstraint;
class UILayout;
class UIRenderer;

class UIComponent
{
public:
	UIComponent(std::string id);
	~UIComponent();

	void UpdateTransform(UILayout* layout, int width, int height);
	void GetDependencies(std::vector<std::string>& list);

	void SetWidth(UISize* size);
	void SetHeight(UISize* size);

	void SetHorizontalBias(float bias) { m_HorizontalBias = bias; }
	void SetVerticalBias(float bias) { m_VerticalBias = bias; }

	void SetTopConstraint(UIConstraint* constraint);
	void SetBottomConstraint(UIConstraint* constraint);
	void SetLeftConstraint(UIConstraint* constraint);
	void SetRightConstraint(UIConstraint* constraint);

	inline int32_t GetZIndex() const { return m_ZIndex; }
	void SetZIndex(int32_t zIndex);

	inline std::string GetID() const { return m_ID; }

	inline UITransform GetActualTransform() const { return m_ActualTransform; }
protected:

	std::string m_ID;

	UISize* m_WidthSize = NULL;
	UISize* m_HeightSize = NULL;

	UIConstraint* m_TopConstraint = NULL;
	UIConstraint* m_BottomConstraint = NULL;
	UIConstraint* m_LeftConstraint = NULL;
	UIConstraint* m_RightConstraint = NULL;

	float m_HorizontalBias = 0.5;
	float m_VerticalBias = 0.5;

	UITransform m_ActualTransform = { 0, 0, 0, 0 };

	int32_t m_ZIndex = 0;

	bool m_Updated = false;

	virtual void OnResized() {}

	virtual void AddRenderInstances(UIRenderer* renderer) = 0;

	friend class UILayout;
};