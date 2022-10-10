#include "database/query_builder.h"
#include "utility/string_utils.h"

#include <gtest/gtest.h>

using enum table_field_properties;

class QueryBuilderTest : public ::testing::Test {
protected:
	const query_builder builder{{
		.name = "table",
		.fields = {
			 {"id", "integer", primary_key|auto_increment},
			 {"field1", "text"},
			 {"field2", "text"}
		}
	}};
};

TEST_F(QueryBuilderTest, test_simple_insert)
{
	std::string str = builder.make_insert_query()
		.add_row({"1", "a1", "a2"})
		.add_row({"2", "b1", "b2"});

	// Remove case and scapes to account for variances in output
	tolower(remove_spaces(str));

	EXPECT_EQ( str, "insertintotable(id,field1,field2)values('1','a1','a2'),('2','b1','b2')");

}

TEST_F(QueryBuilderTest, test_simple_select)
{
	std::string str = builder.make_select_query();

	tolower(remove_spaces(str));

	EXPECT_EQ( str, "selectid,field1,field2fromtable");
}

TEST_F(QueryBuilderTest, test_create_table)
{
	std::string str = builder.make_create_query();

	tolower(remove_spaces(str));

	EXPECT_EQ( str, "createtabletable(idintegerprimarykeyautoincrement,field1text,field2text)");
}
