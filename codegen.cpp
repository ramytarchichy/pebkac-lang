#include "codegen.hpp"
#include "nodes.hpp"

#include <memory>

using namespace pebkac;
using namespace pebkac::codegen;


generator::generator(
	const std::vector<std::shared_ptr<ast::statement_node>>& ast) noexcept:
	ast(ast)
{ }


template<class T>
std::string generator::get_cpp(const std::vector<T>& ptrs, const std::string& indent, const std::string& separator)
{
	std::string result = "";
	for(const auto& ptr : ptrs)
		result += indent + (result.length()?separator:"") + get_cpp(ptr);
	return result;
}


std::string generator::get_cpp(const std::shared_ptr<ast::parameter_node>& ptr)
{
	return get_cpp(ptr->get_type()) + "& " + ptr->get_name() + (ptr->get_default_value()?(" = " + get_cpp(ptr->get_default_value())):"");
}


std::string generator::get_cpp(const std::shared_ptr<ast::type_node>& ptr)
{
	if (ptr == nullptr)
		return "const auto";

	if (std::dynamic_pointer_cast<ast::identifier_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::identifier_node>(ptr);
		return (cast->get_value()=="int"?"":"const ") + cast->get_value();
	}
	else if (std::dynamic_pointer_cast<ast::function_type_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::function_type_node>(ptr);
		return "const std::function<" + get_cpp(cast->get_return_type()) + "(" + get_cpp(cast->get_parameters(), "", ", ") + ")>";
	}

	throw std::runtime_error("WTF (type)");
}


std::string generator::get_cpp(const std::shared_ptr<ast::expression_node>& ptr)
{
	if (std::dynamic_pointer_cast<ast::function_call_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::function_call_node>(ptr);
		return get_cpp(cast->get_function()) + "(" + get_cpp(cast->get_arguments(), "", ", ") + ")";
	}
	else if (std::dynamic_pointer_cast<ast::lambda_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::lambda_node>(ptr);
		return "[&](" + get_cpp(cast->get_parameters(), "", ", ") + "){" + get_cpp(cast->get_statements(), "\n\t", "") + "\n}";
	}
	else if (std::dynamic_pointer_cast<ast::identifier_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::identifier_node>(ptr);
		return cast->get_value();
	}
	else if (std::dynamic_pointer_cast<ast::numeric_literal_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::numeric_literal_node>(ptr);
		return std::to_string(cast->get_value());
	}
	else if (std::dynamic_pointer_cast<ast::boolean_literal_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::boolean_literal_node>(ptr);
		return cast->get_value()?"true":"false";
	}
	else if (std::dynamic_pointer_cast<ast::group_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::group_node>(ptr);
		return "(" + get_cpp(cast->get_expression()) + ")";
	}
	else if (std::dynamic_pointer_cast<ast::unary_operator_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::unary_operator_node>(ptr);
		std::string result = "";

		switch (cast->get_operation())
		{
		case ast::unary_operation::PLUS:
			result += "+";
			break;

		case ast::unary_operation::MINUS:
			result += "-";
			break;

		case ast::unary_operation::NOT:
			result += "!";
			break;
		}

		result += get_cpp(cast->get_operand());
		return result;
	}
	else if (std::dynamic_pointer_cast<ast::operator_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::operator_node>(ptr);
		std::string result = get_cpp(cast->get_operand_a());

		switch (cast->get_operation())
		{
		case ast::operation::AND:
			result += "&&";
			break;

		case ast::operation::OR:
			result += "||";
			break;

		case ast::operation::EQUAL:
			result += "==";
			break;

		case ast::operation::NOT_EQUAL:
			result += "!=";
			break;

		case ast::operation::GREATER_THAN:
			result += ">";
			break;

		case ast::operation::LESS_THAN:
			result += "<";
			break;

		case ast::operation::GREATER_OR_EQUAL:
			result += ">=";
			break;

		case ast::operation::LESS_OR_EQUAL:
			result += "<=";
			break;

		case ast::operation::ADD:
			result += "+";
			break;

		case ast::operation::SUBTRACT:
			result += "-";
			break;

		case ast::operation::DIVIDE:
			result += "/";
			break;

		case ast::operation::MULTIPLY:
			result += "*";
			break;

		case ast::operation::MODULUS:
			result += "%";
			break;
		}

		result += get_cpp(cast->get_operand_b());
		return result;
	}
	else if (std::dynamic_pointer_cast<ast::conditional_expression_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::conditional_expression_node>(ptr);
		return "(" + get_cpp(cast->get_condition()) + "?(" + get_cpp(cast->get_value_true()) + "):(" + get_cpp(cast->get_value_false()) + "))";
	}
	
	throw std::runtime_error("WTF BRO (expression)");
}


std::string generator::get_cpp(const std::shared_ptr<ast::statement_node>& ptr)
{
	if (std::dynamic_pointer_cast<ast::function_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::function_node>(ptr);
		return get_cpp(cast->get_return_type()) +  " " + cast->get_name() + "(" + get_cpp(cast->get_parameters(), "", ", ") + ")" + get_cpp(cast->get_body());
	}
	else if (std::dynamic_pointer_cast<ast::let_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::let_node>(ptr);
		return get_cpp(cast->get_type()) + " " + cast->get_name() + " = " + get_cpp(cast->get_value()) + ";";
	}
	else if (std::dynamic_pointer_cast<ast::conditional_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::conditional_node>(ptr);
		return "if (" + get_cpp(cast->get_condition()) + ") " + get_cpp(cast->get_branch_true()) + (cast->get_branch_false() ? (" else " + get_cpp(cast->get_branch_false())) : "");
	}
	else if (std::dynamic_pointer_cast<ast::return_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::return_node>(ptr);
		return "return " + get_cpp(cast->get_value()) + ";";
	}
	else if (std::dynamic_pointer_cast<ast::empty_statement_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::empty_statement_node>(ptr);
		return "";
	}
	else if (std::dynamic_pointer_cast<ast::expression_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::expression_node>(ptr);
		return get_cpp(cast) + ";";
	}
	else if (std::dynamic_pointer_cast<ast::block_node>(ptr))
	{
		const auto cast = std::dynamic_pointer_cast<ast::block_node>(ptr);
		return "\n{" + get_cpp(cast->get_statements(), "\n\t", "") + "\n}";
	}

	throw std::runtime_error("WTF BRO (statement)");
}


std::string generator::get_cpp()
{
	std::string result = "#include <iostream>\n#include <functional>\n\ntypedef long long integer;\ntypedef bool boolean;\nvoid print(long long n)\n{\n\tstd::cout << n << std::endl;\n}\n\n";

	for(const auto& ptr : ast)
		result += get_cpp(ptr) + "\n\n";

	return result;
}
