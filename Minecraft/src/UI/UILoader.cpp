#include "UI/UILoader.h"

#include "UI/Parsers/Parsers.h"

#include "Util/NumberParser.h"
#include "Util/XMLUtil.h"

#include "pugixml/pugixml.hpp"

#include <iostream>

void UILoader::LoadFile(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());
	if (!result)
	{
		std::cout << "Failed to load xml layout '" << path << "'\n";
		return;
	}

	pugi::xml_node root = doc.first_child();
	for (pugi::xml_node node : root.children())
		std::cout << node.attribute("Name").value() << "\n";;
}

std::unordered_map<std::string, UILayout*> UILoader::LoadLayouts(const std::string& path)
{
	auto layouts = std::unordered_map<std::string, UILayout*>();

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());
	if (!result)
	{
		std::cout << "Failed to load xml file '" << path << "'\n";
		return layouts;
	}

	std::string currentLayout;
	try
	{
		for (auto node : doc.children())
		{
			currentLayout = node.name();
			layouts[currentLayout] = LayoutParser::Parse(node);
		}
	}
	catch (std::exception e)
	{
		std::cout << "Failed to load layout '" << currentLayout << "': " << e.what() << "\n";
	}

	return layouts;
}
