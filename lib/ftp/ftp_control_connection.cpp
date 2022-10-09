#include "ftp_control_connection.h"

#include "utility/format.h"

#include <iostream>
#include <stdexcept>

// TODO: enum with response codes

[[noreturn]] static void throw_error(std::string message)
{
	throw std::runtime_error( message );
}

static const std::string& coalesce(const std::string& parameter, const std::string& default_value)
{
	return parameter.empty() ? default_value : parameter;
}

ftp_control_connection::ftp_control_connection(const ftp_connection_parameters& parameters)
	: _socket(
		coalesce(parameters.host, "localhost"),
		coalesce(parameters.port, "21")
	)
{
	const auto result = receive_response();
	if (result.code == 220)
	{
		authorize(parameters.user, parameters.password);
	}

	// TODO: handle unexpected response
}

void ftp_control_connection::send_command(std::string_view command, std::string_view parameter)
{
	std::string message(command);
	if (!parameter.empty())
	{
		message += ' ';
		message += parameter;
	}
	if (!message.ends_with("\r\n"))
		message += "\r\n";
	_socket.send(message);
}

ftp_control_connection::response ftp_control_connection::receive_response()
{
	const auto msg = _socket.receive();

#ifdef FTP_DEBUG
	std::cout << msg << std::endl;
#endif

	//Response alawys starts with a result code
	if (msg.size() < 3)
		throw_error("Response is too short: " + std::string(msg));

	const auto code = to_int(msg.substr(0, 3));
	if (!code)
		throw_error("Incorrect response: " + std::string(msg));

	return {
		.code = *code,
		.text = std::string(msg.substr(4))
	};
}

// TODO: maybe move to the ftp_client?
void ftp_control_connection::authorize(const std::string& user, const std::string& password)
{
	// When no password supplied, try anonymous login
	send_command("USER", password.empty() ? "anonymous" : user);

	const auto result = receive_response();
	if (result.code == 331)
	{
		// "Guest login ok, type your name as password."
		send_command("PASS", password.empty() ? user : password);
	}

	const auto re = receive_response();
}
