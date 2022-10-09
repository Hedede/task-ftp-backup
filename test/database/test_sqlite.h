#include "sqlite_connection.h"
#include "sqlite_query.h"
#include <iostream>

bool test_sqlite()
{
	sqlite_connection conn("/tmp/db");

	auto result = conn.execute(R"SQL(
		drop table if exists test;
		create table if test(
			id integer primary key autoincrement,
			field1 text,
			field2 text
		)
	)SQL");
	if (!result)
		return false;

	result = conn.execute("insert into test(field1,field2) values('1', '2')");
	if (!result)
		return false;

	int rowCount = 0;

	result = conn.execute("select * from test");
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
