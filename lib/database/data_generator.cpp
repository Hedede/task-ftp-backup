#include "data_generator.h"

#include <cstdlib>
#include <ctime>

data_generator::data_generator(const std::vector<data_field>& fields)
	: _fields(fields)
{
	// TODO: add random_number_generator class for unit testing
	// TODO: replace rand() with random_number_generator
	srand(time(nullptr));
}

std::vector<std::string> data_generator::generate_row()
{
	std::vector<std::string> result;
	for (const auto& field : _fields)
	{
		auto value = rand() / double(RAND_MAX);
		if (value < field.null_probability)
			result.emplace_back();
		else
			result.push_back(field.data[rand() % field.data.size()]);
	}
	return result;
}
