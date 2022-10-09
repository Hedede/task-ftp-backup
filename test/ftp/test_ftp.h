#pragma once

#include <iostream>
#include <cstdio>

#include "ftp/ftp_client.h"


void test_ftp()
{
	ftp_client client({
		.user = "hudd",
		.password = ""
	});

	client.send_data("/tmp/dump1.txt", "1234567890");
	client.send_data("/tmp/dump2.txt", "qwertyuiop");
}
