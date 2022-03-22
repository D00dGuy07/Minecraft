#pragma once

#include <unordered_map>
#include <string>

class UILayout;

class UILoader
{
public:
	static void LoadFile(const std::string& path);
	static std::unordered_map<std::string, UILayout*> LoadLayouts(const std::string& path);
};