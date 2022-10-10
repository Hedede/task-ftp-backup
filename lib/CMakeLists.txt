
option(FTP_DEBUG "debug FTP client" OFF)
if(FTP_DEBUG)
	add_definitions(-DFTP_DEBUG)
endif()

add_library(ftp-backup-lib STATIC
	database/sqlite3_fwd.h
	database/sqlite_connection.h
	database/sqlite_connection.cpp
	database/sqlite_query.h
	database/sqlite_query.cpp
	database/data_generator.h
	database/data_generator.cpp
	database/query_builder.h
	database/query_builder.cpp
	utility/format.h
	utility/command_line_parser.h
	utility/command_line_parser.cpp
	ftp/ftp_connection_parameters.h
	ftp/ftp_client.h
	ftp/ftp_client.cpp
	ftp/ftp_control_connection.h
	ftp/ftp_control_connection.cpp
	network/tcp_socket.h
	network/tcp_socket.cpp)

target_include_directories(ftp-backup-lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ftp-backup-lib
	PUBLIC
		sqlite3)

if (UNIX)
	find_package(fmt)
	target_link_libraries(ftp-backup-lib
		PUBLIC
			fmt::fmt)
endif()
