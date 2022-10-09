#pragma once

#include "network/tcp_socket.h"

struct ftp_connection_parameters {
	std::string host;
	std::string port;
	//! User name to login to the TCP server. Mandatory.
	std::string user;
	//! Password to login to the TCP server. Optional.
	//! If empty, guest login will be attempted.
	std::string password;
};

class ftp_control_connection
{
public:
	ftp_control_connection(const ftp_connection_parameters& parameters);

	void send_command(std::string_view command, std::string_view parameter = {});

	struct response {
		int code;
		std::string text;
	};

	response receive_response();

private:
	void authorize(const std::string& user, const std::string& password);

	tcp_socket _socket;
};

