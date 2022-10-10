#include "database/sqlite_connection.h"
#include "database/sqlite_query.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <filesystem>

class SqliteQueryTest : public ::testing::Test {
protected:
	SqliteQueryTest()
	{
		// TODO: delete the database before each test
		db.execute(R"SQL(
			create table test(
				id integer primary key autoincrement,
				field1 text,
				field2 text
			)
		)SQL");
	}

	~SqliteQueryTest()
	{
		db.close();
		std::filesystem::remove("tmp_db");
	}

	sqlite_connection db{"tmp_db"};
};

TEST_F(SqliteQueryTest, test_create_table)
{
	auto result = db.execute("drop table if exists test");
	EXPECT_TRUE(result);

	result = db.execute(R"SQL(
		create table test(
			id integer primary key autoincrement,
			field1 text,
			field2 text
		)
	)SQL");
	EXPECT_TRUE(result);

	result = db.execute(R"SQL(
		create table test(
			id integer primary key autoincrement,
			field1 text,
			field2 text
		)
	)SQL");
	EXPECT_FALSE(result);
}

TEST_F(SqliteQueryTest, test_insert_and_select)
{
	auto result = db.execute("insert into test(field1,field2) values('1', '2')");
	EXPECT_TRUE(result);

	result = db.execute("select field1,field2 from test");
	EXPECT_TRUE(result);
	int rowCount = 0;
	for (; result.has_next(); result.next())
	{
		EXPECT_EQ(result.data(0).to_int(), 1);
		EXPECT_EQ(result.data(1).to_string(), "2");
		++rowCount;
	}
	EXPECT_EQ(rowCount, 1);

	result = db.execute("insert into test(field1,field2) values('A', 'B')");
	EXPECT_TRUE(result);

	result = db.execute("select field1,field2 from test");
	rowCount = 0;
	for (; result.has_next(); result.next())
	{
		// TODO: check values
		++rowCount;
	}
	EXPECT_EQ(rowCount, 2);
}

TEST_F(SqliteQueryTest, test_invalid_insert)
{
	auto result = db.execute("insert into test2(field1,field2) values('1', '2')");
	EXPECT_FALSE(result);
}
