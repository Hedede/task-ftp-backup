#pragma once

#include <optional>
#include <charconv>

#ifdef _MSC_VER
#include <format>
namespace fmt {
using std::format;
}
#else
#include <fmt/format.h>
#endif

inline std::optional<int> to_int(std::string_view str)
{
	int result = 0;

	const auto first = str.data();
	const auto end   = str.data() + str.size();
	const auto status = std::from_chars(first, end, result);

	if (status.ec == std::errc())
		return result;

	return std::nullopt;
}
