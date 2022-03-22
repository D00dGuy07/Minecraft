#include "UI/Parsers/Parsers.h"

#include "UI/UISize.h"
#include "UI/UIConstraint.h"
#include "UI/UIComponent.h"
#include "UI/Components/Frame.h"

UIComponent* ComponentParser::Parse(const pugi::xml_node& node)
{
	std::string name = node.name();
	if (name == "Frame")
		return ParseFrame(node);
	else
		throw UnknownObjectException("Encountered unknown object while parsing UIComponent!", "UIComponent", name);
}

Frame* ComponentParser::ParseFrame(const pugi::xml_node& node)
{
	Frame* frame = ParseComponent<Frame>(node);

	// Parse color and alpha
	const char* transparency = node.attribute("Alpha").as_string(NULL);
	const char* fill = node.attribute("Fill").as_string(NULL);
	if (fill != NULL)
	{
		float alpha = 1.0f;
		if (transparency != NULL)
			alpha = NumberParser::Parse(transparency);

		glm::vec3 rgb(0.2f, 0.2f, 0.2f);
		NumberParser::ParseColor(fill, rgb);
		frame->SetFill(glm::vec4(rgb.x, rgb.y, rgb.z, alpha));
	}

	// Roundness
	const char* roundness = node.attribute("Roundness").as_string(NULL);
	if (roundness != NULL)
		frame->SetRoundness(NumberParser::Parse(roundness));

	return frame;
}

void ComponentParser::FillComponent(UIComponent* component, const pugi::xml_node& node)
{
	// Size
	{
		pugi::xml_node sizes = XMLRequiredChild(node, "Size");

		bool widthSet = false;
		bool heightSet = false;
		for (auto childNode : sizes.children())
		{
			std::string type = XMLRequiredAttribute(childNode, "Type");

			UISize* size = SizeParser::Parse(childNode); // Parse first to catch other typos before type value
			if (type == "Width")
			{
				component->SetWidth(size);
				widthSet = true;
			}
			else if (type == "Height")
			{
				component->SetHeight(size);
				heightSet = true;
			}
			else
			{
				delete size;
				throw InvalidObjectException("Invalid type value for size!", childNode.name(), childNode);
			}
		}

		if (!widthSet || !heightSet)
			throw InvalidObjectException("Components require width and height values!", "UIComponent", node);
	}

	// Constraints
	{
		pugi::xml_node constraints = node.child("Constraints"); // Not using XMLRequiredChild() because it should work without any constraints

		if (!constraints.empty())
		{
			for (auto childNode : constraints.children())
			{
				DockPosition side = EnumParser::ParseDockPosition(XMLRequiredAttribute(childNode, "Side"));

				UIConstraint* constraint = ConstraintParser::Parse(childNode); // Parse first to catch other typos before side value
				switch (side)
				{
				case DockPosition::Top:
					component->SetTopConstraint(constraint);
					break;
				case DockPosition::Bottom:
					component->SetBottomConstraint(constraint);
					break;
				case DockPosition::Left:
					component->SetLeftConstraint(constraint);
					break;
				case DockPosition::Right:
					component->SetRightConstraint(constraint);
					break;
				default:
					delete constraint;
					throw InvalidObjectException("Invalid side value for constraint!", childNode.name(), childNode);
				}
			}
		}
	}

	// Horizontal and Vertical bias
	{
		const char* hBias = node.attribute("HorizontalBias").as_string(NULL);
		if (hBias != NULL)
			component->SetHorizontalBias(NumberParser::Parse(hBias));

		const char* vBias = node.attribute("VerticalBias").as_string(NULL);
		if (vBias != NULL)
			component->SetVerticalBias(NumberParser::Parse(vBias));
	}

	// ZIndex
	{
		const char* zIndex = node.attribute("ZIndex").as_string(NULL);
		if (zIndex != NULL)
			component->SetZIndex(static_cast<int32_t>(NumberParser::Parse(zIndex)));
	}
}
