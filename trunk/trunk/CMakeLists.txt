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
	lib
	tests
)

ADD_SUBDIRECTORY (lib)

ADD_EXECUTABLE (
	forge
		tests/forge.cpp
)

TARGET_LINK_LIBRARIES (
	forge
		xtl-pgsql
		xtl-common
)



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
		xtl-common
)


ADD_SUBDIRECTORY (unittests)


#INSTALL (
#	TARGETS
#		xc3_calc_ratings
#		xc3_ratings_fetch_journals
#		xc3_ratings_fetch_posts
#		xc3_ratings_read_journal
#		xc3_ratings_read_post
#		xc3_ratings_read_header
#	DESTINATION
#		bin
#)

INSTALL (
	DIRECTORY   lib/xtl
	DESTINATION include
	PATTERN "*.hpp"
	PATTERN "*.cpp" EXCLUDE
	PATTERN ".svn" EXCLUDE
)



