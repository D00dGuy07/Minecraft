#include "UI/UITransform.h"


UIRect ToRect(const UITransform& t)
{
	return {
		t.PosY - t.Height / 2,
		t.PosY + t.Height / 2,
		t.PosX - t.Width / 2,
		t.PosX + t.Width / 2
	};
}

UITransform ToTransform(const UIRect& r)
{
	return {
		r.RightX - r.LeftX,
		r.BottomY - r.TopY,
		r.LeftX + (r.RightX - r.LeftX) / 2,
		r.TopY + (r.BottomY - r.TopY) / 2
	};
}
