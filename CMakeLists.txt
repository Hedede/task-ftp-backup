cmake_minimum_required(VERSION 3.5)

project(ftp-backup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_executable(ftp-backup
	app/random_data.h
	main.cpp)

target_link_libraries(ftp-backup
	ftp-backup-lib
	sqlite3)

install(TARGETS ftp-backup
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_subdirectory(test)
