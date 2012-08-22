PROJECT (XTL)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET (XTL_MAJOR_VERSION   "1")
SET (XTL_MINOR_VERSION   "0")
SET (XTL_RELEASE_VERSION "0")

SET (XTL_FULL_VERSION "${XTL_MAJOR_VERSION}.${XTL_MINOR_VERSION}.${XTL_RELEASE_VERSION}")

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
	lib
	tests
)

ADD_SUBDIRECTORY (lib)

#ADD_EXECUTABLE (
#	forge
#		tests/forge.cpp
#)

#TARGET_LINK_LIBRARIES (
#	forge
#		xtl-pgsql
#		xtl
#)



SET (GEN_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/gen_src")
FILE (MAKE_DIRECTORY ${GEN_SRC_PATH})

INCLUDE_DIRECTORIES (
	${GEN_SRC_PATH}
)



ADD_EXECUTABLE (
	program_options
		tests/program_options.cpp
)

TARGET_LINK_LIBRARIES (
	program_options
		xtl
)


ADD_EXECUTABLE (
	mysql_test
		tests/mysql_test.cpp
)

TARGET_LINK_LIBRARIES (
	mysql_test
		xtl
		xtl-mysql
)


ADD_EXECUTABLE (
	ljstatsd
		tests/ljstatsd.cpp
		tests/lj-stats-writer/StatsWriterServer.cpp
)

TARGET_LINK_LIBRARIES (
	ljstatsd
		xtl
)


ADD_EXECUTABLE (
	ljstats_client
		tests/ljstats_client.cpp
		tests/lj-stats-writer/StatsWriterClient.cpp
)

TARGET_LINK_LIBRARIES (
	ljstats_client
		xtl
)


ADD_SUBDIRECTORY (unittests)
ADD_SUBDIRECTORY (rpm)



