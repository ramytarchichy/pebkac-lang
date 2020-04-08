#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "lexing.hpp"
#include "ast.hpp"
#include "codegen.hpp"

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
	const auto statements = parser.parse_statements();

	//Print AST
	std::cout << "{\"node\":\"top\", \"statements\":" << ast::to_json(statements) << "}" << std::endl;

	//Print C++
	codegen::generator g(statements);
	std::cout << std::endl << std::endl << g.get_cpp() << std::endl;

	return EXIT_SUCCESS;
}
