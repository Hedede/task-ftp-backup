#include "options.h"

#include "utility/command_line_parser.h"

#include <iostream>

options parse_options(int argc, char** argv)
{
	command_line_parser parser({
		{ 'h', "show help message", false },
		{ 'H', "host to connect to the FTP server (default localhost)" },
		{ 'P', "port to connect to the FTP server (default 21)" },
		{ 'u', "username (default guest)" },
		{ 'p', "password" },
		{ 'd', "output directory in the FTP server (default /)" },
	});

	auto result = parser.parse_argv(argc, argv);
	if (result.options.contains('h'))
	{
		std::cout << parser.format_help_string("ftp-backup [OPTIONS...] path_to_db");
		exit(1);
	}

	if (result.positional_parameters.empty())
	{
		std::cerr << "Not enough arguments.\n\n";
		std::cerr << parser.format_help_string("ftp-backup [OPTIONS...] path_to_db");
		exit(1);
	}

	options ret;

	ret.db_connection = result.positional_parameters[0];
	ret.host = result.arg('H');
	ret.port = result.arg('P');
	ret.output_dir = result.arg('d', "/");
	ret.username = result.arg('u', "guest");
	ret.password = result.arg('p');

	return ret;
}
