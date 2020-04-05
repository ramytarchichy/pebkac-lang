#include "nodes.hpp"

#include <numeric>

using namespace pebkac::ast;

// Note: I don't need a fancy json_builder class. This is just for debugging. As a result,
// the following to_json() functions are gonna be fugly. I don't give a damn about that.

std::string to_json(specifier s)
{
	if (s == specifier::IO) return "\"IO\"";

	throw std::runtime_error("JSON: unknown specifier");
}


std::string to_json(const std::unordered_set<specifier>& specifiers)
{
	std::string str = "";
	for(specifier s : specifiers)
	{
		str += (str.length()?", ":"") + to_json(s);
	}
	return "[" + str + "]";
}


std::string to_json(operation op)
{
	if (op == operation::AND) return "\"AND\"";
	if (op == operation::OR) return "\"OR\"";
	if (op == operation::ADD) return "\"ADD\"";
	if (op == operation::SUBTRACT) return "\"SUBTRACT\"";
	if (op == operation::MULTIPLY) return "\"MULTIPLY\"";
	if (op == operation::DIVIDE) return "\"DIVIDE\"";
	if (op == operation::MODULUS) return "\"MODULUS\"";
	if (op == operation::EQUAL) return "\"EQUAL\"";
	if (op == operation::NOT_EQUAL) return "\"NOT_EQUAL\"";
	if (op == operation::LESS_THAN) return "\"LESS_THAN\"";
	if (op == operation::GREATER_THAN) return "\"GREATER_THAN\"";
	if (op == operation::LESS_OR_EQUAL) return "\"LESS_OR_EQUAL\"";
	if (op == operation::GREATER_OR_EQUAL) return "\"GREATER_OR_EQUAL\"";

	throw std::runtime_error("JSON: unknown operation");
}


template<class T>
std::string to_json(const std::vector<std::shared_ptr<T>>& vec)
{
	std::string str = "";
	for(const std::shared_ptr<T>& i : vec)
	{
		str += (str.length()?", ":"") + i->to_json();
	}
	return "[" + str + "]";
}


function_type_node::function_type_node(
	const std::unordered_set<specifier>& specifiers,
	const std::vector<std::shared_ptr<type_node>>& parameters,
	const std::shared_ptr<type_node>& output) noexcept:
	specifiers(specifiers),
	parameters(parameters),
	output(output)
{ }


std::string function_type_node::to_json() const
{
	return "{\"node\":\"function_type\", \"specifiers\":" + ::to_json(specifiers) + ", \"parameters\":" + ::to_json(parameters) + ", \"output\":" + output->to_json() + "}";
}


identifier_node::identifier_node(
	const std::string& value) noexcept:
	value(value)
{ }


std::string identifier_node::to_json() const
{
	return "{\"node\":\"identifier\", \"value\":\"" + value + "\"}";
}


numeric_literal_node::numeric_literal_node(
	long long value) noexcept:
	value(value)
{ }


std::string numeric_literal_node::to_json() const
{
	return "{\"node\":\"numeric_literal\", \"value\":" + std::to_string(value) + "}";
}


boolean_literal_node::boolean_literal_node(
	bool value) noexcept:
	value(value)
{ }


std::string boolean_literal_node::to_json() const
{
	return "{\"node\":\"boolean_literal\", \"value\":" + std::string(value?"true":"false") + "}";
}


group_node::group_node(
	const std::shared_ptr<expression_node>& expression) noexcept:
	expression(expression)
{ }


std::string group_node::to_json() const
{
	return "{\"node\":\"group\", \"expression\":" + expression->to_json() + "}";
}


operator_node::operator_node(
	ast::operation operation,
	const std::shared_ptr<expression_node>& operand_a,
	const std::shared_ptr<expression_node>& operand_b) noexcept:
	operation(operation),
	operand_a(operand_a),
	operand_b(operand_b)
{ }


std::string operator_node::to_json() const
{
	return "{\"node\":\"operator\", \"operation\":" + ::to_json(operation) + ", \"operand_a\":" + operand_a->to_json() + ", \"operand_b\":" + operand_b->to_json() + "}";
}


block_node::block_node(
	const std::vector<std::shared_ptr<statement_node>>& statements) noexcept:
	statements(statements)
{ }


std::string block_node::to_json() const
{
	return "{\"node\":\"block\", \"statements\":" + ::to_json(statements) + "}";
}


conditional_node::conditional_node(
	const std::shared_ptr<expression_node>& condition,
	const std::shared_ptr<statement_node>& branch_true,
	const std::shared_ptr<statement_node>& branch_false) noexcept:
	condition(condition),
	branch_true(branch_true),
	branch_false(branch_false)
{ }


std::string conditional_node::to_json() const
{
	return "{\"node\":\"conditional\", \"condition\":" + condition->to_json() + ", \"branch_true\":" + branch_true->to_json() + ", \"branch_false\":" + (branch_false?branch_false->to_json():"null") + "}";
}


conditional_expression_node::conditional_expression_node(
	const std::shared_ptr<expression_node>& condition,
	const std::shared_ptr<expression_node>& value_true,
	const std::shared_ptr<expression_node>& value_false) noexcept:
	condition(condition),
	value_true(value_true),
	value_false(value_false)
{ }


std::string conditional_expression_node::to_json() const
{
	return "{\"node\":\"conditional\", \"condition\":" + condition->to_json() + ", \"value_true\":" + value_true->to_json() + ", \"value_false\":" + value_false->to_json() + "}";
}


let_node::let_node(
	const std::string& name,
	const std::shared_ptr<type_node> type,
	const std::shared_ptr<expression_node> value) noexcept:
	name(name),
	type(type),
	value(value)
{ }


std::string let_node::to_json() const
{
	return "{\"node\":\"let\", \"name\":\"" + name + "\", \"type\":" + type->to_json() + ", \"value\":" + value->to_json() + "}";
}


parameter_node::parameter_node(
	const std::string& name,
	const std::shared_ptr<type_node>& type,
	const std::shared_ptr<expression_node>& default_value) noexcept:
	name(name),
	type(type),
	default_value(default_value)
{ }


std::string parameter_node::to_json() const
{
	return "{\"node\":\"parameter\", \"name\":\"" + name + "\", \"type\":" + type->to_json() + (default_value?", \"default_value\":" + default_value->to_json():"") + "}";
}


lambda_node::lambda_node(
	const std::vector<std::shared_ptr<parameter_node>>& parameters,
	const std::shared_ptr<statement_node>& statement) noexcept:
	parameters(parameters),
	statement(statement)
{ }


std::string lambda_node::to_json() const
{
	return "{\"node\":\"lambda\", \"parameters\":" + ::to_json(parameters) + ", \"statement\":" + statement->to_json() + "}";
}


function_node::function_node(
	const std::unordered_set<specifier>& specifiers,
	const std::string& name,
	const std::vector<std::shared_ptr<parameter_node>>& parameters,
	const std::shared_ptr<type_node>& return_type,
	const std::shared_ptr<block_node>& body) noexcept:
	specifiers(specifiers),
	name(name),
	parameters(parameters),
	return_type(return_type),
	body(body)
{ }


std::string function_node::to_json() const
{
	return "{\"node\":\"function\", \"specifiers\":" + ::to_json(specifiers) + ", \"name\":\"" + name + "\", \"parameters\":" + ::to_json(parameters) + ", \"return_type\":" + return_type->to_json() + ", \"body\":" + body->to_json() + "}";
}


function_call_node::function_call_node(
	const std::shared_ptr<expression_node>& function,
	const std::vector<std::shared_ptr<expression_node>>& arguments) noexcept:
	function(function),
	arguments(arguments)
{ }


std::string function_call_node::to_json() const
{
	return "{\"node\":\"function_call\", \"function\":" + function->to_json() + ", \"arguments\":" + ::to_json(arguments) + "}";
}


return_node::return_node(
	const std::shared_ptr<expression_node>& value) noexcept:
	value(value)
{ }


std::string return_node::to_json() const
{
	return "{\"node\":\"return\", \"value\":" + value->to_json() + "}";
}
