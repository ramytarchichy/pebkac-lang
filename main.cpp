#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "lexing.hpp"

int main(int argc, const char** argv)
{
	//Read script file
	std::ifstream file(argv[1]);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string source = buffer.str();
	file.close();

	//Lexing
	auto tokens = lexing::get_tokens(source);
	for(const auto& t : tokens)
	{
		std::string str;
		switch(t.first)
		{
			case lexing::type::COMMENT:
				str = "COMMENT";
			break;

			case lexing::type::NUMERIC_LITERAL:
				str = "NUMERIC_LITERAL";
			break;

			case lexing::type::IDENTIFIER:
				str = "IDENTIFIER";
			break;

			case lexing::type::OPERATOR:
				str = "OPERATOR";
			break;

			case lexing::type::UNARY_OPERATOR:
				str = "UNARY_OPERATOR";
			break;

			case lexing::type::KEYWORD:
				str = "KEYWORD";
			break;

			case lexing::type::BRACKET:
				str = "BRACKET";
			break;

			case lexing::type::SYNTATIC_ELEMENT:
				str = "SYNTATIC_ELEMENT";
			break;
		}

		std::cout << str << " " << t.second << std::endl;
	}

	return EXIT_SUCCESS;
}
