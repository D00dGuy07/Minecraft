#include "XMLUtil.h"

#include "boost/algorithm/string.hpp"

bool BoolParser::Parse(const std::string& input)
{
	const std::string lowerCase = boost::algorithm::to_lower_copy(input);
	if (lowerCase == "true" || input == "1")
		return true;
	return false;
}

std::string XMLRequiredAttribute(const pugi::xml_node& node, const std::string& name)
{
	const char* result = node.attribute(name.c_str()).as_string(NULL);

	if (result == NULL)
		throw MissingAttributeException("Couldn't find required attribute!", name);

	return result;
}

pugi::xml_node XMLRequiredChild(const pugi::xml_node& node, const std::string& name)
{
	pugi::xml_node result = node.child(name.c_str());

	if (result.empty())
		throw MissingChildException("Couldn't find required child!", name);

	return result;
}
