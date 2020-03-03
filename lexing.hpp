#pragma once

#include <vector>
#include <array>
#include <string>

namespace lexing
{
	enum class type
	{
		COMMENT,
		NUMERIC_LITERAL,
		IDENTIFIER,
		OPERATOR,
		UNARY_OPERATOR,
		KEYWORD,
		BRACKET,
		SYNTATIC_ELEMENT,
	};

	/**
	 * @brief Splits a source code string into tokens
	 * @param source Source code to parse into tokens
	 * @return A vector of tokens
	 */
	std::vector<std::pair<type, std::string>> get_tokens(const std::string& source);
}
