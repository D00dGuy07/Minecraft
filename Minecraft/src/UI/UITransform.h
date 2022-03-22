#pragma once

enum class UIAxis
{
	X,
	Y
};

enum class DockPosition
{
	Top,
	Bottom,
	Left,
	Right
};

struct UITransform
{
	int Width;
	int Height;
	int PosX;
	int PosY;
};

struct UIRect
{
	int TopY;
	int BottomY;
	int LeftX;
	int RightX;
};

UIRect ToRect(const UITransform& t);

UITransform ToTransform(const UIRect& r);