#include "command_line_parser.h"

#include <cctype>

command_line_parser::command_line_parser(const std::vector<command_line_option>& options)
{
	for (const auto& option : options)
	{
		_options.insert({ option.symbol, option });
	}
}

auto command_line_parser::parse_argv(int argc, char** argv) const -> parse_result
{
	parse_result result;

	auto arg = [&] (int i) { return (i < argc) ? argv[i] : nullptr; };

	bool positional_only = false;

	// Start with 1 because argv[0] is usually the program name
	for (int i = 1; i < argc; ++i)
	{
		auto* argp = arg(i);
		if (!argp)
			break;

		std::string_view value(argp);
		if (positional_only || !value.starts_with('-') || value.size() < 2) {
			result.positional_parameters.push_back(value);
			continue;
		}

		const auto symbol = value[1];

		// Handle '--' (separates options from positional parameters)
		if (symbol == '-')
		{
			positional_only = true;
			continue;
		}

		value.remove_prefix(2); // remove '-o'


		auto it = _options.find(symbol);
		if (it == _options.end())
		{
			// always assume that unknown option has an argument
			// TODO: maybe it is better to throw invalid_argument
			if (value.empty())
				++i;
			continue;
		}

		const auto& option = it->second;
		if (!option.has_argument && !value.empty())
			// TODO: throw invalid_argument
			continue;

		if (option.has_argument && value.empty())
		{
			auto* argp = arg(++i);
			if (!argp)
				break;
			value = argv[i];
		}

		result.options.insert({symbol, value});
	}

	return result;
}

std::string command_line_parser::format_help_string(std::string_view description) const
{
	std::string result;
	result += "Usage:\n";
	if (!description.empty()){
		result += description;
		result += "\n\n";
	}

	for (const auto& [_,option] : _options)
	{
		if (!isprint(option.symbol))
			continue;
		result += "  -";
		result += option.symbol;
		result += ": ";
		// TODO: Split description into multiple lines if it is too long
		result += option.description;
		result += '\n';
	}

	return result;
}
