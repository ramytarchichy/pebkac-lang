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
	//Check argument count
	if (argc != 3)
	{
		std::cerr << "ERROR: Passed " << argc-1 << " argument" << (argc==2?"":"s") << ", expected 2." << std::endl;
		return EXIT_FAILURE;
	}
	const std::string_view output_type_arg(argv[2]);

	//Read source file
	std::ifstream file(argv[1]);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string source = buffer.str();
	file.close();

	//Tokenize
	std::queue<lexing::token> tokens = lexing::tokenize(source);

	if (output_type_arg == "tokens")
	{
		std::cout << "[";
		bool a = true;
		while(!tokens.empty())
		{
			std::cout << (a?"":", ") << tokens.front().serialize()->to_json();
			tokens.pop();
			a = false;
		}
		std::cout << "]" << std::endl;
		return EXIT_SUCCESS;
	}

	//Build Abstract Syntax Tree
	ast::parser parser(tokens);
	const auto statements = parser.parse_statements();
	
	if (output_type_arg == "ast")
	{
		std::cout << serialized_array(statements).to_json() << std::endl;
		return EXIT_SUCCESS;
	}

	//Generate C++
	codegen::generator g(statements);
	const std::string cpp = g.get_cpp();

	if (output_type_arg == "cpp")
	{
		std::cout << cpp << std::endl;
		return EXIT_SUCCESS;
	}

	//Argument didn't match any of the above
	std::cerr << "ERROR: Unrecognized argument \"" << output_type_arg << "\"" << std::endl;
	return EXIT_FAILURE;
}
