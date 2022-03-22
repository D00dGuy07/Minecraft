#pragma once

#include "pugixml/pugixml.hpp"
#include <string>

#include "UI/UITransform.h"

#include "Util/XMLUtil.h"
#include "Util/NumberParser.h"

// Forward declaration
struct UISize;
struct ScaleSize;
struct PixelSize;
struct AspectSize;

struct UIConstraint;
struct SpringConstraint;

class UIComponent;
class Frame;

class UILayout;


// Exceptions

class InvalidEnumException : public std::exception
{
public:
	InvalidEnumException(const std::string& message, const std::string& found)
		: std::exception(message.c_str()), Found(found) {}

	const std::string Found;
};

class UnknownObjectException : public std::exception
{
public:
	UnknownObjectException(const std::string& message, const std::string& typeName, const std::string& found)
		: std::exception(message.c_str()), TypeName(typeName), Found(found) {}

	const std::string TypeName;
	const std::string Found;
};

class InvalidObjectException : public std::exception
{
public:
	InvalidObjectException(const std::string& message, const std::string& typeName, const pugi::xml_node& node)
		: std::exception(message.c_str()), TypeName(typeName), Node(node) {}

	const std::string TypeName;
	const pugi::xml_node Node;
};

// Parsers

class EnumParser
{
public:
	static DockPosition ParseDockPosition(const std::string& input);
	static UIAxis ParseUIAxis(const std::string& input);
};


class ConstraintParser
{
public:
	static UIConstraint* Parse(const pugi::xml_node& node);

	static SpringConstraint* ParseSpringConstraint(const pugi::xml_node& node);

private:
	static void FillComponent(UIConstraint* constraint, const pugi::xml_node& node);
};


class SizeParser
{
public:
	static UISize* Parse(const pugi::xml_node& node);

	template<typename T>
	static T* ParseSize(const pugi::xml_node& node)
	{
		return new T(NumberParser::Parse(XMLRequiredAttribute(node, "Value")));
	}

	static ScaleSize* ParseScaleSize(const pugi::xml_node& node);
	static PixelSize* ParsePixelSize(const pugi::xml_node& node);
	static AspectSize* ParseAspectSize(const pugi::xml_node& node);
};


class ComponentParser
{
public:
	static UIComponent* Parse(const pugi::xml_node& node);

	template<typename T>
	static T* ParseComponent(const pugi::xml_node& node)
	{
		std::string id = XMLRequiredAttribute(node, "ID");
		T* component = new T(id);
		FillComponent(component, node);

		return component;
	}

	static Frame* ParseFrame(const pugi::xml_node& node);

private:
	static void FillComponent(UIComponent* component, const pugi::xml_node& node);
};

class LayoutParser
{
public:
	static UILayout* Parse(const pugi::xml_node& node);
};