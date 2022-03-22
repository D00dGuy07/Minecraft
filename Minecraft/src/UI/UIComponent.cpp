#include "UI/UIComponent.h"

#include "UI/UILayout.h"
#include "UI/UISize.h"
#include "UI/UIConstraint.h"

#include <iostream>

UIComponent::UIComponent(std::string id)
	: m_ID(id) {}

UIComponent::~UIComponent()
{
	delete m_WidthSize;
	delete m_HeightSize;

	if (m_TopConstraint != NULL)
		delete m_TopConstraint;
	if (m_BottomConstraint != NULL)
		delete m_BottomConstraint;
	if (m_LeftConstraint != NULL)
		delete m_LeftConstraint;
	if (m_RightConstraint != NULL)
		delete m_RightConstraint;
}

void UIComponent::UpdateTransform(UILayout* layout, int width, int height)
{
	if (m_WidthSize != NULL && m_HeightSize != NULL && m_WidthSize->IsDependent == true)
	{
		m_ActualTransform.Height = m_HeightSize->GetValue(this, UIAxis::Y, layout->GetWidth(), layout->GetHeight());
		m_ActualTransform.Width = m_WidthSize->GetValue(this, UIAxis::X, layout->GetWidth(), layout->GetHeight());
	}
	else if (m_WidthSize != NULL && m_HeightSize != NULL && m_HeightSize->IsDependent == true)
	{
		m_ActualTransform.Width = m_WidthSize->GetValue(this, UIAxis::X, layout->GetWidth(), layout->GetHeight());
		m_ActualTransform.Height = m_HeightSize->GetValue(this, UIAxis::Y, layout->GetWidth(), layout->GetHeight());
	}
	else if (m_WidthSize != NULL && m_HeightSize != NULL)
	{
		m_ActualTransform.Width = m_WidthSize->GetValue(this, UIAxis::X, layout->GetWidth(), layout->GetHeight());
		m_ActualTransform.Height = m_HeightSize->GetValue(this, UIAxis::Y, layout->GetWidth(), layout->GetHeight());
	}
	else if (m_WidthSize != NULL && m_HeightSize == NULL)
		m_ActualTransform.Width = m_WidthSize->GetValue(this, UIAxis::X, layout->GetWidth(), layout->GetHeight());
	else if (m_WidthSize == NULL && m_HeightSize != NULL)
		m_ActualTransform.Height = m_HeightSize->GetValue(this, UIAxis::Y, layout->GetWidth(), layout->GetHeight());

	UIRect rect = ToRect(m_ActualTransform);

	if (m_LeftConstraint != NULL && m_RightConstraint != NULL)
	{
		int leftOffset = m_LeftConstraint->GetValue(this, layout, DockPosition::Left, rect);
		int rightOffset = m_RightConstraint->GetValue(this, layout, DockPosition::Right, rect);
		int leftInterpolated = static_cast<int>((1 - m_HorizontalBias) * (rect.LeftX + leftOffset) + m_HorizontalBias * (rect.LeftX + rightOffset));
		int rightInterpolated = static_cast<int>((1 - m_HorizontalBias) * (rect.RightX + leftOffset) + m_HorizontalBias * (rect.RightX + rightOffset));

		if (m_LeftConstraint->Resize)
			rect.LeftX += leftOffset;
		else
			rect.LeftX = leftInterpolated;

		if (m_RightConstraint->Resize)
			rect.RightX += rightOffset;
		else
			rect.RightX = rightInterpolated;
	}
	else if (m_LeftConstraint != NULL && m_RightConstraint == NULL)
	{
		int leftOffset = m_LeftConstraint->GetValue(this, layout, DockPosition::Left, rect);

		rect.LeftX += leftOffset;

		if (!m_LeftConstraint->Resize)
			rect.RightX += leftOffset;
	}
	else if (m_LeftConstraint == NULL && m_RightConstraint != NULL)
	{
		int rightOffset = m_RightConstraint->GetValue(this, layout, DockPosition::Right, rect);

		rect.RightX += rightOffset;

		if (!m_RightConstraint->Resize)
			rect.LeftX += rightOffset;
	}

	if (m_TopConstraint != NULL && m_BottomConstraint != NULL)
	{
		int topOffset = m_TopConstraint->GetValue(this, layout, DockPosition::Top, rect);
		int bottomOffset = m_BottomConstraint->GetValue(this, layout, DockPosition::Bottom, rect);
		int topInterpolated = static_cast<int>((1 - m_VerticalBias) * (rect.TopY + topOffset) + m_VerticalBias * (rect.TopY + bottomOffset));
		int bottomInterpolated = static_cast<int>((1 - m_VerticalBias) * (rect.BottomY + topOffset) + m_VerticalBias * (rect.BottomY + bottomOffset));

		if (m_TopConstraint->Resize)
			rect.TopY += topOffset;
		else
			rect.TopY = topInterpolated;

		if (m_BottomConstraint->Resize)
			rect.BottomY += bottomOffset;
		else
			rect.BottomY = bottomInterpolated;
	}
	else if (m_TopConstraint != NULL && m_BottomConstraint == NULL)
	{
		int topOffset = m_TopConstraint->GetValue(this, layout, DockPosition::Top, rect);

		rect.TopY += topOffset;

		if (!m_TopConstraint->Resize)
			rect.BottomY += topOffset;
	}
	else if (m_TopConstraint == NULL && m_BottomConstraint != NULL)
	{
		int bottomOffset = m_BottomConstraint->GetValue(this, layout, DockPosition::Bottom, rect);

		rect.BottomY += bottomOffset;

		if (!m_BottomConstraint->Resize)
			rect.TopY += bottomOffset;
	}

	m_ActualTransform = ToTransform(rect);

	OnResized();

	m_Updated = true;
}

void UIComponent::GetDependencies(std::vector<std::string>& list)
{
	if (m_TopConstraint != NULL)
		m_TopConstraint->GetDependencies(list);
	if (m_BottomConstraint != NULL)
		m_BottomConstraint->GetDependencies(list);
	if (m_LeftConstraint != NULL)
		m_LeftConstraint->GetDependencies(list);
	if (m_RightConstraint != NULL)
		m_RightConstraint->GetDependencies(list);
}


void UIComponent::SetWidth(UISize* size)
{
	if (m_WidthSize != NULL)
		delete m_WidthSize;
	m_WidthSize = size;
	m_Updated = true;
}

void UIComponent::SetHeight(UISize* size)
{
	if (m_HeightSize != NULL)
		delete m_HeightSize;
	m_HeightSize = size;
	m_Updated = true;
}


void UIComponent::SetTopConstraint(UIConstraint* constraint)
{
	if (m_TopConstraint != NULL)
		delete m_TopConstraint;
	m_TopConstraint = constraint;
	m_Updated = true;
}

void UIComponent::SetBottomConstraint(UIConstraint* constraint)
{
	if (m_BottomConstraint != NULL)
		delete m_BottomConstraint;
	m_BottomConstraint = constraint;
	m_Updated = true;
}

void UIComponent::SetLeftConstraint(UIConstraint* constraint)
{
	if (m_LeftConstraint != NULL)
		delete m_LeftConstraint;
	m_LeftConstraint = constraint;
	m_Updated = true;
}

void UIComponent::SetRightConstraint(UIConstraint* constraint)
{
	if (m_RightConstraint != NULL)
		delete m_RightConstraint;
	m_RightConstraint = constraint;
	m_Updated = true;
}

void UIComponent::SetZIndex(int32_t zIndex)
{
	m_ZIndex = zIndex;
	m_Updated = true;
}