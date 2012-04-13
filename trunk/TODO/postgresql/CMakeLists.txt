PROJECT (POSTGRES)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

ADD_LIBRARY (
	xc3postgres SHARED
	PostgreSqlConnection.cpp
	PostgreSqlResult.cpp
	PostgreSqlQuery.cpp
	PostgreSqlQueryParams.cpp
)

INSTALL (
	TARGETS
		xc3postgres
	DESTINATION
		lib
	OPTIONAL
)
