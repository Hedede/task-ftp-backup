add_executable(ftp-backup-test
	main.cpp
	ftp/test_ftp.h
	network/test_socket.h
	database/test_query_builder.h
	database/test_sqlite.h)

target_link_libraries(ftp-backup-test
	ftp-backup-lib
	sqlite3)
