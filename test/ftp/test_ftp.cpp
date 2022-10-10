#pragma once

#include <filesystem>

#include "ftp/ftp_client.h"

#include <gtest/gtest.h>

// TODO: for meaningful testing some shell scripting is required
// for now it is a simple test that makes too much assumptions

// And mocking a FTP server provides negative value, it cannot account for all
// the differences between implementations

TEST(FtpTest, test_send)
{
#ifdef _WIN32
	if (!std::filesystem::is_directory("./ftp"))
		GTEST_SKIP();
#endif

	ftp_client client({
		.user = "guest",
		.password = ""
	});

#ifdef _WIN32
	auto path = "./ftp/dump1.txt";
#else
	auto path = "/tmp/ftp/dump1.txt";
#endif

	std::filesystem::remove(path);
	client.send_data("dump1.txt", "1234567890");
	EXPECT_TRUE(std::filesystem::exists(path));
}
