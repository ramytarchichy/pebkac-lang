#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "lexing.hpp"
#include "ast.hpp"

using namespace pebkac;

int main(int argc, const char** argv)
{
	//Read source file
	std::ifstream file(argv[1]);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string source = buffer.str();
	file.close();

	//Lexing
	const std::queue<lexing::token> tokens = lexing::tokenize(source);

	//Build Abstract Syntax Tree
	ast::parser parser(tokens);
	
	//Print AST
	std::cout << parser.parse_function()->to_json() << std::endl;

	return EXIT_SUCCESS;
}
