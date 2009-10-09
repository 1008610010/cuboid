PROJECT (CUBOID)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

INCLUDE(CheckIncludeFiles)
INCLUDE(CheckCXXSourceCompiles)

IF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
	MESSAGE(STATUS "#### Building for FreeBSD...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	MESSAGE(STATUS "#### Building for Linux...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
	MESSAGE(STATUS "#### Building for SunOS")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic -D_DEBUG")

INCLUDE_DIRECTORIES(
	.
)

ADD_EXECUTABLE(
	test
	test/test.cpp
	xtl/ByteBuffer.cpp
	xtl/TcpSocket.cpp
	xtl/TcpClient.cpp
	xtl/SocketAddressInet.cpp
)

ADD_EXECUTABLE(
	httpget
	test/httpget.cpp
	xtl/ByteBuffer.cpp
	xtl/TcpSocket.cpp
	xtl/SocketAddressInet.cpp
)

ADD_EXECUTABLE(
	server
	test/server.cpp
	xtl/TcpSocket.cpp
	xtl/SocketAddressInet.cpp
)

ADD_EXECUTABLE (
	json-test
	test/json-test.cpp
	xtl/Json.cpp
	xtl/StringUtils.cpp
	xtl/FileLock.cpp
)

ADD_EXECUTABLE (
	file-lock-test
	test/file-lock-test.cpp
	xtl/FileLock.cpp
)
