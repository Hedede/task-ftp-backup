#pragma once

#include "sqlite_connection.h"

#include <vector>
#include <string>

class sqlite_dump
{
public:
	explicit sqlite_dump(const std::string& path_to_db);

	struct db_object {
		std::string type;
		std::string name;
		std::string sql;
	};

	std::vector<db_object> get_db_objects();

	std::string make_table_sql_dump(std::string_view table_name);
	std::string make_sql_dump();

private:
	sqlite_connection _conn;
};

