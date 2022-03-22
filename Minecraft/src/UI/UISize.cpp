#include "UI/UISize.h"

#include "UI/UIComponent.h"

// ScaleSize

ScaleSize::ScaleSize(float scale)
	: m_Scale(scale)
{
	IsDependent = false;
}

int ScaleSize::GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY)
{
	switch (axis)
	{
	case UIAxis::X:
		return static_cast<int>((windowX - 1) * m_Scale);
	case UIAxis::Y:
		return static_cast<int>((windowY - 1) * m_Scale);
	default:
		return 0;
	}
}

// PixelSize

PixelSize::PixelSize(float size)
	: m_Size(size)
{
	IsDependent = false;
}
int PixelSize::GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY)
{
	return m_Size;
}

// AspectSize

AspectSize::AspectSize(float ratio)
	: m_Ratio(ratio)
{
	IsDependent = true;
}
int AspectSize::GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY)
{
	switch (axis)
	{
	case UIAxis::X:
		return static_cast<int>(m_Ratio * parent->GetActualTransform().Height);
	case UIAxis::Y:
		return static_cast<int>(parent->GetActualTransform().Width / m_Ratio);
	default:
		return 0;
	}
}