#pragma once

#include <string>
#include <vector>

class tcp_socket
{
public:
	/*!
	 * \brief Connect to a remote server with specified address and port
	 * \param address Remote server address. Can be a hostname or an IP address.
	 * \param port TCP port number.
	 * \throws runtime_error on connection error
	 */
	tcp_socket(const std::string& address, const std::string& port);

	~tcp_socket();

	/*!
	 * \brief Receive data from remote host.
	 * \return View of received data.
	 * \warning Subsequent calls to this method will invalidate the contents of previously
	 *          returned string_view!
	 * \throws runtime_error on connection error
	 */
	std::string_view receive();

	/*!
	 * \brief Send message to remote host.
	 * \param message
	 * \throws runtime_error on connection error
	 */
	void send(std::string_view message);

private:
	void connect(const std::string& address, const std::string& port);

	int _fd = -1;

	std::vector<char> _buf;
};

