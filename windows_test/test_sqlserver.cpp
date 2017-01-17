#include "stdafx.h"
/* #include "afxdb.h" */
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

using namespace std;

int main() {
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLRETURN ret;
	SQLSMALLINT columns;
	int row = 0;

	/* Allocate an environment handle */
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	/* We want ODBC 3 support */
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	/* Allocate a connection handle */
	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

	/* Connect to the DSN */
	SQLDriverConnectW(dbc, NULL, L"DRIVER={SQL Server};SERVER=127.0.0.1,1433;DATABASE=xxx;UID=xxx;PWD=123456;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

	/* Check for success */
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt)) {
		cout << "Failed to connect" << endl;
		goto DONE;
	}

	if (SQL_SUCCESS != SQLExecDirect(stmt, L"select OnlineCount from njb.dbo.TGameRoomInfo where RoomID=9", SQL_NTS)) {
		cout << "Failed to select" << endl;
		goto DONE;
	}
	else {
		int count = 0;
		if (SQLFetch(stmt) == SQL_SUCCESS) {
			SQLGetData(stmt, 1, SQL_C_ULONG, &count, 0, NULL);
			cout << count << endl;
		}
		else {
			cout << "Failed to fetch" << endl;
		}
	}

DONE:
	std::cin.get();
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	SQLDisconnect(dbc);
	SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	SQLFreeHandle(SQL_HANDLE_ENV, env);
	return 0;
}

