#pragma once

#include <string>

#include "sqlite3_fwd.h"

class sqlite_query;

/*!
 * \brief Wrapper for a SQLite connection.
 */
class sqlite_connection
{
public:
	explicit sqlite_connection(const std::string& path);

	~sqlite_connection();

	sqlite_connection(const sqlite_connection& other) = delete;
	sqlite_connection& operator=(const sqlite_connection& other) = delete;

	/*!
	 * \brief Close the database.
	 */
	void close();

	/*!
	 * \brief Execute an SQL query.
	 * \param query String containing the SQL query.
	 * \note Supports only single statement queries.
	 *       Only the first statement will be executed, the rest ignored.
	 */
	sqlite_query execute(std::string_view query);

private:
	sqlite3* _db = nullptr;
};

