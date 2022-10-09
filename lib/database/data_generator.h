#pragma once

#include <string>
#include <vector>

// For ease of unit testing I use arrays of strings instead of some more compilcated rules

struct data_field {
	std::string name;

	std::vector<std::string> data;
	double null_probability = 0.0;
};

class data_generator
{
public:
	data_generator(const std::vector<data_field>& fields);

	std::vector<std::string> generate_row();

private:
	std::vector<data_field> _fields;
};

