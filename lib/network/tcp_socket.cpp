#include "tcp_socket.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
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

// TODO: use wchar_t
[[noreturn]] static void throw_error(std::string where)
{
#ifdef _WIN32
	auto strerror = [](int errc)
	{
		char* str = nullptr;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&str, 0, NULL);
		auto msg = std::string(str);
		LocalFree(str);
		return msg;
	};

	const auto errc = WSAGetLastError();
	throw std::runtime_error(where + ": " + strerror(errc));
#else
	throw std::runtime_error(where + ": " + strerror(errno));
#endif
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
	if (addr == nullptr || ret < 0) {
		// TODO: differentiate between invalid_argument and runtime_error
		using namespace std::string_literals;
		throw std::runtime_error("get_addrinfo: "s +
#ifdef _WIN32
			// TODO: use wchar
			gai_strerrorA(ret)
#else
			gai_strerror(ret)
#endif
		);
	}

	return { addr, freeaddrinfo };
}

tcp_socket::tcp_socket(const std::string& address, const std::string& port)
{
#ifdef _WIN32
	static bool wsa_initialized = false;
	if (!wsa_initialized)
	{
		WSAData data;
		WORD version = MAKEWORD(2, 2);
		auto ret = WSAStartup(version, &data);
		if (ret){
			throw_error("WSAStartup");
		} else {
			atexit([] { WSACleanup(); });
			wsa_initialized = true;
		}
	}
#endif

	const auto addr = get_addrinfo(address, port);

	_fd = ::socket(addr->ai_family, SOCK_STREAM, IPPROTO_TCP);
	if (_fd < 0)
		throw_error("socket");

	// Use the first available address for simplicity
	const int res = ::connect(_fd, addr->ai_addr, addr->ai_addrlen);
	if (res < 0)
		throw_error("Could not connect to " + address + ":" + port);

	_buf.resize(1024);
}

tcp_socket::~tcp_socket()
{
#ifdef _WIN32
	::closesocket(_fd);
#else
	::close(_fd);
#endif
}

std::string_view tcp_socket::receive()
{
	int ret = ::recv(_fd, _buf.data(), _buf.size(), 0);
	if (ret < -1)
		throw_error("read");

	return std::string_view( _buf.data(), static_cast<size_t>(ret) );
}

void tcp_socket::send(std::string_view message)
{
	do {
		int res = ::send(_fd, message.data(), message.size(), 0);
		if (res == -1)
			throw_error("write");

		message.remove_prefix(res);
	} while(!message.empty());
}
