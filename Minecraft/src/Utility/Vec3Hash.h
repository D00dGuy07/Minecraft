#pragma once

#include <functional>
#include <boost/functional/hash.hpp>

#include "glm/glm.hpp"

struct Vec3Hash
{
	Vec3Hash(int X, int Y, int Z)
		: x(X), y(Y), z(Z){}

	Vec3Hash(glm::vec3 coords)
		: x((int)coords.x), y((int)coords.y), z((int)coords.z) {}

	int x;
	int y;
	int z;
};

bool operator==(const Vec3Hash& left, const Vec3Hash& right);

namespace std {
	template <>
	struct hash<Vec3Hash>
	{
		std::size_t operator()(const Vec3Hash& k) const
		{
			std::size_t seed = 0;

			boost::hash_combine(seed, boost::hash_value(k.x));
			boost::hash_combine(seed, boost::hash_value(k.y));
			boost::hash_combine(seed, boost::hash_value(k.z));

			return seed;
		}
	};
}