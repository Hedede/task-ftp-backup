#pragma once

#include "ftp_connection_parameters.h"

#include <memory>

class ftp_control_connection;

class ftp_client
{
public:
	explicit ftp_client(const ftp_connection_parameters& parameters);

	~ftp_client();

	void send_data(const std::string& path, const std::string& data);
	void send_file(const std::string& path, const std::string& file);

private:
	std::unique_ptr<ftp_control_connection> _control;
};

