#pragma once

#include <string>
#include <vector>

class tcp_socket
{
public:
	tcp_socket(const std::string& address, const std::string& port);

	~tcp_socket();

	/*!
	 * \brief Receive data from remote host.
	 * \return View of received data.
	 * \warning Subsequent calls to this method will invalidate the contents of previously
	 *          returned string_view!
	 */
	std::string_view receive();

	void send(std::string_view message);

private:
	void connect(const std::string& address, const std::string& port);

	int _fd = -1;

	std::vector<char> _buf;
};

