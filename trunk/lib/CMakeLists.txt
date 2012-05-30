PROJECT (XTL)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

INCLUDE(CheckIncludeFiles)
INCLUDE(CheckCXXSourceCompiles)

#TODO remove hardcode

IF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
	MESSAGE(STATUS "#### Building for FreeBSD...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	MESSAGE(STATUS "#### Building for Linux...")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
	MESSAGE(STATUS "#### Building for SunOS")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic -fPIC")
# -std=c++0x
# SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wundef -Wsign-compare -Wpointer-arith -Wparentheses -Wno-long-long -pedantic -D_DEBUG")

#############################################################################################################

SET (
	XTL_SOURCES
		xtl/Exception.cpp
		xtl/FormatString.cpp
		xtl/StringUtils.cpp
		xtl/Variant2.cpp

		xtl/linux/AccessMode.cpp
		xtl/linux/UnixError.cpp
		xtl/linux/fs/File.cpp
		xtl/linux/fs/FilePath.cpp
		xtl/linux/fs/FilePathTokenizer.cpp
		xtl/linux/fs/FileStats.cpp
		xtl/linux/fs/FileUtils.cpp

		xtl/linux/net/ClientSocket.cpp
		xtl/linux/net/ServerSocket.cpp
		xtl/linux/net/Socket.cpp
		xtl/linux/net/SocketAddressError.cpp
		xtl/linux/net/SocketAddressInet.cpp
		xtl/linux/net/SocketAddressUnix.cpp
		xtl/linux/net/SocketSelector.cpp
		xtl/linux/net/SocketSet.cpp
		xtl/linux/net/TcpSocket.cpp
		xtl/linux/net/UnixSocket.cpp

		xtl/plain/Algorithm.cpp
		xtl/plain/RecordPrototype.cpp
		xtl/plain/RecordPrototypeBuilder.cpp
		xtl/plain/RecordArray.cpp
		xtl/plain/Struct.cpp
		xtl/plain/StructPrototype.cpp
		xtl/plain/StructPrototypeBuilder.cpp

		xtl/utils/BitUtils.cpp
		xtl/utils/Chronometer.cpp
		xtl/utils/Date.cpp

		xtl/io/FileInputStream.cpp
		xtl/io/FileInputStreamBuffered.cpp
		xtl/io/FileOutputStream.cpp
		xtl/io/FileOutputStreamBuffered.cpp
		xtl/io/StringOutputStream.cpp

		xtl/tp/CharClassifier.cpp
		xtl/tp/TextCharSource.cpp
		xtl/tp/TextParser.cpp

		xtl/simple_template/SimpleTemplate.cpp
		xtl/simple_template/SimpleTemplateItem.cpp
		xtl/simple_template/SimpleTemplateWriter.cpp
)

ADD_LIBRARY (
	xtl SHARED
		${XTL_SOURCES}
)



FIND_LIBRARY (
	POSTGRESQL_CLIENT_LIBRARY
		NAMES pq
		PATHS /opt/lib /usr/lib /usr/local/lib
		DOC "Path where the PostgreSQL client library can be found"
)

SET (LIBPQ_FE_H libpq-fe.h)

FIND_PATH (
	LIBPQ_FE_INCLUDE_DIR
	${LIBPQ_FE_H}
	PATH
		/usr/local/include
		/usr/include
		/usr/include/postgresql
		NO_DEFAULT_PATH
	DOC
		"Searching ${LIBPQ_FE_H}"
)

IF (LIBPQ_FE_INCLUDE_DIR)
	MESSAGE (STATUS "${LIBPQ_FE_H} found in ${LIBPQ_FE_INCLUDE_DIR}")
ELSE (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
	MESSAGE (FATAL_ERROR "${LIBPQ_FE_H} NOT found")
ENDIF (LIBPQ_FE_INCLUDE_DIR)

MARK_AS_ADVANCED (
	LIBPQ_FE_INCLUDE_DIR
)

INCLUDE_DIRECTORIES (
	${LIBPQ_FE_INCLUDE_DIR}
)

SET (
	XTL_PGSQL_SOURCES
		xtl/pgsql/Connection.cpp
		xtl/pgsql/ConnectionConfig.cpp
		xtl/pgsql/Cursor.cpp
		xtl/pgsql/Exception.cpp
		xtl/pgsql/QueryResult.cpp
)

ADD_LIBRARY (
	xtl-pgsql SHARED
		${XTL_PGSQL_SOURCES}
)

TARGET_LINK_LIBRARIES (
	xtl-pgsql
		${POSTGRESQL_CLIENT_LIBRARY}
)



FIND_LIBRARY (
	MYSQL_CLIENT_LIBRARY
		NAMES mysqlclient
		PATHS /opt/lib /usr/lib /usr/local/lib /usr/lib64/mysql
		DOC "Path where the MySQL client library can be found"
)

SET (
	XTL_MYSQL_SOURCES
		xtl/mysql/Connection.cpp
		xtl/mysql/Exception.cpp
		xtl/mysql/QueryResult.cpp
		xtl/mysql/QueryResultRow.cpp
)

ADD_LIBRARY (
	xtl-mysql SHARED
		${XTL_MYSQL_SOURCES}
)

TARGET_LINK_LIBRARIES (
	xtl-mysql
		xtl
		${MYSQL_CLIENT_LIBRARY}
)



INSTALL (
	TARGETS
		xtl
		xtl-mysql
		xtl-pgsql
	DESTINATION
		lib
)

INSTALL (
	DIRECTORY   xtl
	DESTINATION include
	PATTERN "*.hpp"
	PATTERN "*.cpp" EXCLUDE
	PATTERN ".svn" EXCLUDE
	PATTERN "todo" EXCLUDE
	PATTERN "TODO" EXCLUDE
)



