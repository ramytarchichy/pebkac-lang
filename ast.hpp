#pragma once

#include "nodes.hpp"
#include "lexing.hpp"

#include <array>
#include <memory>
#include <vector>
#include <unordered_set>

namespace pebkac::ast
{
	class parser
	{
	public:
		parser(const std::queue<lexing::token>& tokens) noexcept;

		std::shared_ptr<expression_node> parse_expression();
		std::shared_ptr<statement_node> parse_statement();
		std::shared_ptr<conditional_node> parse_conditional();
		std::shared_ptr<conditional_expression_node> parse_conditional_expression();
		std::shared_ptr<type_node> parse_type();
		std::shared_ptr<identifier_node> parse_identifier();
		std::shared_ptr<function_type_node> parse_function_type();
		std::shared_ptr<let_node> parse_let();
		std::shared_ptr<parameter_node> parse_parameter();
		std::shared_ptr<function_node> parse_function();
		std::shared_ptr<function_call_node> parse_function_call();
		std::shared_ptr<return_node> parse_return();
		std::shared_ptr<block_node> parse_block();
		std::shared_ptr<lambda_node> parse_lambda();
		std::shared_ptr<boolean_literal_node> parse_boolean_literal();
		std::shared_ptr<numeric_literal_node> parse_numeric_literal();
		std::shared_ptr<group_node> parse_group();
		std::shared_ptr<operator_node> parse_operator();
		std::shared_ptr<unary_operator_node> parse_unary_operator();
		std::shared_ptr<empty_statement_node> parse_empty_statement();

		//std::vector<specifier> parse_specifiers();
		std::vector<std::shared_ptr<statement_node>> parse_statements();
		std::vector<std::shared_ptr<expression_node>> parse_expressions();
		std::vector<std::shared_ptr<parameter_node>> parse_parameters();

		bool is_end();

	private:
		std::queue<lexing::token> tokens;
		
		const lexing::token& peek_token();
		lexing::token consume_token();
		lexing::token consume_token(lexing::token_type type);
		lexing::token consume_token(lexing::token_type type, const std::string& value);
		lexing::token consume_token(const lexing::token& token);
	};


	class parsing_error: public std::runtime_error
	{
	public:
		parsing_error(
			const std::string& msg
		) noexcept;
	};


	class unexpected_token_type_error: public parsing_error
	{
	public:
		unexpected_token_type_error(
			const lexing::token_type& expected,
			const lexing::token_type& got
		) noexcept;

		const lexing::token_type& get_expected() const noexcept;
		const lexing::token_type& get_got() const noexcept;

	private:
		const lexing::token_type expected;
		const lexing::token_type got;
	};


	class unexpected_token_value_error: public parsing_error
	{
	public:
		unexpected_token_value_error(
			const std::string& expected,
			const std::string& got
		) noexcept;

		const std::string& get_expected() const noexcept;
		const std::string& get_got() const noexcept;

	private:
		const std::string expected;
		const std::string got;
	};


	class end_error: public parsing_error
	{
	public:
		end_error() noexcept;
	};
}
