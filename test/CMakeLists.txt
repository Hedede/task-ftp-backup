find_package(GTest REQUIRED)

add_executable(ftp-backup-test
	utility/test_cmd_parser.cpp
	network/test_socket.cpp
	database/test_query_builder.cpp
	database/test_sqlite.cpp
	ftp/test_ftp.cpp
)

target_link_libraries(ftp-backup-test
	PRIVATE
		ftp-backup-lib
		sqlite3
		GTest::gtest
		GTest::gtest_main
)
