#pragma once

#include <string>

#include "sqlite3_fwd.h"

class sqlite_connection;
class sqlite_query_data;

/*!
 * \brief Result of an SQLite query.
 *        Allows to iterate over rows and retrieve data.
 */
class sqlite_query {
public:
	~sqlite_query();

	sqlite_query(sqlite_query&& other);

	sqlite_query& operator=(sqlite_query&& other);

	//! Signifies if the query was susccessful or not
	explicit operator bool() const;

	bool has_next() const;
	bool next();

	sqlite_query_data data(int index) const;

private:
	friend sqlite_connection;

	explicit sqlite_query(sqlite3* db, std::string_view query);

	sqlite3_stmt* _stmt = nullptr;
	int           _status;
};


class sqlite_query_data {
public:
	bool is_valid() const;

	std::string to_string() const;
	int to_int() const;
	bool to_bool() const;

private:
	friend sqlite_query;

	sqlite_query_data() = default;
	sqlite_query_data(sqlite3_stmt* stmt, int index);

	sqlite3_stmt* _stmt = nullptr;
	int           _index = -1;
};
