#include "UI/Parsers/Parsers.h"

DockPosition EnumParser::ParseDockPosition(const std::string& input)
{
	if (input == "Top")
		return DockPosition::Top;
	else if (input == "Bottom")
		return DockPosition::Bottom;
	else if (input == "Left")
		return DockPosition::Left;
	else if (input == "Right")
		return DockPosition::Right;
	else
		throw InvalidEnumException("Invalid input while parsing DockPosition", input);
}

UIAxis EnumParser::ParseUIAxis(const std::string& input)
{
	if (input == "X")
		return UIAxis::X;
	else if (input == "Y")
		return UIAxis::Y;
	else
		throw InvalidEnumException("Invalid input while parsing UIAxis", input);
}