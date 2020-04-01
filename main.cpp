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
	std::queue<lexing::token> tokens = lexing::tokenize(source);

	return EXIT_SUCCESS;
}
