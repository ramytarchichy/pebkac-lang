#pragma once

#include "serialization.hpp"

#include <vector>
#include <queue>
#include <array>
#include <string>

namespace pebkac::lexing
{
	enum class token_type
	{
		COMMENT,
		NUMERIC_LITERAL,
		BOOLEAN_LITERAL,
		IDENTIFIER,
		OPERATOR,
		KEYWORD,
		BRACKET,
		SYNTATIC_ELEMENT,
	};

	std::string to_string(token_type t);

	class token: public serializable
	{
	public:
		token(token_type type, const std::string& value) noexcept;

		bool operator == (const token& other) const noexcept;
		bool operator != (const token& other) const noexcept;

		const token_type& get_type() const noexcept;
		const std::string& get_value() const noexcept;

		std::shared_ptr<serialized> serialize() const;

	private:
		const token_type type;
		const std::string value;
	};

	/**
	 * @brief Splits a source code string into tokens
	 * @param source Source code to parse into tokens
	 * @return A FIFO queue of tokens, used for parsing
	 */
	std::queue<token> tokenize(const std::string& source);
}
