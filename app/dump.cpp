#include "dump.h"

#include "random_data.h"

#include "utility/format.h"

#include "database/data_generator.h"
#include "database/sqlite_connection.h"
#include "database/sqlite_dump.h"
#include "database/sqlite_query.h"
#include "database/query_builder.h"

#include <chrono>
#include <stdexcept>
#include <filesystem>
#include <fstream>

std::string make_insert(const query_builder& builder)
{
	auto copy_vector = [] (const auto& src)
	{
		return std::vector<std::string>(src.begin(), src.end());
	};

	data_generator gen({
		{
			.name = "firstname",
			.data = copy_vector(first_names),
		},
		{
			.name = "lastname",
			.data = copy_vector(last_names),
		},
		{
			.name = "address",
			.data = copy_vector(street_names),
			.null_probability = 0.5,
		},
		{
			.name = "phone",
			.data = copy_vector(phone_numbers),
			.null_probability = 0.3,
		},
	});

	auto query = builder.make_insert_query();
	for (int i = 0; i < 100; ++i)
	{
		query.add_row(gen.generate_row());
	}

	return query;
}

void create_schema(const std::string& path_to_db)
{
	sqlite_connection db(path_to_db);

	query_builder builder(
		{
			.name = "employees",
			.fields = {
				{"firstname", "text"},
				{"lastname", "text"},
				{"address", "text"},
				{"phone", "text"},
			},
		}
	);

	auto result = db.execute(builder.make_create_query(true));
	if (!result)
		throw std::runtime_error(result.error().data());

	result = db.execute(make_insert(builder));
	if (!result)
		throw std::runtime_error(result.error().data());

}

static std::string make_timestamp()
{
	//TODO: not supported by GCC/clang yet...
	//auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string buf(30, '\0');
	std::strftime(&buf[0], buf.size(), "%Y%m%d%H%M%S", std::localtime(&now));
	// get rid of extra \0s
	return buf.data();
}

static void dump_database_sql(const std::string& path_to_db, const std::string& output_path)
{
	sqlite_dump dump(path_to_db);
	const auto sql = dump.make_sql_dump();

	std::ofstream of(output_path);
	of.write(sql.data(), sql.size());
	if (of.bad())
		throw std::runtime_error("Could not write sql dump");
}

std::string make_dump(const std::string& path_to_db)
{
	const std::filesystem::path path = path_to_db;

	create_schema(path_to_db);

	auto output_path = fmt::format("{}_{}.sql", path.filename().string(), make_timestamp());

	dump_database_sql(path_to_db, output_path);

	return output_path;
}
