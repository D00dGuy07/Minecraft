#include "Vec3Hash.h"

bool operator==(const Vec3Hash& left, const Vec3Hash& right)
{
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}