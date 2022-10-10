#pragma once

#include <string>

/*!
 * \brief Structure for storing input parameters for the application.
 */
struct options {
	//! FTP server host
	std::string host;
	//! FTP server port
	std::string port;
	//! User name to login with (empty = attemp anonymous login)
	std::string username;
	//! Password to login with
	std::string password;
	//! Database connection string
	std::string db_connection;
	//! Directory path ont he FTP server where SQL dump will be stored
	std::string output_dir;
};

options parse_options(int argc, char** argv);
