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

template <typename Container, typename Add>
std::string join(const Container& vec, Add&& add, char delimiter = ',')
{
	auto begin = vec.cbegin();
	auto end   = vec.cend();

	std::string sink;
	if (begin == end)
		return sink;

	add(sink, *begin++);
	while (begin != end) {
		sink += delimiter;
		add(sink, *begin++);
	}

	return sink;
}

template <typename Container>
std::string join(const Container& vec, char delimiter = ',')
{
	return join(vec, [] (auto& sink, const auto& value) { sink += value; }, delimiter);
}
