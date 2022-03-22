#pragma once

#include "UI/UITransform.h"

class UIComponent;

struct UISize
{
public:
	virtual int GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY) = 0;

	bool IsDependent;
};

struct ScaleSize : public UISize
{
public:
	ScaleSize(float scale);

	virtual int GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY) override;
private:
	float m_Scale;
};

struct PixelSize : public UISize
{
public:
	PixelSize(float size);

	virtual int GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY) override;
private:
	float m_Size;
};

struct AspectSize : public UISize
{
public:
	AspectSize(float ratio);

	virtual int GetValue(UIComponent* parent, UIAxis axis, int windowX, int windowY) override;
private:
	float m_Ratio;
};