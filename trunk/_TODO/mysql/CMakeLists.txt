PROJECT (XC3MYSQL)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic -fPIC -D_DEBUG ")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${APXS_CFLAGS} ${APXS_CFLAGS_SHLIB} -static-all -fPIC")

#############################################################################################################

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

INCLUDE(MacroFindLib)

FIND_PACKAGE(MySql)

SET (MYSQL_INCLUDE_DIR "${MYSQL_INCLUDE_DIR}" CACHE STRING "MySQL Include Directory")
SET (MYSQL_LIBRARIES "${MYSQL_LIBRARIES}" CACHE STRING "MySQL Libraries")

MESSAGE (STATUS "MYSQL_INCLUDE_DIR = ${MYSQL_INCLUDE_DIR}")
MESSAGE (STATUS "MYSQL_LIBRARIES = ${MYSQL_LIBRARIES}")

INCLUDE_DIRECTORIES (
	${MYSQL_INCLUDE_DIR}
)

ADD_LIBRARY (
	xc3mysql STATIC
		MySqlBaseConnection.cpp
		MySqlConnection.cpp
		MySqlStableConnection.cpp
		MySqlLibrary.cpp
		MySqlResult.cpp
		../xtl/StringUtils.cpp
)

TARGET_LINK_LIBRARIES (
	xc3mysql
		${MYSQL_LIBRARIES}
)

#INSTALL (TARGETS xc3mysql DESTINATION lib)
