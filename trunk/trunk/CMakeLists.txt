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
	xc3_dump_users_table
		tests/xc3_dump_users_table.cpp
		lib/xc3/db/ReaderWeight.cpp
		lib/xc3/db/UsersTableLoader.cpp
)

TARGET_LINK_LIBRARIES (
	xc3_dump_users_table
		xtl-pgsql-st
		xtl-common-st
)

ADD_EXECUTABLE (
	xc3_calc_post_ratings
		tests/xc3_calc_post_ratings.cpp
		lib/xc3/ratings/UsersArray.cpp
		lib/xc3/ratings/PostRating.cpp
		lib/xc3/ratings/PostRatingMassFilter.cpp
)

TARGET_LINK_LIBRARIES (
	xc3_calc_post_ratings
		xtl-common-st
)



#	--cpp_out=${GEN_SRC_PATH}
# ADD_CUSTOM_COMMAND (
# 	TARGET xc3_import_users
# 	PRE_BUILD
# 	COMMAND /home/dnikolaev/Dropbox/Projects/cxx/xtl/generate-source.pl ${GEN_SRC_PATH}
# 	COMMENT "Generating file"
# )

ADD_EXECUTABLE (
	comment_stats_reader
		tests/comment_stats_reader.cpp
)

TARGET_LINK_LIBRARIES (
	comment_stats_reader
		xtl-common
)

ADD_EXECUTABLE (
	calc_rating
		tests/calc_rating.cpp
)

TARGET_LINK_LIBRARIES (
	calc_rating
		xtl-common
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

