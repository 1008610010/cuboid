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
		xtl/Logger.cpp
		xtl/PrintStream.cpp
		xtl/ProgramOptions.cpp
		xtl/RegExp.cpp
		xtl/StringUtils.cpp
		xtl/Variant.cpp
		xtl/VariantArray.cpp
		xtl/VariantPtr.cpp
		xtl/VariantScalar.cpp
		xtl/VariantStruct.cpp

		xtl/ini/IniConfig.cpp
		xtl/ini/IniConfigParser.cpp

		xtl/json/JsonPrinter.cpp

		xtl/linux/AccessMode.cpp
		xtl/linux/Signals.cpp
		xtl/linux/SystemConfig.cpp
		xtl/linux/UnixError.cpp
		xtl/linux/UnixUser.cpp
		xtl/linux/Utils.cpp
		xtl/linux/fs/DirectoryReader.cpp
		xtl/linux/fs/DirectoryReaderRecursive.cpp
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

		xtl/linux/utils/Daemonize.cpp
		xtl/linux/utils/Execute.cpp
		xtl/linux/utils/PidFile.cpp
		xtl/linux/utils/SocketServer.cpp
		xtl/linux/utils/TcpSocketClient.cpp
		xtl/linux/utils/TcpSocketServer.cpp
		xtl/linux/utils/UnixSocketClient.cpp
		xtl/linux/utils/UnixSocketServer.cpp

		xtl/plain/Algorithm.cpp
		xtl/plain/RecordPrototype.cpp
		xtl/plain/RecordPrototypeBuilder.cpp
		xtl/plain/RecordArray.cpp

		xtl/utils/Base64.cpp
		xtl/utils/BitUtils.cpp
		xtl/utils/Chronometer.cpp
		xtl/utils/Date.cpp
		xtl/utils/SimpleStringMatcher.cpp
		xtl/utils/StringsTable.cpp
		xtl/utils/StringsTablePrinter.cpp

		xtl/io/FileInputStream.cpp
		xtl/io/FileInputStreamBuffered.cpp
		xtl/io/FileOutputStream.cpp
		xtl/io/FileOutputStreamBuffered.cpp
		xtl/io/FileTransaction.cpp
		xtl/io/StringOutputStream.cpp

		xtl/tp/CharClass.cpp
		xtl/tp/CharClassifier.cpp
		xtl/tp/CharSource.cpp
		xtl/tp/NumberBuilder.cpp
		xtl/tp/NumberLiteralParser.cpp
		xtl/tp/Parser.cpp
		xtl/tp/StringLiteralParser.cpp
		xtl/tp/TextCharSource.cpp

		xtl/simple_template/SimpleTemplate.cpp
		xtl/simple_template/SimpleTemplateItem.cpp
		xtl/simple_template/SimpleTemplateWriter.cpp
)

ADD_LIBRARY (
	xtl SHARED
		${XTL_SOURCES}
)

TARGET_LINK_LIBRARIES (
	xtl
		boost_regex
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
		NAMES
			mysqlclient
		PATHS
			/opt/lib
			/usr/lib
			/usr/local/lib
			/usr/lib64/mysql
			/usr/lib/i386-linux-gnu
			/usr/lib/x86_64-linux-gnu
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



FIND_LIBRARY (
	SQLITE_LIBRARY
		NAMES
			sqlite3
		PATHS
			/opt/lib
			/usr/lib
			/usr/local/lib
			/usr/lib64/mysql
			/usr/lib/i386-linux-gnu
			/usr/lib/x86_64-linux-gnu
		DOC "Path where the SQLite library can be found"
)

SET (
	XTL_SQLITE_SOURCES
		xtl/sqlite/Database.cpp
		xtl/sqlite/Exception.cpp
		xtl/sqlite/Statement.cpp
)

ADD_LIBRARY (
	xtl-sqlite SHARED
		${XTL_SQLITE_SOURCES}
)

TARGET_LINK_LIBRARIES (
	xtl-sqlite
		xtl
		${SQLITE_LIBRARY}
)



INSTALL (
	TARGETS
		xtl
		xtl-mysql
		xtl-pgsql
		xtl-sqlite
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


