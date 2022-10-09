#include "sqlite_connection.h"
#include "sqlite_query.h"

#include <stdexcept>

#define CHECK_RESULT() \
	if (!result) \
		throw std::runtime_error(std::string(result.error()));

bool test_sqlite()
{
	sqlite_connection conn("/tmp/db");

	auto result = conn.execute("drop table if exists test");
	CHECK_RESULT();

	conn.execute(R"SQL(
		create table test(
			id integer primary key autoincrement,
			field1 text,
			field2 text
		)
	)SQL");
	CHECK_RESULT();

	result = conn.execute("insert into test(field1,field2) values('1', '2')");
	CHECK_RESULT();

	result = conn.execute("select field1,field2 from test");
	CHECK_RESULT();

	int rowCount = 0;
	for (; result.has_next(); result.next())
	{
		if (result.data(0).to_int() != 1)
			return false;
		if (result.data(1).to_string() != "2")
			return false;
		++rowCount;
	}

	return rowCount == 1;
}
