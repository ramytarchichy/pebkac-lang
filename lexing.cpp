#include "lexing.hpp"

#include <regex>
#include <limits>
#include <algorithm>

using namespace pebkac::lexing;

std::queue<token> tokenize(const std::string& source)
{
	// Regex query corresponding to each type of token/lexeme. Yes, regex is fugly.
	// Order of elements matters! Later elements in the list have higher priority.
	const static std::array<const std::pair<const token_type, const std::regex>, 8> regex_queries = {
		std::make_pair(token_type::COMMENT, std::regex("(\\/{2,}.*)|(\\/\\*[\\s\\S]*?\\*\\/)")),
		std::make_pair(token_type::IDENTIFIER, std::regex("\\w+")),
		std::make_pair(token_type::OPERATOR, std::regex("[+\\-*/%]|!=|==|<|>|<=|>=|&&|\\|\\|")),
		std::make_pair(token_type::UNARY_OPERATOR, std::regex("[+\\-!]")),
		std::make_pair(token_type::KEYWORD, std::regex("(fun|io|return|let|if|else|false|true)\\b")),
		std::make_pair(token_type::BRACKET, std::regex("[(){}[\\]]")),
		std::make_pair(token_type::SYNTATIC_ELEMENT, std::regex(":|;|->|=|,")),
		std::make_pair(token_type::NUMERIC_LITERAL, std::regex("\\d*\\.?\\d+")),
	};

	// Loop through the source code and parse it into tokens
	std::queue<token> result = {};
	for(std::string::const_iterator current = source.begin(); current != source.end();)
	{
		// Try every single regex
		size_t i = 0;
		size_t fails = 0;
		std::array<std::pair<size_t, std::string>, regex_queries.size()> query_results;
		for(const auto& [t, r] : regex_queries)
		{
			if (auto it = std::sregex_iterator(current, source.end(), r); it != std::sregex_iterator())
			{
				query_results[i] = std::make_pair(it->position(), it->str());
			}
			else
			{
				query_results[i] = std::make_pair(std::numeric_limits<size_t>::max(), "");
				++fails;
			}
			++i;
		}

		//If no more tokens exist, end the loop
		if (fails == regex_queries.size())
			break;

		// Get whichever regex matches better
		size_t index = std::distance(query_results.begin(), std::min_element(query_results.begin(), query_results.end(),
			[](const std::pair<size_t, std::string>& a, const std::pair<size_t, std::string>& b){
				if (a.first != b.first)
					return a.first < b.first;
				return a.second.length() >= b.second.length();
			}));

		//Update result
		result.push(token(regex_queries[index].first, query_results[index].second));
		current += query_results[index].first + query_results[index].second.length();
	}

	// C++11 has move-semantics, so it does not have to copy the local vector when returning.
	return result;
}
