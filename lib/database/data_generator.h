#pragma once

#include <string>
#include <vector>

// For ease of unit testing I use arrays of strings instead of some more compilcated rules
// TODO: write tests

struct data_field {
	//! Name of the field. For convenience only, not used internally.
	std::string name;
	//! List of values to randomly choose from
	std::vector<std::string> data;
	//! Probablility that the cell will be null
	double null_probability = 0.0;
};

/*!
 * \brief Generates random rows from specified dataset.
 */
class data_generator
{
public:
	data_generator(const std::vector<data_field>& fields);

	std::vector<std::string> generate_row();

private:
	std::vector<data_field> _fields;
};

