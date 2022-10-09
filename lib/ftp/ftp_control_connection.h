#pragma once

#include "network/tcp_socket.h"
#include "ftp_connection_parameters.h"

class ftp_control_connection
{
public:
	explicit ftp_control_connection(const std::string& host = {}, const std::string port = {});

	struct response {
		int code;
		std::string text;

		//! Returns true if response is positive (no error occured)
		explicit operator bool() const { return code < 400 || code >= 600; }
	};

	response receive_response();

	void send_command(std::string_view command, std::string_view parameter = {});

	response send_command_with_reply(std::string_view command, std::string_view parameter = {})
	{
		send_command(command, parameter);
		return receive_response();
	}

private:
	void authorize(const std::string& user, const std::string& password);

	tcp_socket _socket;
};

