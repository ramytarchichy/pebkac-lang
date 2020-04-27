#pragma once

#include "nodes.hpp"

namespace pebkac::codegen
{
	class generator
	{
	public:
		generator(
			const std::vector<std::shared_ptr<ast::statement_node>>& ast
		) noexcept;

		std::string get_cpp();
		std::string get_cpp(const std::shared_ptr<ast::expression_node>& ptr);
		std::string get_cpp(const std::shared_ptr<ast::statement_node>& ptr);
		std::string get_cpp(const std::shared_ptr<ast::type_node>& ptr);
		std::string get_cpp(const std::shared_ptr<ast::parameter_node>& ptr);

		template<class T>
		std::string get_cpp(const std::vector<T>& ptrs, const std::string& indent, const std::string& separator);

	private:
		const std::vector<std::shared_ptr<ast::statement_node>>& ast;
	};
}
