add_executable(ftp-backup-test
	main.cpp
	database/test_sqlite.h)

target_link_libraries(ftp-backup-test
	ftp-backup-lib
	sqlite3)
