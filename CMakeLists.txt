project(xbmc-joystick-client)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH				"${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS 				"${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXPORT_COMPILE_COMMANDS 	ON)

file(
	COPY ${CMAKE_SOURCE_DIR}/resources
	DESTINATION ${CMAKE_BINARY_DIR}
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SRC
	src/joystick.cpp
	src/main.cpp
	src/xbmcjoystick.cpp
)

set(HDR
	include/joystick.h
	include/xbmcclient.h
	include/xbmcjoystick.h
)

add_executable(
	xbmc-joystick-client
	${SRC}
	${HDR}
)

target_link_libraries(
	xbmc-joystick-client
	m
	pthread
)