#pragma once

#include <string>
#include <vector>

struct table_description {
	std::string name;
	std::vector<std::string> fields;
};

class query_builder
{
public:
	query_builder(const table_description& table)
		: _table(table)
	{
	}

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

	// TODO: add "columns" parameters to insert only specified columns

	insert_query make_insert_query() const { return insert_query(_table); }

	std::string make_select_query() const;

	std::string make_create_query(bool if_not_exists = false) const;

private:
	table_description _table;
};

