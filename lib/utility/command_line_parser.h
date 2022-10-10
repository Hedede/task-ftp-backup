#pragma once

#include <vector>
#include <string>
#include <map>

/*!
 * \brief The option structure describes a command line option.
 *
 * Currently supports only short (-o) options.
 */
struct command_line_option {
	//! Symbol representing the option.
	//! Must be a printable character.
	char symbol;
	//! Description displayed in the help section
	std::string description;
	//! Does this option have an argument or not
	//! Argument be written with (-o "file") or without space (-ofile)
	bool has_argument = true;
};

/**
 * \brief Simple command line parser. Supports only short (-o) options.
 */
class command_line_parser
{
public:
	/*!
	 * \brief Initialize the command line parser
	 * \param options List of known options. Only short options are supported.
	 */
	explicit command_line_parser(const std::vector<command_line_option>& options);

	struct parse_result {
		/*!
		 * \brief Map of found options and their values.
		 * If an option is found multiple times the first value is stored;
		 */
		std::map<char, std::string_view> options;

		/*!
		 * \brief List of positional parameter (i.e. parameters that don't start with -o)
		 * Everything that goes after '--' is considered a positional parameter.
		 */
		std::vector<std::string_view> positional_parameters;

		std::string arg(char option, std::string_view default_value = {}) const
		{
			const auto it = options.find(option);
			const auto value = it == options.end() ? default_value : it->second;
			return std::string(value);
		}
	};

	/*!
	 * \brief Parses the command line and returns found options with their values.
	 * \param argc Number of parameters passed to main.
	 * \param argv Pointer to array of parameters passed to main.
	 *             Must have argc+1 elements with last element set to nullptr.
	 * \throws invalid_argument if an unknown option is encountered
	 * \return Map of options and list of positional parameters.
	 */
	parse_result parse_argv(int argc, char** argv) const;

	/*!
	 * \brief Formats a string that can be displayed as a short help for a user.
	 * \param description (Optional) Short description of the program.
	 * \return Formatted help string. Only printable options are displayed.
	 */
	std::string format_help_string(std::string_view description = {}) const;

private:
	std::map<char, command_line_option> _options;
};
