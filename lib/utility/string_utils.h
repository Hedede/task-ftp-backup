#pragma once

#include <algorithm>
#include <cctype>
#include <string>

static std::string& remove_spaces(std::string& str)
{
	auto new_end = std::remove_if(str.begin(), str.end(), [] (char c) { return std::isspace(c); });
	str.erase(new_end, str.end());
	return str;
}

static std::string& tolower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [] (char c) { return std::tolower(c); } );
	return str;
}
