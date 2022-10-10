#pragma once

#include "utility/command_line_parser.h"

#include <gtest/gtest.h>

TEST(TestCmdParser, mixed_test)
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

	EXPECT_EQ(result.options.size(), 2);
	EXPECT_EQ(result.positional_parameters.size(), 2);
	EXPECT_EQ(result.options['o'], "file");
	EXPECT_EQ(result.options['i'], "file");
	EXPECT_EQ(result.positional_parameters[0], "+test");
	EXPECT_EQ(result.positional_parameters[1], "-test");
}
