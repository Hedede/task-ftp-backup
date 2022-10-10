#include "utility/string_utils.h"
#include "utility/format.h"

#include <gtest/gtest.h>

TEST(StringUtilsTest, test_remove_spaces)
{
	std::string s1 = "123\t45 67  8\n9";
	std::string s2 = "123456789";
	std::string s3;

	EXPECT_EQ(remove_spaces(s1), "123456789");
	EXPECT_EQ(remove_spaces(s2), "123456789");
	EXPECT_EQ(remove_spaces(s3), "");
}

TEST(StringUtilsTest, test_tolower)
{
	std::string s1 = "ABC def GH";
	std::string s2 = "abcdefgh";
	std::string s3;


	EXPECT_EQ(tolower(s1), "abc def gh");
	EXPECT_EQ(tolower(s2), "abcdefgh");
	EXPECT_EQ(tolower(s3), "");
}

TEST(StringUtilsTest, test_to_int)
{
	std::string s1 = "65000";
	std::string s2 = "-50000";
	std::string s3 = "";
	std::string s4 = "fs8714u";

	EXPECT_EQ(to_int(s1), 65000);
	EXPECT_EQ(to_int(s2), -50000);
	EXPECT_EQ(to_int(s3), std::nullopt);
	EXPECT_EQ(to_int(s4), std::nullopt);
}

TEST(StringUtilsTest, test_join)
{
	std::vector<std::string> v1;
	std::vector<std::string> v2 = { "asd", "", "123", "4567" };
	std::vector<std::string> v3 = { "asd4567" };

	EXPECT_EQ(join(v1), "");
	EXPECT_EQ(join(v2), "asd,,123,4567");
	EXPECT_EQ(join(v3), "asd4567");
}
