#pragma once

#include "database/query_builder.h"

#include <algorithm>
#include <cctype>

void remove_spaces(std::string& str)
{
	auto new_end = std::remove_if(str.begin(), str.end(), [] (char c) { return std::isspace(c); });
	str.erase(new_end, str.end());
}

bool test_query_builder()
{
	query_builder builder{{
		.name = "table",
		.fields = { "id", "field1", "field2" }
	}};

	std::string str = builder.make_insert_query()
		.add_row({"1", "a1", "a2"})
		.add_row({"2", "b1", "b2"});

	remove_spaces(str);
	if (str != "insertintotable(id,field1,field2)values('1','a1','a2'),('2','b1','b2')")
		return false;

	str = builder.make_select_query();
	remove_spaces(str);

	if (str != "selectid,field1,field2fromtable")
		return false;

	return true;
}
