#include "ftp_client.h"

#include "ftp/ftp_control_connection.h"

#include "utility/format.h"

#include <fstream>

[[noreturn]] static void throw_error(std::string message)
{
	throw std::runtime_error( message );
}


ftp_client::ftp_client(const ftp_connection_parameters& parameters)
	: _control(new ftp_control_connection(parameters.host, parameters.port))
{
	// Receive the welcome message
	_control->receive_response();

	authorize(parameters.user, parameters.password);

	// Switch to binary mode
	_control->send_command_with_reply("TYPE", "I");
}

ftp_client::~ftp_client()
{
	_control->send_command("QUIT");
	_control->receive_response();
}

void ftp_client::authorize(const std::string& user, const std::string& password)
{
	// When no password supplied, try anonymous login
	auto result = _control->send_command_with_reply("USER", password.empty() ? "anonymous" : user);
	if (result.code == 331)
	{
		// If password is empty, pass user because
		// "Guest login ok, type your name as password."
		result = _control->send_command_with_reply("PASS", password.empty() ? user : password);
	}

	if (!result)
		throw_error(result.text);

}

static std::string_view get_pasv_parameters(std::string_view message)
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

static std::vector<std::string_view> split(std::string_view message, char delimiter = ',')
{
	std::vector<std::string_view> result;

	int count = 1;
	for (char c : message) {
		if (c == delimiter)
			++count;
	}
	result.resize(count);

	size_t pos = 0;
	for (int i = 0; i < count; ++i)
	{
		auto next = message.find(',', pos);
		result[i] = message.substr(pos, next - pos);
		if (next == message.npos)
			break;
		pos = next+1;
	}

	return result;
}

static unsigned short parse_port(std::string_view high, std::string_view low)
{
	const auto high_num = *to_int(high);
	const auto low_num = *to_int(low);
	return static_cast<unsigned short>((high_num << 8) + low_num);
}

auto ftp_client::parse_passive_connection_parameters(std::string_view message) -> passive_connection
{
	message = get_pasv_parameters(message);

	// response has format of "(ip1,ip2,ip3,ip4,port1,port2)"
	const auto bits = split(message);
	if (bits.size() < 6)
		throw_error("Incorrect response");

	auto port = parse_port(bits[4], bits[5]);

	passive_connection result;
	result.ip   = fmt::format("{}.{}.{}.{}", bits[0], bits[1], bits[2], bits[3]);
	result.port = std::to_string(port);
	return result;
}

auto ftp_client::establish_passive_connection() -> passive_connection
{
	auto result = _control->send_command_with_reply("PASV");
	if (!result)
		throw_error(result.text);

	return parse_passive_connection_parameters(result.text);
}


void ftp_client::send_data(const std::string& path, const std::string& data)
{
	const auto conn = establish_passive_connection();
	{
		tcp_socket data_socket(conn.ip, conn.port);

		const auto result = _control->send_command_with_reply("STOR", path);
		if (!result)
			throw_error("Failed to send file: " + result.text);

		data_socket.send(data);
	}
	_control->receive_response();
}

void ftp_client::send_file(const std::string& path, const std::string& file_path)
{
	std::ifstream file;
	file.open(file_path);
	if (!file.is_open())
		throw_error("Could not open the file specified: " + file_path);

	const auto conn = establish_passive_connection();
	{
		tcp_socket data_socket(conn.ip, conn.port);

		const auto result = _control->send_command_with_reply("STOR", path);
		if (!result)
			throw_error("Failed to send file: " + result.text);

		std::vector<char> chunk(1024);
		while (!file.eof()) {
			file.read(chunk.data(), chunk.size());
			data_socket.send(std::string_view(chunk.data(), file.gcount()));
		}
	}
	_control->receive_response();
}
