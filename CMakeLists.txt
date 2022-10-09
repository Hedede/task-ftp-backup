cmake_minimum_required(VERSION 3.5)

project(ftp-backup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ftp-backup-lib STATIC
	lib/database/sqlite3_fwd.h
	lib/database/sqlite_connection.h
	lib/database/sqlite_connection.cpp
	lib/database/sqlite_query.h
	lib/database/sqlite_query.cpp
	lib/database/data_generator.h
	lib/database/data_generator.cpp
	lib/utility/format.h
	lib/ftp/ftp_connection_parameters.h
	lib/ftp/ftp_client.h
	lib/ftp/ftp_client.cpp
	lib/ftp/ftp_control_connection.h
	lib/ftp/ftp_control_connection.cpp
	lib/network/tcp_socket.h
	lib/network/tcp_socket.cpp)

target_include_directories(ftp-backup-lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/lib
)


add_executable(ftp-backup
	app/random_data.h
	main.cpp)

target_link_libraries(ftp-backup
	ftp-backup-lib
	sqlite3)

install(TARGETS ftp-backup
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_subdirectory(test)
