PROJECT (XTL)

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

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic")
# -std=c++0x
# SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic -D_DEBUG")

#############################################################################################################

INCLUDE_DIRECTORIES (
	src
)

ADD_LIBRARY (
	nginx_leveldb SHARED
		src/LevelDB.cpp
)

TARGET_LINK_LIBRARIES (
	nginx_leveldb
		leveldb
		pthread
		snappy
)

#############################################################################################################

ADD_EXECUTABLE (
	simpletest
		simpletest.c
)

TARGET_LINK_LIBRARIES (
	simpletest
		nginx_leveldb
)

#############################################################################################################

ADD_EXECUTABLE (
	test_threads
		test_threads.c
)

TARGET_LINK_LIBRARIES (
	test_threads
		nginx_leveldb
)

#############################################################################################################

INSTALL (
	TARGETS
		nginx_leveldb
	DESTINATION
		lib
)

INSTALL (
	FILES
		conf/nginx-leveldb.conf
	RENAME
		nginx-leveldb.conf.example
	DESTINATION
		/usr/share/nginx-leveldb/conf
)
