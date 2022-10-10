#pragma once

#include <string>
#include <vector>

enum class table_field_properties {
	no_properties   = 0x0,
	primary_key     = 0x1,
	auto_increment  = 0x2,
};

// TODO: use to_underlying
inline table_field_properties operator|(table_field_properties lhs, table_field_properties rhs)
{
	return table_field_properties(unsigned(lhs)|unsigned(rhs));
}

inline table_field_properties operator&(table_field_properties lhs, table_field_properties rhs)
{
	return table_field_properties(unsigned(lhs)&unsigned(rhs));
}

struct table_field {
	std::string name;
	std::string type;
	table_field_properties properies = table_field_properties::no_properties;
};

struct table_description {
	std::string name;
	std::vector<table_field> fields;
};

/*!
 * \brief Builds SQL queries based on the table description
 */
class query_builder
{
public:
	query_builder(const table_description& table)
		: _table(table)
	{
	}

	query_builder(const query_builder& other) = default;
	query_builder& operator=(const query_builder& other) = default;

	/*!
	 * \brief Helper class that allows to build an insert query incrementally row by row.
	 */
	class insert_query {
	public:
		insert_query& add_row(const std::vector<std::string>& fields);

		operator std::string() const;

	private:
		friend query_builder;

		explicit insert_query(const table_description& table)
			: _table(table)
		{}


		table_description _table;
		std::string _values;
	};

	// TODO: add "columns" parameters to explicitly set selected/inserted columns

	insert_query make_insert_query() const { return insert_query(_table); }

	std::string make_select_query() const;

	std::string make_create_query(bool if_not_exists = false) const;

private:
	table_description _table;
};

