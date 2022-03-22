#include "UI/Parsers/Parsers.h"

#include "UI/UILayout.h"

UILayout* LayoutParser::Parse(const pugi::xml_node& node)
{
	UILayout* layout = new UILayout();

	for (auto childNode : node.children())
		layout->AddComponent(ComponentParser::Parse(childNode));

	return layout;
}