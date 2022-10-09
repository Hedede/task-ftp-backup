#pragma once

#include <string>

struct ftp_connection_parameters {
	//! FTP server host. Optional, default is "localhost"
	std::string host;
	//! FTP server port. Optional, default is 21.
	std::string port;
	//! User name to login to the TCP server. Mandatory.
	std::string user;
	//! Password to login to the TCP server. Optional.
	//! If empty, guest login will be attempted.
	std::string password;
};
