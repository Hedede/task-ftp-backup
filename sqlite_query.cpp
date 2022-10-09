#include "sqlite_query.h"

#include <utility>

#include <sqlite3.h>

sqlite_query::sqlite_query(sqlite3* db, std::string_view query)
{
	// TODO: support for multiple statements
	auto begin = query.data();

	_status = sqlite3_prepare_v2( db, begin, query.size(), &_stmt, &begin);
	if (_status == SQLITE_OK)
		_status = sqlite3_step(_stmt);

}

sqlite_query::~sqlite_query()
{
	// No need to check for nullptr because
	// invoking sqlite3_finalize() on a NULL pointer is a harmless no-op.
	sqlite3_finalize(_stmt);
}

sqlite_query::sqlite_query(sqlite_query&& other)
{
	_stmt   = std::exchange(other._stmt, nullptr);
	_status = std::exchange(other._status, SQLITE_DONE);
}

sqlite_query& sqlite_query::operator=(sqlite_query&& other)
{
	sqlite3_finalize(_stmt);
	_stmt   = std::exchange(other._stmt, nullptr);
	_status = std::exchange(other._status, SQLITE_DONE);
	return *this;
}

sqlite_query::operator bool() const
{
	return _status == SQLITE_DONE ||
	       _status == SQLITE_ROW;
}

bool sqlite_query::has_next() const
{
	return _status == SQLITE_ROW;
}

bool sqlite_query::next()
{
	_status = sqlite3_step( _stmt );
	return has_next();
}

sqlite_query_data sqlite_query::data(int index) const
{
	if (index < 0)
		return {};
	const auto column_count = sqlite3_column_count(_stmt);
	if (index < column_count)
		return sqlite_query_data{ _stmt, index };
	return {};
}

// -----------------------------------------------------------------

sqlite_query_data::sqlite_query_data(sqlite3_stmt* stmt, int index)
	: _stmt(stmt)
	, _index(index)
{}

bool sqlite_query_data::is_valid() const
{
	return _stmt != nullptr;
}

std::string sqlite_query_data::to_string() const
{
	const auto string = sqlite3_column_text( _stmt, _index );
	// unsigned char -> char aliasing is allowed, so
	// use reinterpret_cast here to avoid manually copying the data
	return reinterpret_cast<const char*>(string);
}

int sqlite_query_data::to_int() const
{
	return sqlite3_column_int( _stmt, _index );
}

bool sqlite_query_data::to_bool() const
{
	return static_cast<bool>( to_int() );
}

