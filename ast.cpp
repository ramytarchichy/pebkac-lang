#include "ast.hpp"

#include <stack>

using namespace pebkac;
using namespace pebkac::ast;


unary_operation string_to_unary_operation(const std::string& v)
{
	if (v == "+")  return unary_operation::PLUS;
	if (v == "-")  return unary_operation::MINUS;
	if (v == "!")  return unary_operation::NOT;

	throw std::runtime_error("Unknown unary operator: " + v);
}


operation string_to_operation(const std::string& v)
{
	if (v == "+")  return operation::ADD;
	if (v == "-")  return operation::SUBTRACT;
	if (v == "*")  return operation::MULTIPLY;
	if (v == "/")  return operation::DIVIDE;
	if (v == "%")  return operation::MODULUS;
	if (v == "==") return operation::EQUAL;
	if (v == "!=") return operation::NOT_EQUAL;
	if (v == "<")  return operation::LESS_THAN;
	if (v == ">")  return operation::GREATER_THAN;
	if (v == "<=") return operation::LESS_OR_EQUAL;
	if (v == ">=") return operation::GREATER_OR_EQUAL;
	if (v == "&&") return operation::AND;
	if (v == "||") return operation::OR;

	throw std::runtime_error("Unknown operator: " + v);
}


parser::parser(
	const std::queue<lexing::token>& tokens) noexcept:
	tokens(tokens)
{ }


void shunting_yard(std::stack<std::shared_ptr<expression_node>>& expressions, std::stack<std::string>& operations)
{
	while(operations.size())
	{
		const auto& op = operations.top();

		if (expressions.size() == 1)
		{
			const auto exp = expressions.top();
			expressions.pop();

			expressions.push(std::make_shared<unary_operator_node>(string_to_unary_operation(op), exp));
		}
		else
		{
			const auto exp_b = expressions.top();
			expressions.pop();
			const auto exp_a = expressions.top();
			expressions.pop();

			expressions.push(std::make_shared<operator_node>(string_to_operation(op), exp_a, exp_b));
		}

		operations.pop();
	}
}


size_t precedence(const std::string& s)
{
	if (s == "!")  return 0;
	if (s == "&&") return 1;
	if (s == "||") return 1;
	if (s == "+")  return 2;
	if (s == "-")  return 2;
	if (s == "*")  return 3;
	if (s == "/")  return 3;
	if (s == "%")  return 3;
	if (s == "==") return 4;
	if (s == "!=") return 4;
	if (s == "<")  return 4;
	if (s == ">")  return 4;
	if (s == "<=") return 4;
	if (s == ">=") return 4;

	throw std::runtime_error("Unknown operator: " + s);
}


std::shared_ptr<expression_node> parser::parse_expression()
{
	std::stack<std::shared_ptr<expression_node>> expressions = { };
	std::stack<std::string> operations = { };

	bool prev_op = false;
	while(true)
	{
		const lexing::token t = peek_token();

		if (t == lexing::token(lexing::token_type::BRACKET, "{"))
			expressions.push(parse_lambda());

		else if (t == lexing::token(lexing::token_type::BRACKET, "("))
			if (prev_op || expressions.size() == 0) expressions.push(parse_group());
			else
			{
				const auto previous_exp = expressions.top();
				expressions.pop();

				consume_token();
				expressions.push(std::make_shared<function_call_node>(previous_exp, parse_expressions()));
				consume_token(lexing::token_type::BRACKET, ")");
			}

		else if (t == lexing::token(lexing::token_type::KEYWORD, "if"))
			expressions.push(parse_conditional_expression());

		else if (t.get_type() == lexing::token_type::IDENTIFIER)
			expressions.push(parse_identifier());

		else if (t.get_type() == lexing::token_type::BOOLEAN_LITERAL)
			expressions.push(parse_boolean_literal());

		else if (t.get_type() == lexing::token_type::NUMERIC_LITERAL)
			expressions.push(parse_numeric_literal());

		else if (t.get_type() == lexing::token_type::OPERATOR)
			{
				//Shunting-Yard algorithm
				consume_token();
				const auto& op_b = t.get_value();
				if (operations.size() > 0)
				{
					const auto op_a = operations.top();
					
					if (precedence(op_b) < precedence(op_a))
						shunting_yard(expressions, operations);
				}

				operations.push(op_b);
			}

		else
			break;

		prev_op = t.get_type() == lexing::token_type::OPERATOR;
	}

	if (prev_op)
		throw parsing_error("Postfix operator detected.");

	//Shunting-Yard algorithm
	shunting_yard(expressions, operations);

	if (expressions.size() == 1 && operations.size() == 0)
		return expressions.top();
	else
		throw parsing_error("Malformed expression");
}


std::shared_ptr<statement_node> parser::parse_statement()
{
	const lexing::token t = peek_token();

	while(!is_end())
	{
		if (t == lexing::token(lexing::token_type::KEYWORD, "if"))
			return parse_conditional();
		else if (t == lexing::token(lexing::token_type::KEYWORD, "return"))
			return parse_return();
		else if (t == lexing::token(lexing::token_type::KEYWORD, "let"))
			return parse_let();
		else if (t == lexing::token(lexing::token_type::BRACKET, "{"))
			return parse_block();
		else if (t == lexing::token(lexing::token_type::KEYWORD, "fun") || t == lexing::token(lexing::token_type::KEYWORD, "io"))
			return parse_function();
		else if (t == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, ";"))
			return parse_empty_statement();
		else
		{
			const auto result = parse_expression();
			consume_token(lexing::token_type::SYNTATIC_ELEMENT, ";");
			return result;
		}
	}

	throw parsing_error("Cannot find statement");
}


std::shared_ptr<type_node> parser::parse_type()
{
	if (peek_token() == lexing::token(lexing::token_type::BRACKET, "("))
	{
		return parse_function_type();
	}
	else
	{
		return parse_identifier();
	}
}


std::vector<std::shared_ptr<statement_node>> parser::parse_statements()
{
	std::vector<std::shared_ptr<statement_node>> statements = { };
	while(!is_end() && peek_token() != lexing::token(lexing::token_type::BRACKET, "}"))
	{
		statements.push_back(parse_statement());
	}
	return statements;
}


std::vector<std::shared_ptr<expression_node>> parser::parse_expressions()
{
	std::vector<std::shared_ptr<expression_node>> expressions = { };
	if (peek_token() != lexing::token(lexing::token_type::BRACKET, ")"))
	{
		expressions.push_back(parse_expression());
		while(peek_token() == lexing::token(lexing::token_type::SYNTATIC_ELEMENT, ","))
		{
			consume_token();
			expressions.push_back(parse_expression());
		}
	}

	return expressions;
}


std::vector<std::shared_ptr<parameter_node>> parser::parse_parameters()
{
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

	return parameters;
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


std::shared_ptr<unary_operator_node> parser::parse_unary_operator()
{
	// <op> <expression>

	const unary_operation op = string_to_unary_operation(consume_token(lexing::token_type::OPERATOR).get_value());
	const auto expression = parse_expression();

	return std::make_shared<unary_operator_node>(op, expression);
}


std::shared_ptr<operator_node> parser::parse_operator()
{
	// <a> <op> <b>

	const auto a = parse_expression();
	const operation op = string_to_operation(consume_token(lexing::token_type::OPERATOR).get_value());
	const auto b = parse_expression();

	return std::make_shared<operator_node>(op, a, b);
}


std::shared_ptr<function_call_node> parser::parse_function_call()
{
	// <function> ( [args] )

	// Function
	const auto function = parse_expression();
	consume_token(lexing::token_type::BRACKET, "(");
	
	// Arguments
	const auto arguments = parse_expressions();
	consume_token(lexing::token_type::BRACKET, ")");

	return std::make_shared<function_call_node>(function, arguments);
}


std::shared_ptr<lambda_node> parser::parse_lambda()
{
	// { [params] -> <statement> }

	consume_token(lexing::token_type::BRACKET, "{");
	const auto params = parse_parameters();
	consume_token(lexing::token_type::SYNTATIC_ELEMENT, "->");
	const auto statements = parse_statements();
	consume_token(lexing::token_type::BRACKET, "}");

	return std::make_shared<lambda_node>(params, statements);
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
	consume_token(lexing::token_type::BRACKET, "(");
	const auto expression = parse_expression();
	consume_token(lexing::token_type::BRACKET, ")");
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
	consume_token(lexing::token_type::BRACKET, "(");
	const auto expression = parse_expression();
	consume_token(lexing::token_type::BRACKET, ")");
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
	const auto parameters = parse_parameters();

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


std::shared_ptr<empty_statement_node> parser::parse_empty_statement()
{
	// ;

	consume_token(lexing::token_type::SYNTATIC_ELEMENT, ";");
	return std::make_shared<empty_statement_node>();
}


bool parser::is_end()
{
	// Ignore comments
	while(tokens.size() != 0 && tokens.front().get_type() == lexing::token_type::COMMENT)
	{
		tokens.pop();
	}

	return tokens.size() == 0;
}


const lexing::token& parser::peek_token()
{
	if (is_end())
		throw end_error();
	
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
	parsing_error("Unexpected token type: expected: \"" + lexing::to_string(expected) + "\", got: \"" + lexing::to_string(got) + "\".")
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
	parsing_error("Unexpected token: expected: \"" + expected + "\", got: \"" + got + "\".")
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
	parsing_error("End of token queue reached.")
{ }