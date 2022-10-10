#include <iostream>

#include "options.h"
#include "dump.h"

#include "ftp/ftp_client.h"

#include <iostream>
#include <filesystem>

std::string make_output_path(const std::string& input_file, const std::string& output_dir)
{
	std::filesystem::path input_path(input_file);
	std::filesystem::path directory_path(output_dir);
	return (directory_path / input_path.filename()).string();
}

int main(int argc, char** argv)
{
	try {
		const auto options = parse_options(argc, argv);

		const auto path_to_dump = make_dump(options.db_connection);

		ftp_client ftp({
			.host = options.host,
			.port = options.port,
			.user = options.username,
			.password = options.password
		});

		ftp.send_file(make_output_path(path_to_dump, options.output_dir), path_to_dump);

	} catch (std::exception& ex) {
		std::cerr << "error: " << ex.what() << '\n';
	}

	return 0;
}
