#include "query_builder.h"

#include "utility/string_utils.h"

auto query_builder::insert_query::add_row(const std::vector<std::string>& fields) -> insert_query&
{
	// Wrap the value in ''
	static const auto add = [] (auto& sink, const std::string& value) {
		if (value.empty())
		{
			sink += "NULL";
		}
		else
		{
			sink += '\'';
			sink += value;
			sink += '\'';
		}
	};

	if (!_values.empty())
		_values += ",\n";
	_values += '(';
	_values += join(fields, add);
	_values += ')';
	return *this;
}

static const auto append_field_name = [] (auto& sink, const table_field& field) {
	sink += field.name;
};

static std::vector<table_field> remove_auto_increment_fields(std::vector<table_field> fields)
{
	using enum table_field_properties;
	auto new_end = std::remove_if(fields.begin(), fields.end(), [](const table_field& field) { return (field.properies & auto_increment) == auto_increment; });
	fields.erase(new_end, fields.end());
	return fields;
}

query_builder::insert_query::operator std::string() const
{
	std::string result = "insert into ";
	result += _table.name;
	result += '(';
	result += join(remove_auto_increment_fields(_table.fields), append_field_name);
	result += ')';
	result += " values ";
	result += _values;
	return result;
}

std::string query_builder::make_select_query() const
{
	// TODO: add support for where, orber by, join
	std::string result = "select ";
	result += join(_table.fields, append_field_name);
	result += " from ";
	result += _table.name;
	return result;
}

std::string query_builder::make_create_query(bool if_not_exists) const
{
	// TODO: add support for different types
	static const auto append_field = [] (auto& sink, const table_field& field) {
		sink += field.name;
		sink += ' ';
		sink += field.type;

		using enum table_field_properties;
		if ((field.properies & primary_key) == primary_key)
			sink += " primary key";
		if ((field.properies & auto_increment) == auto_increment)
			sink += " autoincrement";
	};

	std::string result = "create table ";
	if (if_not_exists)
		result += "if not exists ";
	result += _table.name;
	result += '(';
	result += join(_table.fields, append_field);
	result += ')';
	return result;
}
