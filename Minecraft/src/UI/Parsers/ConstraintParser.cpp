#include "UI/Parsers/Parsers.h"

#include "UI/UIConstraint.h"

#include "Util/XMLUtil.h"

UIConstraint* ConstraintParser::Parse(const pugi::xml_node& node)
{
	std::string name = node.name();
	if (name == "SpringConstraint")
		return ParseSpringConstraint(node);
	else
		throw UnknownObjectException("Encountered unknown object while parsing UIConstraint!", "UIConstraint", name);
}

SpringConstraint* ConstraintParser::ParseSpringConstraint(const pugi::xml_node& node)
{
	std::string target = XMLRequiredAttribute(node, "Target");
	std::string targetSide = XMLRequiredAttribute(node, "TargetSide");

	SpringConstraint* constraint;
	if (target == "Window")
		constraint = new SpringConstraint(new WindowDock(EnumParser::ParseDockPosition(targetSide)));
	else
		constraint = new SpringConstraint(new ComponentDock(target, EnumParser::ParseDockPosition(targetSide)));

	FillComponent(constraint, node);
	return constraint;
}

void ConstraintParser::FillComponent(UIConstraint* constraint, const pugi::xml_node& node)
{
	const char* resize = node.attribute("Resize").as_string(NULL);
	if (resize != NULL)
		constraint->Resize = BoolParser::Parse(resize);
}