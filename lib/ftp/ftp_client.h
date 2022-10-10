#pragma once

#include "ftp_connection_parameters.h"

#include <memory>

class ftp_control_connection;

/*!
 * \brief Implements a FTP client. Currently supports only passive mode file transfer.
 */
class ftp_client
{
public:
	/*!
	 * \brief Establish connection to a FTP server.
	 * \param parameters Connection parameters.
	 *                   See the ftp_connection_parameters structure for more information.
	 * \throws runtime_error when a connection fails for whatever reason
	 */
	explicit ftp_client(const ftp_connection_parameters& parameters);

	~ftp_client();

	/*!
	 * \brief Write binary data to the specified file on the remote server.
	 * \param path Path to file.
	 * \param data Data to write.
	 * \throws runtime_error on connection error
	 */
	void send_data(const std::string& path, const std::string& data);

	/*!
	 * \brief Copy specified file to the FTP server.
	 * \param path Path to file on the remote server.
	 * \param data Path to the local file.
	 * \throws runtime_error on connection error
	 */
	void send_file(const std::string& path, const std::string& file);

private:
	struct passive_connection {
		std::string ip;
		std::string port;
	};

	passive_connection parse_passive_connection_parameters(std::string_view message);
	passive_connection establish_passive_connection();

	void authorize(const std::string& user, const std::string& password);

	std::unique_ptr<ftp_control_connection> _control;
};

