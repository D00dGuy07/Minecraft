#pragma once

#include <vector>

// Returns -1 if not found
template<typename t>
int VectorLinearSearch(const t& key, const std::vector<t>& data)
{
	for (t i : data)
		if (i == key)
			return static_cast<int>(&i - &data[0]);
	return -1;
}