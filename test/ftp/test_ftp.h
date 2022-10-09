#pragma once

#include "ftp/ftp_control_connection.h"

#include "utility/format.h"

#include <iostream>
#include <cstdio>

struct pasv {
	std::string ip;
	int port = 0;
};

std::string_view get_pasv_parameters(std::string_view message)
{
	auto start = message.find('(');
	if (start == message.npos)
		return {};
	++start; // skip '('
	const auto end = message.find(')', start);
	if (end == message.npos)
		return {};
	return message.substr(start, end - start);
}

pasv parse_pasv(std::string_view message)
{
	message = get_pasv_parameters(message);

	size_t pos = 0;
	int bits[6] = {};
	for (int i = 0; i < 6; ++i)
	{
		auto next = message.find(',', pos);
		bits[i] = *to_int(message.substr(pos, next));
		if (next == message.npos)
			break;
		pos = next+1;
	}

	pasv result = {
		.ip = fmt::format("{}.{}.{}.{}", bits[0], bits[1], bits[2], bits[3]),
		.port = (bits[4] << 8) + bits[5]
	};
	return result;
}


void test_ftp()
{
	ftp_control_connection control({
		.user = "hudd",
		.password = ""
	});

	control.send_command("PASV");

	auto reply = control.receive_response();

	auto pasv = parse_pasv(reply.text);
	std::cout << pasv.ip << " " << pasv.port << std::endl;
	{
		tcp_socket data_socket(pasv.ip, std::to_string(pasv.port));

		control.send_command("STOR", "/tmp/dump.txt");
		control.receive_response();

		data_socket.send("12345");
	}
	control.receive_response();

	control.send_command("QUIT");
	control.receive_response();
}
