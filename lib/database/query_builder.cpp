#include "query_builder.h"

template <typename Add>
std::string join(const std::vector<std::string>& vec, Add add, char delimiter = ',')
{
	auto begin = vec.cbegin();
	auto end   = vec.cend();

	std::string sink;
	if (begin == end)
		return sink;

	add(sink, *begin++);
	while (begin != end) {
		sink += delimiter;
		add(sink, *begin++);
	}

	return sink;
}

std::string join(const std::vector<std::string>& vec, char delimiter = ',')
{
	return join(vec, [] (auto& sink, const auto& value) { sink += value; });
}

auto query_builder::insert_query::add_row(const std::vector<std::string>& fields) -> insert_query&
{
	// Wrap the value in ''
	static const auto add = [] (auto& sink, std::string value) {
		sink += '\'';
		sink += value;
		sink += '\'';
	};

	if (!_values.empty())
		_values.push_back(',');
	_values += '(';
	_values += join(fields, add);
	_values += ')';
	return *this;
}

query_builder::insert_query::operator std::string() const
{
	std::string result = "insert into ";
	result += _table.name;
	result += '(';
	result += join(_table.fields);
	result += ')';
	result += " values ";
	result += _values;
	return result;
}

std::string query_builder::make_select_query() const
{
	// TODO: add support for where, orber by, join
	std::string result = "select ";
	result += join(_table.fields);
	result += " from ";
	result += _table.name;
	return result;
}