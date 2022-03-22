#include "UI/Parsers/Parsers.h"

#include <vector>
#include <stack>
#include <stdexcept>
#include <iostream>

// Token Parsing

constexpr auto LPAREN = '(';
constexpr auto RPAREN = ')';
constexpr auto EXPONENT = '^';
constexpr auto MULTIPLY = '*';
constexpr auto DIVIDE = '/';
constexpr auto PLUS = '+';
constexpr auto MINUS = '-';

constexpr auto DIGITS_BIN = "01";
constexpr auto DIGITS_DEC = ".0123456789";
constexpr auto DIGITS_HEX = "0123456789abcdefABCDEF";

constexpr auto PERIOD = '.';

enum class TokenType
{
	Number,
	Operator,
	LParenthesis,
	RParenthesis
};

enum class OperatorType
{
	Exponent,
	Multiply,
	Divide,
	Plus,
	Minus
};

struct Token
{
	Token(TokenType type) 
		: Type(type) {}

	TokenType Type;
};

struct NumToken : public Token
{
	NumToken(float value)
		: Token(TokenType::Number), Value(value) {}
	NumToken(Token* token)
		: Token(TokenType::Number)
	{
		NumToken tok = *static_cast<NumToken*>(token);
		Value = tok.Value;
	}

	float Value;
};

struct OpToken : public Token
{
	OpToken(OperatorType value)
		: Token(TokenType::Operator), Value(value) {}
	OpToken(Token* token)
		: Token(TokenType::Operator)
	{
		OpToken tok = *static_cast<OpToken*>(token);
		Value = tok.Value;
	}

	OperatorType Value;
};

std::vector<Token*> ParseTokens(const std::string& input)
{
	int32_t inputLength = static_cast<int>(input.length());

	int32_t cursor = 0;

	std::vector<Token*> tokens = std::vector<Token*>();
	do
	{
		char character = input[cursor];

		switch (character)
		{
		case ' ':
			cursor++;
			continue;
		case LPAREN:
			tokens.push_back(new Token(TokenType::LParenthesis));
			cursor++;
			continue;
		case RPAREN:
			tokens.push_back(new Token(TokenType::RParenthesis));
			cursor++;
			continue;
		case EXPONENT:
			tokens.push_back(new OpToken(OperatorType::Exponent));
			cursor++;
			continue;
		case MULTIPLY:
			tokens.push_back(new OpToken(OperatorType::Multiply));
			cursor++;
			continue;
		case DIVIDE:
			tokens.push_back(new OpToken(OperatorType::Divide));
			cursor++;
			continue;
		case PLUS:
			tokens.push_back(new OpToken(OperatorType::Plus));
			cursor++;
			continue;
		case MINUS:
			if ((cursor == 0 || input[cursor - 1] == ' ') && std::string(DIGITS_DEC).find(input[cursor + 1]) != std::string::npos)
				break;
			tokens.push_back(new OpToken(OperatorType::Minus));
			cursor++;
			continue;
		default:
			break;
		}

		if (cursor < inputLength - 1 && character == '0' && input[cursor + 1] == 'b')
		{	// Binary
			cursor += 2;
			character = input[cursor];

			std::string buffer = "";
			do
			{
				buffer += character;

				cursor++;
				if (!(cursor < inputLength))
					break;

				character = input[cursor];
			} while (std::string(DIGITS_BIN).find(character) != std::string::npos);

			if (buffer.length() > 0)
			{
				try { tokens.push_back(new NumToken(static_cast<float>(std::stoi(buffer, NULL, 2)))); }
				catch (std::invalid_argument) {}
				catch (std::out_of_range) {}
			}

			continue;
		}
		else if (cursor < inputLength - 1 && character == '0' && input[cursor + 1] == 'x')
		{	// Hexidecimal
			cursor += 2;
			character = input[cursor];

			std::string buffer = "";
			do
			{
				// Lowercase
				if (character >= 'A' && character <= 'Z')
					character += 32;

				buffer += character;

				cursor++;
				if (!(cursor < inputLength))
					break;

				character = input[cursor];
			} while (std::string(DIGITS_HEX).find(character) != std::string::npos);

			if (buffer.length() > 0)
			{
				try { tokens.push_back(new NumToken(static_cast<float>(std::stoi(buffer, NULL, 16)))); }
				catch (std::invalid_argument) {}
				catch (std::out_of_range) {}
			}

			continue;
		}
		else if (std::string(DIGITS_DEC).find(character) != std::string::npos || character == MINUS)
		{	// Decimal
			int periodCount = 0;
			std::string buffer = "";
			do
			{
				if (character == PERIOD)
					periodCount++;

				if (periodCount > 1)
					throw NumberParsingError("Invalid period found while parsing input!", cursor);

				buffer += character;

				cursor++;
				if (!(cursor < inputLength))
					break;

				character = input[cursor];
			} while (std::string(DIGITS_DEC).find(character) != std::string::npos);

			if (buffer.length() > 0)
			{
				try { tokens.push_back(new NumToken(std::stof(buffer))); }
				catch (std::invalid_argument) {}
				catch (std::out_of_range) {}
			}

			continue;
		}

		throw NumberParsingError("Invalid character found while parsing input!", cursor);
	} while (cursor < inputLength);

	return tokens;
}

void deleteTokens(const std::vector<Token*>& tokens)
{
	for (Token* token : tokens)
		delete token;
}

// Shunting yard algorithm

int getPrecedence(OperatorType type)
{
	switch (type)
	{
	case OperatorType::Exponent:
		return 4;
	case OperatorType::Multiply:
		return 3;
	case OperatorType::Divide:
		return 3;
	case OperatorType::Plus:
		return 2;
	case OperatorType::Minus:
		return 2;
	default:
		return -1;
	}
}

bool isLeftAssociative(OperatorType type)
{
	switch (type)
	{
	case OperatorType::Exponent:
		return false;
	case OperatorType::Multiply:
		return true;
	case OperatorType::Divide:
		return true;
	case OperatorType::Plus:
		return true;
	case OperatorType::Minus:
		return true;
	default:
		return false;
	}
}

std::vector<Token*> InfixToPostfix(const std::vector<Token*>& tokens)
{
	std::stack<Token*> stack = std::stack<Token*>();

	std::vector<Token*> output = std::vector<Token*>();

	for (Token* token : tokens)
	{
		switch (token->Type)
		{
		case TokenType::Number:
			output.push_back(token);
			break;
		case TokenType::Operator:
		{
			const OperatorType o1 = OpToken(token).Value;

			while (!stack.empty())
			{
				const OperatorType o2 = OpToken(stack.top()).Value;

				if ((isLeftAssociative(o1) && getPrecedence(o1) <= getPrecedence(o2)) ||
					(!isLeftAssociative(o1) && getPrecedence(o1) < getPrecedence(o2)))
				{
					output.push_back(stack.top());
					stack.pop();
					continue;
				}

				break;
			}
			stack.push(token);
			break;
		}
		case TokenType::LParenthesis:
			stack.push(token);
			break;
		case TokenType::RParenthesis:
			while (!stack.empty() && stack.top()->Type != TokenType::LParenthesis)
			{
				output.push_back(stack.top());
				stack.pop();
			}
			stack.pop();
			break;
		default:
			break;
		}
	}

	while (!stack.empty())
	{
		if (stack.top()->Type == TokenType::LParenthesis)
			throw NumberEvaluationError("Mismatched parentheses found while evaluating infix expression!");

		output.push_back(stack.top());
		stack.pop();
	}

	return output;
}

// Postfix notation evaluation

float EvaluatePostfix(const std::vector<Token*>& tokens)
{
	std::stack<float> stack = std::stack<float>();

	for (Token* token : tokens)
	{
		switch (token->Type)
		{
		case TokenType::Number:
			stack.push(NumToken(token).Value);
			break;
		case TokenType::Operator:
		{
			if (stack.size() < 2)
				throw NumberEvaluationError("Failed to evaluate postfix expression!");

			float num2 = NumToken(stack.top()).Value;
			stack.pop();
			float num1 = NumToken(stack.top()).Value;
			stack.pop();

			switch (OpToken(token).Value)
			{
			case OperatorType::Exponent:
				stack.push(std::powf(num1, num2));
				break;
			case OperatorType::Multiply:
				stack.push(num1 * num2);
				break;
			case OperatorType::Divide:
				stack.push(num1 / num2);
				break;
			case OperatorType::Plus:
				stack.push(num1 + num2);
				break;
			case OperatorType::Minus:
				stack.push(num1 - num2);
				break;
			default:
				break;
			}
			break;
		}
		case TokenType::LParenthesis:
			throw NumberEvaluationError("Mismatched parentheses found while evaluating postfix expression!");
		case TokenType::RParenthesis:
			throw NumberEvaluationError("Mismatched parentheses found while evaluating postfix expression!");
		default:
			throw NumberEvaluationError("Invalid token found while evaluating postfix expression!");
		};
	}
	
	if (stack.size() == 1)
		return stack.top();
	else if (stack.size() == 0)
		return 0.0f;
	else
		throw NumberEvaluationError("Invalid postfix expression found during evaluation!");
}

float NumberParser::Parse(std::string input)
{
	std::vector<Token*> tokens = std::vector<Token*>();

	try
	{
		tokens = ParseTokens(input);
		std::vector<Token*> postfix = InfixToPostfix(tokens);
		float value = EvaluatePostfix(postfix);

		deleteTokens(tokens);

		return value;
	}
	catch (const NumberParsingError& e)
	{
		deleteTokens(tokens);
		throw e;
	}
	catch (const NumberEvaluationError& e)
	{
		deleteTokens(tokens);
		throw e;
	}
}

bool NumberParser::ParseColor(std::string input, glm::vec3& color)
{
	if (input[0] == '#')
	{
		color = glm::vec3(
			static_cast<float>(std::stoi(input.substr(1, 2), NULL, 16)) / 255.0f,
			static_cast<float>(std::stoi(input.substr(3, 2), NULL, 16)) / 255.0f,
			static_cast<float>(std::stoi(input.substr(5, 2), NULL, 16)) / 255.0f
		);
		return true;
	}
	else 
		return false;
}

bool NumberParser::ParseColor(std::string input, glm::vec4& color)
{
	if (input[0] == '#')
	{
		color = glm::vec4(
			static_cast<float>(std::stoi(input.substr(1, 2), NULL, 16)) / 255.0f,
			static_cast<float>(std::stoi(input.substr(3, 2), NULL, 16)) / 255.0f,
			static_cast<float>(std::stoi(input.substr(5, 2), NULL, 16)) / 255.0f,
			static_cast<float>(std::stoi(input.substr(7, 2), NULL, 16)) / 255.0f
		);
		return true;
	}
	else
		return false;
}