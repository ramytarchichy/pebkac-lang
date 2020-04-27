#include "nodes.hpp"

#include <numeric>
#include <stdexcept>

using namespace pebkac;
using namespace pebkac::ast;

//This allows the use of "..."s to create an std::string instead of a char*
using namespace std::string_literals;


std::string to_string(specifier s)
{
	if (s == specifier::IO) return "IO";

	throw std::runtime_error("JSON: unknown specifier");
}


std::vector<std::string> to_string(const std::unordered_set<specifier>& specifiers)
{
	std::vector<std::string> v = {};
	for(specifier s : specifiers)
	{
		v.push_back(to_string(s));
	}
	return v;
}


std::string to_string(operation op)
{
	if (op == operation::AND) return "AND";
	if (op == operation::OR) return "OR";
	if (op == operation::ADD) return "ADD";
	if (op == operation::SUBTRACT) return "SUBTRACT";
	if (op == operation::MULTIPLY) return "MULTIPLY";
	if (op == operation::DIVIDE) return "DIVIDE";
	if (op == operation::MODULUS) return "MODULUS";
	if (op == operation::EQUAL) return "EQUAL";
	if (op == operation::NOT_EQUAL) return "NOT_EQUAL";
	if (op == operation::LESS_THAN) return "LESS_THAN";
	if (op == operation::GREATER_THAN) return "GREATER_THAN";
	if (op == operation::LESS_OR_EQUAL) return "LESS_OR_EQUAL";
	if (op == operation::GREATER_OR_EQUAL) return "GREATER_OR_EQUAL";

	throw std::runtime_error("JSON: unknown operation");
}


std::string to_string(unary_operation op)
{
	if (op == unary_operation::PLUS) return "PLUS";
	if (op == unary_operation::MINUS) return "MINUS";
	if (op == unary_operation::NOT) return "NOT";

	throw std::runtime_error("JSON: unknown unary operation");
}


function_type_node::function_type_node(
	const std::unordered_set<specifier>& specifiers,
	const std::vector<std::shared_ptr<type_node>>& parameters,
	const std::shared_ptr<type_node>& return_type) noexcept:
	specifiers(specifiers),
	parameters(parameters),
	return_type(return_type)
{ }


const std::unordered_set<specifier>& function_type_node::get_specifiers() const noexcept
{
	return specifiers;
}


const std::vector<std::shared_ptr<type_node>>& function_type_node::get_parameters() const noexcept
{
	return parameters;
}


const std::shared_ptr<type_node>& function_type_node::get_return_type() const noexcept
{
	return return_type;
}


std::shared_ptr<serialized> function_type_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "function_type"s);
	*obj += std::make_pair("specifiers"s, to_string(specifiers));
	*obj += std::make_pair("parameters"s, parameters);
	*obj += std::make_pair("return_type"s, return_type);
	return obj;
}


identifier_node::identifier_node(
	const std::string& value) noexcept:
	value(value)
{ }


const std::string& identifier_node::get_value() const noexcept
{
	return value;
}


std::shared_ptr<serialized> identifier_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "identifier"s);
	*obj += std::make_pair("value"s, value);
	return obj;
}


numeric_literal_node::numeric_literal_node(
	long long value) noexcept:
	value(value)
{ }


long long numeric_literal_node::get_value() const noexcept
{
	return value;
}


std::shared_ptr<serialized> numeric_literal_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "numeric_literal"s);
	*obj += std::make_pair("value"s, value);
	return obj;
}


boolean_literal_node::boolean_literal_node(
	bool value) noexcept:
	value(value)
{ }


bool boolean_literal_node::get_value() const noexcept
{
	return value;
}


std::shared_ptr<serialized> boolean_literal_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "boolean_literal"s);
	*obj += std::make_pair("value"s, value);
	return obj;
}


group_node::group_node(
	const std::shared_ptr<expression_node>& expression) noexcept:
	expression(expression)
{ }


const std::shared_ptr<expression_node>& group_node::get_expression() const noexcept
{
	return expression;
}


std::shared_ptr<serialized> group_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "group"s);
	*obj += std::make_pair("expression"s, expression);
	return obj;
}


unary_operator_node::unary_operator_node(
	unary_operation operation,
	const std::shared_ptr<expression_node>& operand) noexcept:
	operation(operation),
	operand(operand)
{ }


unary_operation unary_operator_node::get_operation() const noexcept
{
	return operation;
}


const std::shared_ptr<expression_node>& unary_operator_node::get_operand() const noexcept
{
	return operand;
}


std::shared_ptr<serialized> unary_operator_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "unary_operator"s);
	*obj += std::make_pair("operation"s, to_string(operation));
	*obj += std::make_pair("operand"s, operand);
	return obj;
}


operator_node::operator_node(
	ast::operation operation,
	const std::shared_ptr<expression_node>& operand_a,
	const std::shared_ptr<expression_node>& operand_b) noexcept:
	operation(operation),
	operand_a(operand_a),
	operand_b(operand_b)
{ }


operation operator_node::get_operation() const noexcept
{
	return operation;
}


const std::shared_ptr<expression_node>& operator_node::get_operand_a() const noexcept
{
	return operand_a;
}


const std::shared_ptr<expression_node>& operator_node::get_operand_b() const noexcept
{
	return operand_b;
}


std::shared_ptr<serialized> operator_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "operator"s);
	*obj += std::make_pair("operation"s, to_string(operation));
	*obj += std::make_pair("operand_a"s, operand_a);
	*obj += std::make_pair("operand_b"s, operand_b);
	return obj;
}


block_node::block_node(
	const std::vector<std::shared_ptr<statement_node>>& statements) noexcept:
	statements(statements)
{ }


const std::vector<std::shared_ptr<statement_node>>& block_node::get_statements() const noexcept
{
	return statements;
}


std::shared_ptr<serialized> block_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "block"s);
	*obj += std::make_pair("statements"s, statements);
	return obj;
}


conditional_node::conditional_node(
	const std::shared_ptr<expression_node>& condition,
	const std::shared_ptr<statement_node>& branch_true,
	const std::shared_ptr<statement_node>& branch_false) noexcept:
	condition(condition),
	branch_true(branch_true),
	branch_false(branch_false)
{ }


const std::shared_ptr<expression_node>& conditional_node::get_condition() const noexcept
{
	return condition;
}

const std::shared_ptr<statement_node>& conditional_node::get_branch_true() const noexcept
{
	return branch_true;
}


const std::shared_ptr<statement_node>& conditional_node::get_branch_false() const noexcept
{
	return branch_false;
}


std::shared_ptr<serialized> conditional_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "conditional"s);
	*obj += std::make_pair("condition"s, condition);
	*obj += std::make_pair("branch_true"s, branch_true);
	*obj += std::make_pair("branch_false"s, branch_false);
	return obj;
}


conditional_expression_node::conditional_expression_node(
	const std::shared_ptr<expression_node>& condition,
	const std::shared_ptr<expression_node>& value_true,
	const std::shared_ptr<expression_node>& value_false) noexcept:
	condition(condition),
	value_true(value_true),
	value_false(value_false)
{ }


const std::shared_ptr<expression_node>& conditional_expression_node::get_condition() const noexcept
{
	return condition;
}

const std::shared_ptr<expression_node>& conditional_expression_node::get_value_true() const noexcept
{
	return value_true;
}


const std::shared_ptr<expression_node>& conditional_expression_node::get_value_false() const noexcept
{
	return value_false;
}


std::shared_ptr<serialized> conditional_expression_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "conditional_expression"s);
	*obj += std::make_pair("condition"s, condition);
	*obj += std::make_pair("value_true"s, value_true);
	*obj += std::make_pair("value_false"s, value_false);
	return obj;
}


let_node::let_node(
	const std::string& name,
	const std::shared_ptr<type_node> type,
	const std::shared_ptr<expression_node> value) noexcept:
	name(name),
	type(type),
	value(value)
{ }


const std::string& let_node::get_name() const noexcept
{
	return name;
}


const std::shared_ptr<type_node>& let_node::get_type() const noexcept
{
	return type;
}


const std::shared_ptr<expression_node>& let_node::get_value() const noexcept
{
	return value;
}


std::shared_ptr<serialized> let_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "let"s);
	*obj += std::make_pair("name"s, name);
	*obj += std::make_pair("type"s, type);
	*obj += std::make_pair("value"s, value);
	return obj;
}


parameter_node::parameter_node(
	const std::string& name,
	const std::shared_ptr<type_node>& type,
	const std::shared_ptr<expression_node>& default_value) noexcept:
	name(name),
	type(type),
	default_value(default_value)
{ }


const std::string& parameter_node::get_name() const noexcept
{
	return name;
}


const std::shared_ptr<type_node>& parameter_node::get_type() const noexcept
{
	return type;
}


const std::shared_ptr<expression_node>& parameter_node::get_default_value() const noexcept
{
	return default_value;
}


std::shared_ptr<serialized> parameter_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "parameter"s);
	*obj += std::make_pair("name"s, name);
	*obj += std::make_pair("type"s, type);
	*obj += std::make_pair("default_value"s, default_value);
	return obj;
}


lambda_node::lambda_node(
	const std::vector<std::shared_ptr<parameter_node>>& parameters,
	const std::vector<std::shared_ptr<statement_node>>& statements) noexcept:
	parameters(parameters),
	statements(statements)
{ }


const std::vector<std::shared_ptr<parameter_node>>& lambda_node::get_parameters() const noexcept
{
	return parameters;
}


const std::vector<std::shared_ptr<statement_node>>& lambda_node::get_statements() const noexcept
{
	return statements;
}


std::shared_ptr<serialized> lambda_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "lambda"s);
	*obj += std::make_pair("parameters"s, parameters);
	*obj += std::make_pair("statements"s, statements);
	return obj;
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


const std::unordered_set<specifier>& function_node::get_specifiers() const noexcept
{
	return specifiers;
}


const std::string& function_node::get_name() const noexcept
{
	return name;
}


const std::vector<std::shared_ptr<parameter_node>>& function_node::get_parameters() const noexcept
{
	return parameters;
}


const std::shared_ptr<type_node>& function_node::get_return_type() const noexcept
{
	return return_type;
}


const std::shared_ptr<block_node>& function_node::get_body() const noexcept
{
	return body;
}


std::shared_ptr<serialized> function_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "function"s);
	*obj += std::make_pair("specifiers"s, to_string(specifiers));
	*obj += std::make_pair("name"s, name);
	*obj += std::make_pair("parameters"s, parameters);
	*obj += std::make_pair("return_type"s, return_type);
	*obj += std::make_pair("body"s, body);
	return obj;
}


function_call_node::function_call_node(
	const std::shared_ptr<expression_node>& function,
	const std::vector<std::shared_ptr<expression_node>>& arguments) noexcept:
	function(function),
	arguments(arguments)
{ }


const std::shared_ptr<expression_node>& function_call_node::get_function() const noexcept
{
	return function;
}


const std::vector<std::shared_ptr<expression_node>>& function_call_node::get_arguments() const noexcept
{
	return arguments;
}


std::shared_ptr<serialized> function_call_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "function_call"s);
	*obj += std::make_pair("function"s, function);
	*obj += std::make_pair("arguments"s, arguments);
	return obj;
}


return_node::return_node(
	const std::shared_ptr<expression_node>& value) noexcept:
	value(value)
{ }


const std::shared_ptr<expression_node>& return_node::get_value() const noexcept
{
	return value;
}


std::shared_ptr<serialized> return_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "return"s);
	*obj += std::make_pair("value"s, value);
	return obj;
}


empty_statement_node:: empty_statement_node() noexcept
{ }


std::shared_ptr<serialized> empty_statement_node::serialize() const
{
	auto obj = std::make_shared<serialized_object>();
	*obj += std::make_pair("node"s, "empty_statement"s);
	return obj;
}
