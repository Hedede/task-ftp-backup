#include "sqlite_connection.h"

#include "sqlite_query.h"

#include <sqlite3.h>

sqlite_connection::sqlite_connection(const std::string& path)
{
	sqlite3_open( path.data(), &_db );
}

sqlite_connection::~sqlite_connection()
{
	sqlite3_close( _db );
}

sqlite_query sqlite_connection::execute(std::string_view query)
{
	return sqlite_query(_db, query);
}
