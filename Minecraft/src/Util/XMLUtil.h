#pragma once

#include <string>

#include "pugixml/pugixml.hpp"

// Exceptions

class MissingAttributeException : public std::exception
{
public:
	MissingAttributeException(const std::string& message, const std::string& attributeName)
		: std::exception(message.c_str()), AttributeName(attributeName) {}

	const std::string AttributeName;
};

class MissingChildException : public std::exception
{
public:
	MissingChildException(const std::string& message, const std::string& childName)
		: std::exception(message.c_str()), ChildName(childName) {}

	const std::string ChildName;
};

// Classes

class BoolParser
{
public:
	static bool Parse(const std::string& input);
};

// Functions

std::string XMLRequiredAttribute(const pugi::xml_node& node, const std::string& name);
pugi::xml_node XMLRequiredChild(const pugi::xml_node& node, const std::string& name);