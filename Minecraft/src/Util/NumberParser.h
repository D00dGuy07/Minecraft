#pragma once

#include <string>

#include "glm/glm.hpp"

// Exceptions

class NumberParsingError : public std::exception
{
public:
	int Cursor;

	NumberParsingError(const std::string& message, int cursor)
		: std::exception(message.c_str()), Cursor(cursor) {}
};

class NumberEvaluationError : public std::exception
{
public:
	NumberEvaluationError(const std::string& message)
		: std::exception(message.c_str()) {}
};

// Parser

namespace NumberParser 
{
	float Parse(std::string input);

	// Hex code expected
	bool ParseColor(std::string input, glm::vec3& color);
	bool ParseColor(std::string input, glm::vec4& color);
};