#include "sqlite_dump.h"

#include "sqlite_query.h"
#include "query_builder.h"

#include "utility/format.h"

sqlite_dump::sqlite_dump(const std::string& path_to_db)
	: _conn(path_to_db)
{
}

auto sqlite_dump::get_db_objects() -> std::vector<db_object>
{
	std::vector<db_object> objects;

	auto result = _conn.execute("SELECT type,name,sql FROM sqlite_master");
	for (; result.has_next(); result.next())
	{
		db_object object;
		object.name = result.data(1).to_string();
		if (object.name.starts_with("sqlite_"))
			continue;
		object.type = result.data(0).to_string();
		object.sql  = result.data(2).to_string();

		objects.push_back(std::move(object));
	}

	return objects;
}

std::string sqlite_dump::make_table_sql_dump(std::string_view table_name)
{
	auto result = _conn.execute(fmt::format("SELECT * FROM {}", table_name));

	table_description desc;
	desc.name = std::string(table_name);

	const int column_count = result.column_count();
	for (int i = 0; i < column_count; ++i)
	{
		desc.fields.push_back({ result.column(i) });
	}

	query_builder builder(desc);

	auto query = builder.make_insert_query();
	for (; result.has_next(); result.next())
	{
		std::vector<std::string> values(column_count);
		for (int i = 0; i < column_count; ++i)
		{
			values[i] = result.data(i).to_string();
		}
		query.add_row(values);
	}

	return query;
}

std::string sqlite_dump::make_sql_dump()
{
	std::string sql;

	const auto objects = get_db_objects();

	for (const auto& object : objects)
	{
		sql += object.sql;
		sql += ";\n";

		if (object.type == "table")
		{
			sql += make_table_sql_dump(object.name);
			sql += ";\n";
		}
	}

	return sql;
}
