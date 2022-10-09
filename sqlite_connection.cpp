#include "sqlite_connection.h"

#include "sqlite_query.h"

#include <sqlite3.h>

sqlite_connection::sqlite_connection(const std::string& path)
{
	sqlite3_open( path.data(), &_db );
}

sqlite_error sqlite_connection::last_error()
{
	return {
		sqlite3_errmsg( _db ),
		sqlite3_errcode( _db )
	};
}

sqlite_connection::~sqlite_connection()
{
	sqlite3_close( _db );
}

sqlite_query sqlite_connection::execute(std::string_view query)
{
	sqlite3_stmt* stmt = nullptr;
	auto rc = sqlite3_prepare_v2( _db, query.data(), query.size(), &stmt, nullptr);
	if ( rc != SQLITE_OK ) {
		// TODO
		//std::cerr << sqlite3_errmsg( _db ) << '\n';
	}

	return sqlite_query(stmt);
}
