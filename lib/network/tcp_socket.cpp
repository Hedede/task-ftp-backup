#include "tcp_socket.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <memory>

[[noreturn]] void throw_error(std::string where)
{
	throw std::runtime_error( where + ": " + strerror(errno) );
}

using addrinfo_ptr = std::unique_ptr<
	addrinfo,
	void(*)(struct addrinfo *)
>;

/*!
 * \brief Parse host and port.
 * \param address Remote host address
 * \param port Remote host port
 * \return Pointer to the addrinfo struct
 */
static addrinfo_ptr get_addrinfo(const std::string& address, const std::string& port)
{
	struct addrinfo hints = {
		.ai_family   = PF_INET,
		.ai_socktype = SOCK_STREAM,
	};

	struct addrinfo* addr = nullptr;
	auto ret = getaddrinfo(address.data(), port.data(), &hints, &addr);
	if (ret < 0) {
		using namespace std::string_literals;
		throw std::runtime_error( "get_addrinfo: "s + gai_strerror(ret) );
	}

	return { addr, freeaddrinfo };
}

tcp_socket::tcp_socket(const std::string& address, const std::string& port)
{
	const auto addr = get_addrinfo(address, port);

	_fd = ::socket(addr->ai_family, SOCK_STREAM, IPPROTO_TCP);
	if (_fd < 0)
		throw_error("socket");

	// Use the first available address for simplicity
	const int res = ::connect(_fd, addr->ai_addr, addr->ai_addrlen);
	if (res < 0)
		throw_error("connect");

	_buf.resize(1024);
}

tcp_socket::~tcp_socket()
{
	close(_fd);
}

std::string_view tcp_socket::receive()
{
	int ret = read(_fd, _buf.data(), _buf.size());
	if (ret < -1)
		throw_error("read");

	return std::string_view( _buf.data(), static_cast<size_t>(ret) );
}

void tcp_socket::send(std::string_view message)
{
	int err = write(_fd, message.data(), message.size());
	if (err == -1)
		throw_error("write");
}
