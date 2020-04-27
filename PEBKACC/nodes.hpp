#pragma once

#include "serialization.hpp"

#include <memory>
#include <vector>
#include <unordered_set>

namespace pebkac::ast
{
	enum class unary_operation
	{
		PLUS,
		MINUS,
		NOT,
	};


	enum class operation
	{
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		MODULUS,
		EQUAL,
		NOT_EQUAL,
		LESS_THAN,
		GREATER_THAN,
		LESS_OR_EQUAL,
		GREATER_OR_EQUAL,
		AND,
		OR,
	};


	enum class specifier
	{
		IO,
	};


	class node: public serializable { };
	class statement_node: public node { };
	class expression_node: public statement_node { };
	class type_node: public node { };


	class function_type_node: public type_node
	{
	public:
		function_type_node(
			const std::unordered_set<specifier>& specifiers,
			const std::vector<std::shared_ptr<type_node>>& parameters,
			const std::shared_ptr<type_node>& output
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::unordered_set<specifier>& get_specifiers() const noexcept;
		const std::vector<std::shared_ptr<type_node>>& get_parameters() const noexcept;
		const std::shared_ptr<type_node>& get_return_type() const noexcept;

	private:
		const std::unordered_set<specifier> specifiers;
		const std::vector<std::shared_ptr<type_node>> parameters;
		const std::shared_ptr<type_node> return_type;
	};


	class identifier_node: public expression_node, public type_node
	{
	public:
		identifier_node(
			const std::string& value
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::string& get_value() const noexcept;

	private:
		const std::string value;
	};


	class numeric_literal_node: public expression_node
	{
	public:
		numeric_literal_node(
			long long value
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		long long get_value() const noexcept;

	private:
		const long long value;
	};


	class boolean_literal_node: public expression_node
	{
	public:
		boolean_literal_node(
			bool value 
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		bool get_value() const noexcept;

	private:
		const bool value;
	};


	class group_node: public expression_node
	{
	public:
		group_node(
			const std::shared_ptr<expression_node>& expression
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::shared_ptr<expression_node>& get_expression() const noexcept;

	private:
		const std::shared_ptr<expression_node> expression;
	};


	class unary_operator_node: public expression_node
	{
	public:
		unary_operator_node(
			unary_operation operation,
			const std::shared_ptr<expression_node>& operand
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		unary_operation get_operation() const noexcept;
		const std::shared_ptr<expression_node>& get_operand() const noexcept;

	private:
		const unary_operation operation;
		const std::shared_ptr<expression_node> operand;
	};


	class operator_node: public expression_node
	{
	public:
		operator_node(
			operation operation,
			const std::shared_ptr<expression_node>& operand_a,
			const std::shared_ptr<expression_node>& operand_b
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		operation get_operation() const noexcept;
		const std::shared_ptr<expression_node>& get_operand_a() const noexcept;
		const std::shared_ptr<expression_node>& get_operand_b() const noexcept;

	private:
		const operation operation;
		const std::shared_ptr<expression_node> operand_a;
		const std::shared_ptr<expression_node> operand_b;
	};


	class block_node: public statement_node
	{
	public:
		block_node(
			const std::vector<std::shared_ptr<statement_node>>& statements
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::vector<std::shared_ptr<statement_node>>& get_statements() const noexcept;

	private:
		const std::vector<std::shared_ptr<statement_node>> statements;
	};


	class conditional_node: public statement_node
	{
	public:
		conditional_node(
			const std::shared_ptr<expression_node>& condition,
			const std::shared_ptr<statement_node>& branch_true,
			const std::shared_ptr<statement_node>& branch_false
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::shared_ptr<expression_node>& get_condition() const noexcept;
		const std::shared_ptr<statement_node>& get_branch_true() const noexcept;
		const std::shared_ptr<statement_node>& get_branch_false() const noexcept;

	private:
		const std::shared_ptr<expression_node> condition;
		const std::shared_ptr<statement_node> branch_true;
		const std::shared_ptr<statement_node> branch_false;
	};


	class conditional_expression_node: public expression_node
	{
	public:
		conditional_expression_node(
			const std::shared_ptr<expression_node>& condition,
			const std::shared_ptr<expression_node>& value_true,
			const std::shared_ptr<expression_node>& value_false
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::shared_ptr<expression_node>& get_condition() const noexcept;
		const std::shared_ptr<expression_node>& get_value_true() const noexcept;
		const std::shared_ptr<expression_node>& get_value_false() const noexcept;

	private:
		const std::shared_ptr<expression_node> condition;
		const std::shared_ptr<expression_node> value_true;
		const std::shared_ptr<expression_node> value_false;
	};


	class let_node: public statement_node
	{
	public:
		let_node(
			const std::string& name,
			const std::shared_ptr<type_node> type,
			const std::shared_ptr<expression_node> value
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::string& get_name() const noexcept;
		const std::shared_ptr<type_node>& get_type() const noexcept;
		const std::shared_ptr<expression_node>& get_value() const noexcept;

	private:
		const std::string name;
		const std::shared_ptr<type_node> type;
		const std::shared_ptr<expression_node> value;
	};


	class parameter_node: public node
	{
	public:
		parameter_node(
			const std::string& name,
			const std::shared_ptr<type_node>& type,
			const std::shared_ptr<expression_node>& default_value
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::string& get_name() const noexcept;
		const std::shared_ptr<type_node>& get_type() const noexcept;
		const std::shared_ptr<expression_node>& get_default_value() const noexcept;

	private:
		const std::string name;
		const std::shared_ptr<type_node> type;
		const std::shared_ptr<expression_node> default_value;
	};


	class lambda_node: public expression_node
	{
	public:
		lambda_node(
			const std::vector<std::shared_ptr<parameter_node>>& parameters,
			const std::vector<std::shared_ptr<statement_node>>& statements
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::vector<std::shared_ptr<parameter_node>>& get_parameters() const noexcept;
		const std::vector<std::shared_ptr<statement_node>>& get_statements() const noexcept;

	private:
		const std::vector<std::shared_ptr<parameter_node>> parameters;
		const std::vector<std::shared_ptr<statement_node>> statements;
	};


	class function_node: public statement_node
	{
	public:
		function_node(
			const std::unordered_set<specifier>& specifiers,
			const std::string& name,
			const std::vector<std::shared_ptr<parameter_node>>& parameters,
			const std::shared_ptr<type_node>& return_type,
			const std::shared_ptr<block_node>& body
		) noexcept;
		
		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::unordered_set<specifier>& get_specifiers() const noexcept;
		const std::string& get_name() const noexcept;
		const std::vector<std::shared_ptr<parameter_node>>& get_parameters() const noexcept;
		const std::shared_ptr<type_node>& get_return_type() const noexcept;
		const std::shared_ptr<block_node>& get_body() const noexcept;

	private:
		const std::unordered_set<specifier> specifiers;
		const std::string name;
		const std::vector<std::shared_ptr<parameter_node>> parameters;
		const std::shared_ptr<type_node> return_type;
		const std::shared_ptr<block_node> body;
	};

	
	class function_call_node: public expression_node
	{
	public:
		function_call_node(
			const std::shared_ptr<expression_node>& function,
			const std::vector<std::shared_ptr<expression_node>>& arguments
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::shared_ptr<expression_node>& get_function() const noexcept;
		const std::vector<std::shared_ptr<expression_node>>& get_arguments() const noexcept;

	private:
		const std::shared_ptr<expression_node> function;
		const std::vector<std::shared_ptr<expression_node>> arguments;
	};


	class return_node: public statement_node
	{
	public:
		return_node(
			const std::shared_ptr<expression_node>& value
		) noexcept;

		std::shared_ptr<serialized> serialize() const;

		// Getters
		const std::shared_ptr<expression_node>& get_value() const noexcept;

	private:
		const std::shared_ptr<expression_node> value;
	};


	class empty_statement_node: public statement_node
	{
	public:
		empty_statement_node() noexcept;

		std::shared_ptr<serialized> serialize() const;
	};
}
