PROJECT (XTL)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET (XTL_MAJOR_VERSION   "1")
SET (XTL_MINOR_VERSION   "0")
SET (XTL_RELEASE_VERSION "4")

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

# SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic")
# -std=c++0x
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic -fPIC -D_DEBUG")

OPTION (BUILD_PGSQL "Build PostgreSQL library wrapper" ON)

#############################################################################################################

INCLUDE_DIRECTORIES (
	lib
	tests
)

ADD_SUBDIRECTORY (lib)

ADD_EXECUTABLE (
	forge
		tests/forge.cpp
		tests/TemplateParser.cpp
		tests/TemplateTreeBuilder.cpp
)

TARGET_LINK_LIBRARIES (
	forge
		xtl
		xtl-pgsql
)

ADD_EXECUTABLE (
	expr
		tests/expr.cpp
		tests/Expression.cpp
		tests/ExpressionOperator.cpp
		tests/SyntaxAnalyzer.cpp
)

TARGET_LINK_LIBRARIES (
	expr
		xtl
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
		xtl
)


ADD_EXECUTABLE (
	json_parser
		tests/json_parser.cpp
)

TARGET_LINK_LIBRARIES (
	json_parser
		xtl
)

ADD_EXECUTABLE (
	html_tokenizer
		tests/html_tokenizer.cpp
)

TARGET_LINK_LIBRARIES (
	html_tokenizer
		xtl
)

#ADD_EXECUTABLE (
#	mysql_test
#		tests/mysql_test.cpp
#)

#TARGET_LINK_LIBRARIES (
#	mysql_test
#		xtl
#		xtl-mysql
#)

ADD_EXECUTABLE (
	sqlite_test
		tests/sqlite_test.cpp
)

TARGET_LINK_LIBRARIES (
	sqlite_test
		xtl
		xtl-sqlite
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

ADD_EXECUTABLE (
	test_fork_exec
		tests/test_fork_exec.cpp
)

TARGET_LINK_LIBRARIES (
	test_fork_exec
		xtl
)

ADD_EXECUTABLE (
	regexp_test
		tests/regexp_test.cpp
)

TARGET_LINK_LIBRARIES (
	regexp_test
		xtl
)


ADD_SUBDIRECTORY (unittests)
ADD_SUBDIRECTORY (rpm)
