#include "ftp_client.h"

#include "ftp/ftp_control_connection.h"

#include "utility/format.h"

#include <fstream>

[[noreturn]] static void throw_error(std::string message)
{
	throw std::runtime_error( message );
}


ftp_client::ftp_client(const ftp_connection_parameters& parameters)
	: _control(new ftp_control_connection(parameters))
{
}

ftp_client::~ftp_client()
{
	_control->send_command("QUIT");
	_control->receive_response();
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

struct pasv {
	std::string ip;
	std::string port;
};

static pasv parse_passive_connection_parameters(std::string_view message)
{
	message = get_pasv_parameters(message);

	// response has format of "(ip1,ip2,ip3,ip4,port1,port2)"
	const auto bits = split(message);
	if (bits.size() < 6)
		throw_error("Incorrect response");

	auto port = parse_port(bits[4], bits[5]);

	pasv result;
	result.ip   = fmt::format("{}.{}.{}.{}", bits[0], bits[1], bits[2], bits[3]);
	result.port = std::to_string(port);
	return result;
}


void ftp_client::send_data(const std::string& path, const std::string& data)
{
	_control->send_command("PASV");

	const auto conn = parse_passive_connection_parameters(_control->receive_response().text);
	{
		tcp_socket data_socket(conn.ip, conn.port);

		_control->send_command("STOR", path);
		_control->receive_response();

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

	_control->send_command("PASV");

	const auto conn = parse_passive_connection_parameters(_control->receive_response().text);
	{
		tcp_socket data_socket(conn.ip, conn.port);

		_control->send_command("STOR", path);
		_control->receive_response();

		std::vector<char> chunk(1024);
		while (!file.eof()) {
			file.read(chunk.data(), chunk.size());
			data_socket.send(std::string_view(chunk.data(), file.gcount()));
		}
	}
	_control->receive_response();
}
