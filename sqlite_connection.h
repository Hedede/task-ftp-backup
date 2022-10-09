#pragma once

#include <string>

extern "C" {
typedef struct sqlite3 sqlite3;
}

struct sqlite_error {
	const char* msg;
	int         code;
};

class sqlite_query;

class sqlite_connection
{
public:
	explicit sqlite_connection(const std::string& path);

	~sqlite_connection();

	sqlite_connection(const sqlite_connection& other) = delete;
	sqlite_connection& operator=(const sqlite_connection& other) = delete;

	sqlite_error last_error();

	/*!
	 * \brief Execute an SQL query.
	 * \param query String containing the SQL query.
	 */
	sqlite_query execute(std::string_view query);

private:
	sqlite3* _db = nullptr;
};

