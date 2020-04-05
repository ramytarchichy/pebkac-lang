#include "ast.hpp"

using namespace pebkac;
using namespace pebkac::ast;


parser::parser(
	const std::queue<lexing::token>& tokens) noexcept:
	tokens(tokens)
{ }


std::shared_ptr<expression_node> parser::parse_expression()
{

}


std::shared_ptr<statement_node> parser::parse_statement()
{
	
}


std::shared_ptr<boolean_literal_node> parser::parse_boolean_literal()
{
	const lexing::token t = consume_token(lexing::token_type::BOOLEAN_LITERAL);
	return std::make_shared<boolean_literal_node>(t.get_value() == "true");
}


std::shared_ptr<numeric_literal_node> parser::parse_numeric_literal()
{
	const lexing::token t = consume_token(lexing::token_type::NUMERIC_LITERAL);
	return std::make_shared<numeric_literal_node>(std::stoll(t.get_value()));
}


std::shared_ptr<group_node> parser::parse_group()
{
	consume_token(lexing::token_type::BRACKET, "(");
	const auto exp = parse_expression();
	consume_token(lexing::token_type::BRACKET, ")");

	return std::make_shared<group_node>(exp);
}


std::shared_ptr<operator_node> parser::parse_operator()
{
	// <a> <op> <b>

	const auto a = parse_expression();
	const std::string v = consume_token(lexing::token_type::OPERATOR).get_value();
	const auto b = parse_expression();

	operation op;
	if (v == "+")  op = operation::ADD;
	if (v == "-")  op = operation::SUBTRACT;
	if (v == "*")  op = operation::MULTIPLY;
	if (v == "/")  op = operation::DIVIDE;
	if (v == "%")  op = operation::MODULUS;
	if (v == "==") op = operation::EQUAL;
	if (v == "!=") op = operation::NOT_EQUAL;
	if (v == "<")  op = operation::LESS_THAN;
	if (v == ">")  op = operation::GREATER_THAN;
	if (v == "<=") op = operation::LESS_OR_EQUAL;
	if (v == ">=") op = operation::GREATER_OR_EQUAL;
	if (v == "&&") op = operation::AND;
	if (v == "||") op = operation::OR;

	return std::make_shared<operator_node>(op, a, b);
}


std::shared_ptr<function_call_node> parser::parse_function_call()
{
	// <function> ( [args] )

	// Function
	const auto function = parse_expression();
	consume_token(lexing::token_type::BRACKET, "(");
	
	// Arguments
	std::vector<std::shared_ptr<expression_node>> arguments = { };
	if (peek_token() != lexing::token(lexing::token_type::BRACKET, ")"))
	{
		arguments.push_back(parse_expression());
		while(peek_token() == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, ","))
		{
			consume_token();
			arguments.push_back(parse_expression());
		}
	}
	consume_token(lexing::token_type::BRACKET, ")");

	return std::make_shared<function_call_node>(function, arguments);
}


std::shared_ptr<lambda_node> parser::parse_lambda()
{
	// { [params] -> <statement> }

	consume_token(lexing::token_type::BRACKET, "{");
	//const auto params = parse_parameters();
	const std::vector<std::shared_ptr<parameter_node>> params = {}; //TODO:
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, "->");
	const auto statements = parse_statement();
	consume_token(lexing::token_type::BRACKET, "}");

	return std::make_shared<lambda_node>(params, statements);
}


std::shared_ptr<type_node> parser::parse_type()
{
	try
	{
		return parse_identifier();
	}
	catch(const parsing_error& e)
	{
		return parse_function_type();
	}
}


std::shared_ptr<identifier_node> parser::parse_identifier()
{
	// <name>
	return std::make_shared<identifier_node>(consume_token(lexing::token_type::IDENTIFIER).get_value());
}


std::shared_ptr<function_type_node> parser::parse_function_type()
{
	//TODO: specifiers
	// ( [param_types] ) -> <return_type>

	consume_token(lexing::token_type::BRACKET, "(");

	std::vector<std::shared_ptr<type_node>> parameter_types = { };
	if (peek_token() != lexing::token(lexing::token_type::BRACKET, ")"))
	{
		parameter_types.push_back(parse_type());
		while(peek_token() == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, ","))
		{
			consume_token();
			parameter_types.push_back(parse_type());
		}
	}
	consume_token();

	consume_token(lexing::token_type::SYNTATIC_ELEMENT, "->");
	const auto return_type = parse_type(); 

	std::unordered_set<specifier> aaa = {};
	return std::make_shared<function_type_node>(aaa, parameter_types, return_type);
}


std::shared_ptr<conditional_node> parser::parse_conditional()
{
	// if ( <condition> ) <branch_true> [else <branch_false>]

	consume_token(lexing::token_type::KEYWORD, "if");
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, "(");
	const auto expression = parse_expression();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ")");
	const auto branch_true = parse_statement();
	
	std::shared_ptr<statement_node> branch_false = nullptr;
	if (peek_token() == lexing::token(lexing::token_type::KEYWORD, "else"))
	{
		consume_token();
		branch_false = parse_statement();
	}

	return std::make_shared<conditional_node>(expression, branch_true, branch_false);
}


std::shared_ptr<conditional_expression_node> parser::parse_conditional_expression()
{
	// if ( <condition> ) <branch_true> else <branch_false>

	consume_token(lexing::token_type::KEYWORD, "if");
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, "(");
	const auto expression = parse_expression();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ")");
	const auto branch_true = parse_expression();
	consume_token(lexing::token_type::KEYWORD, "else");
	const auto branch_false = parse_expression();

	return std::make_shared<conditional_expression_node>(expression, branch_true, branch_false);
}


std::shared_ptr<let_node> parser::parse_let()
{
	// let <name> : <type> = <value>;
	consume_token(lexing::token_type::KEYWORD, "let");
	const std::string& name = consume_token(lexing::token_type::IDENTIFIER).get_value();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ":");
	const std::shared_ptr<type_node> type = parse_type();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, "=");
	const std::shared_ptr<expression_node> value = parse_expression();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ";");

	return std::make_unique<let_node>(name, type, value);
}


std::shared_ptr<parameter_node> parser::parse_parameter()
{
	// <name> : <type> [= <expression>]
	const std::string name = consume_token(lexing::token_type::IDENTIFIER).get_value();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ":");
	const std::shared_ptr<type_node> type = parse_type();

	std::shared_ptr<expression_node> value = nullptr;
	if (peek_token() == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, "="))
	{
		consume_token();
		value = parse_expression();
	}

	return std::make_shared<parameter_node>(name, type, value);
}


std::shared_ptr<function_node> parser::parse_function()
{
	// [specifiers] fun <name>([params]) : <return_type> [= <expression>;] | { <statements> }

	// Specifiers
	const std::array<lexing::token, 1> specifier_array = {
		lexing::token(lexing::token_type::KEYWORD, "io"),
	};
	std::unordered_set<specifier> specifiers = { };
	if (peek_token() == specifier_array[0]) // TODO: loops, and check duplicates
	{
		consume_token();
		specifiers.insert(specifier::IO);
	}

	// Some syntatic stuff and name
	consume_token(lexing::token_type::KEYWORD, "fun");
	const std::string name = consume_token(lexing::token_type::IDENTIFIER).get_value();
	consume_token(lexing::token_type::BRACKET, "(");
	
	// Parameters
	std::vector<std::shared_ptr<parameter_node>> parameters = {};
	if (peek_token().get_type() == lexing::token_type::IDENTIFIER)
	{
		parameters.push_back(parse_parameter());
		while(peek_token() == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, ","))
		{
			consume_token();
			parameters.push_back(parse_parameter());
		}
	}

	// Return type
	consume_token(lexing::token_type::BRACKET, ")");
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ":");
	const std::shared_ptr<type_node> type = parse_type();

	// Body
	std::shared_ptr<block_node> body;
	if (peek_token() == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, "="))
	{
		consume_token();
		const auto value = parse_expression();
		consume_token(lexing::token_type::SYNTATIC_ELEMENT, ";");

		const std::vector<std::shared_ptr<statement_node>> b = {std::make_shared<return_node>(value)};
		body = std::make_shared<block_node>(b);
	}
	else
	{
		body = parse_block();
	}

	// Create node and return
	return std::make_shared<function_node>(specifiers, name, parameters, type, body);
}


std::shared_ptr<return_node> parser::parse_return()
{
	// return <expression>;
	consume_token(lexing::token_type::KEYWORD, "return");
	const std::shared_ptr<expression_node> value = parse_expression();;
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ";");

	return std::make_shared<return_node>(value);
}


std::shared_ptr<block_node> parser::parse_block()
{
	// { [statements] }

	consume_token(lexing::token_type::BRACKET, "{");

	std::vector<std::shared_ptr<statement_node>> statements = { };
	while(peek_token() != lexing::token(lexing::token_type::BRACKET, "}"))
	{
		statements.push_back(parse_statement());
	}

	consume_token();

	return std::make_shared<block_node>(statements);
}


const lexing::token& parser::peek_token()
{
	//TODO: check for end?

	// Ignore comments
	while(tokens.front().get_type() == lexing::token_type::COMMENT)
	{
		tokens.pop();
	}
	
	return tokens.front();
}


lexing::token parser::consume_token()
{
	const lexing::token token = peek_token();
	tokens.pop();
	return token;
}


lexing::token parser::consume_token(lexing::token_type type)
{
	const lexing::token t = consume_token();
	if(t.get_type() == type)
	{
		return t;
	}
	else
	{
		throw unexpected_token_type_error(type, t.get_type());
	}
}


lexing::token parser::consume_token(lexing::token_type type, const std::string& value)
{
	const lexing::token t = consume_token(type);
	if(t.get_value() == value)
	{
		return t;
	}
	else
	{
		throw unexpected_token_value_error(value, t.get_value());
	}
}


lexing::token parser::consume_token(const lexing::token& token)
{
	return consume_token(token.get_type(), token.get_value());
}


parsing_error::parsing_error(
	const std::string& msg) noexcept:
	std::runtime_error(msg)
{ }


unexpected_token_type_error::unexpected_token_type_error(
	const lexing::token_type& expected,
	const lexing::token_type& got) noexcept:
	expected(expected),
	got(got),
	parsing_error("")//TODO
{ }


const lexing::token_type& unexpected_token_type_error::get_expected() const noexcept
{
	return expected;
}


const lexing::token_type& unexpected_token_type_error::get_got() const noexcept
{
	return got;
}


unexpected_token_value_error::unexpected_token_value_error(
	const std::string& expected,
	const std::string& got) noexcept:
	expected(expected),
	got(got),
	parsing_error("")//TODO
{ }


const std::string& unexpected_token_value_error::get_expected() const noexcept
{
	return expected;
}


const std::string& unexpected_token_value_error::get_got() const noexcept
{
	return got;
}


end_error::end_error() noexcept:
	parsing_error("") //TODO
{ }