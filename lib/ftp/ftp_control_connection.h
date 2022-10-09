#pragma once

#include "network/tcp_socket.h"
#include "ftp_connection_parameters.h"

class ftp_control_connection
{
public:
	explicit ftp_control_connection(const ftp_connection_parameters& parameters);

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

