#include "UI/Parsers/Parsers.h"

#include "UI/UISize.h"

#include "Util/XMLUtil.h"

UISize* SizeParser::Parse(const pugi::xml_node& node)
{
	std::string name = node.name();
	if (name == "ScaleSize")
		return ParseScaleSize(node);
	else if (name == "PixelSize")
		return ParsePixelSize(node);
	else if (name == "AspectSize")
		return ParseAspectSize(node);
	else
		throw UnknownObjectException("Encountered unknown object while parsing UISize!", "UISize", name);
}

ScaleSize* SizeParser::ParseScaleSize(const pugi::xml_node& node)
{
	return ParseSize<ScaleSize>(node);
}

PixelSize* SizeParser::ParsePixelSize(const pugi::xml_node& node)
{
	return ParseSize<PixelSize>(node);
}

AspectSize* SizeParser::ParseAspectSize(const pugi::xml_node& node)
{
	return ParseSize<AspectSize>(node);
}