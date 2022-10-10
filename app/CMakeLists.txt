
add_executable(ftp-backup
	random_data.h
	options.h
	options.cpp
	dump.h
	dump.cpp
	main.cpp)

target_link_libraries(ftp-backup
	ftp-backup-lib
	sqlite3)

install(TARGETS ftp-backup
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
