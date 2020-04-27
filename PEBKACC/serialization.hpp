#pragma once

#include <string>
#include <memory>
#include <vector>


class serialized
{
public:
	virtual std::string to_json() const = 0;
};


class serializable
{
public:
	virtual std::shared_ptr<serialized> serialize() const = 0;
};


class serialized_array: public serialized
{
public:
	template<class T>
	serialized_array(const std::vector<T>& v) noexcept
	{
		for(const auto& i : v)
		{
			*this += i;
		}
	}

	void operator+= (bool other);
	void operator+= (long long other);
	void operator+= (const std::string& other);
	void operator+= (const std::shared_ptr<serializable>& other);

	template<class T>
	void operator+= (const std::vector<T>& other);


	const std::vector<std::shared_ptr<serialized>>& get_data() const;
	std::string to_json() const;

private:
	std::vector<std::shared_ptr<serialized>> elements = { };
};


class serialized_object: public serialized
{
public:
	serialized_object() noexcept;

	void operator+= (const std::pair<std::string, bool>& other);
	void operator+= (const std::pair<std::string, long long>& other);
	void operator+= (const std::pair<std::string, const std::string&>& other);
	void operator+= (const std::pair<std::string, std::shared_ptr<serializable>>& other);

	template<class T> //Templates are weird and have to live in the header file.
	void operator+= (const std::pair<std::string, std::vector<T>>& other)
	{
		elements.push_back({other.first, std::make_shared<serialized_array>(other.second)});
	}

	const std::vector<std::pair<std::string, std::shared_ptr<serialized>>>& get_data() const;
	std::string to_json() const;

private:
	std::vector<std::pair<std::string, std::shared_ptr<serialized>>> elements = { };
};


template<class T>
class serialized_literal: public serialized
{
public:
	serialized_literal(
		const T& value
	) noexcept;

	const T& get_value() const;
	virtual std::string to_json() const = 0;

protected:
	const T value;
};


class serialized_literal_bool: public serialized_literal<bool>
{
public:
	serialized_literal_bool(
		bool value
	) noexcept;

	std::string to_json() const;
};


class serialized_literal_long_long: public serialized_literal<long long>
{
public:
	serialized_literal_long_long(
		long long value
	) noexcept;

	std::string to_json() const;
};


class serialized_literal_string: public serialized_literal<std::string>
{
public:
	serialized_literal_string(
		const std::string& value
	) noexcept;

	std::string to_json() const;
};
