#include "serialization.hpp"


serialized_object::serialized_object() noexcept
{ }


void serialized_object::operator+= (const std::pair<std::string, bool>& other)
{
	elements.push_back({other.first, std::make_shared<serialized_literal_bool>(other.second)});
}


void serialized_object::operator+= (const std::pair<std::string, long long>& other)
{
	elements.push_back({other.first, std::make_shared<serialized_literal_long_long>(other.second)});
}


void serialized_object::operator+= (const std::pair<std::string, const std::string&>& other)
{
	elements.push_back({other.first, std::make_shared<serialized_literal_string>(other.second)});
}


void serialized_object::operator+= (const std::pair<std::string, std::shared_ptr<serializable>>& other)
{
	elements.push_back({other.first, other.second?other.second->serialize():nullptr});
}

/*
template<class T>
void serialized_object::operator+= (const std::pair<std::string, std::vector<T>>& other)
{
	if (other.second)
		elements.push_back({other.first, std::make_shared<serialized_array>(other.second)});
	else
		elements.push_back({other.first, nullptr});
}*/


const std::vector<std::pair<std::string, std::shared_ptr<serialized>>>& serialized_object::get_data() const
{
	return elements;
}


std::string serialized_object::to_json() const
{
	std::string str = "";
	for(const auto& e : elements)
		str += (str.length()?",":"") + std::string("\"") + e.first + "\":" + (e.second?e.second->to_json():"null");
	
	return "{" + str + "}";
}

/*
template<class T>
serialized_array::serialized_array(const std::vector<T>& v) noexcept
{
	for(const auto& i : v)
	{
		(*this) += i;
	}
}*/


void serialized_array::operator+= (bool other)
{
	elements.push_back(std::make_shared<serialized_literal_bool>(other));
}


void serialized_array::operator+= (long long other)
{
	elements.push_back(std::make_shared<serialized_literal_long_long>(other));
}


void serialized_array::operator+= (const std::string& other)
{
	elements.push_back(std::make_shared<serialized_literal_string>(other));
}


void serialized_array::operator+= (const std::shared_ptr<serializable>& other)
{
	if (other)
		elements.push_back(other->serialize());
	else
		elements.push_back(nullptr);
}


template<class T>
void serialized_array::operator+= (const std::vector<T>& other)
{
	elements.push_back(std::make_shared<serialized_array>(other));
}


const std::vector<std::shared_ptr<serialized>>& serialized_array::get_data() const
{
	return elements;
}


std::string serialized_array::to_json() const
{
	std::string str = "";
	for(const auto& e : elements)
		str += (str.length()?",":"") + (e?e->to_json():"null");
	
	return "[" + str + "]";
}


template<class T>
serialized_literal<T>::serialized_literal(const T& value) noexcept:
	value(value)
{ }


template<class T>
const T& serialized_literal<T>::get_value() const
{
	return value;
}


serialized_literal_bool::serialized_literal_bool(bool value) noexcept:
	serialized_literal(value)
{ }


std::string serialized_literal_bool::to_json() const
{
	return value ? "true" : "false";
}


serialized_literal_long_long::serialized_literal_long_long(long long value) noexcept:
	serialized_literal(value)
{ }


std::string serialized_literal_long_long::to_json() const
{
	return std::to_string(value);
}


serialized_literal_string::serialized_literal_string(const std::string& value) noexcept:
	serialized_literal(value)
{ }


std::string serialized_literal_string::to_json() const
{
	return "\"" + value + "\"";
}
