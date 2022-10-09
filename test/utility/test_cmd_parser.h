#pragma once

#include "utility/command_line_parser.h"

bool test_cmd_parser()
{
	command_line_parser parser({
		{ 'h', "show help message", false },
		{ 'o', "output file path" },
		{ 'i', "input file path" },
	});

	std::vector<std::string> arguments = {
		"program",
		"-o",
		"file",
		"-o",
		"file2",
		"-ofile2",
		"-ifile",
		"-funknown",
		"+test",
		"--",
		"-test",
	};

	std::vector<char*> argv;
	for (auto& arg : arguments)
		argv.push_back(arg.data());

	auto result = parser.parse_argv(argv.size(), argv.data());

	if (result.options.size() != 2)
		return false;
	if (result.positional_parameters.size() != 2)
		return false;
	if (result.options['o'] != "file")
		return false;
	if (result.options['i'] != "file")
		return false;
	if (result.positional_parameters[0] != "+test")
		return false;
	if (result.positional_parameters[1] != "-test")
		return false;

	return true;
}
