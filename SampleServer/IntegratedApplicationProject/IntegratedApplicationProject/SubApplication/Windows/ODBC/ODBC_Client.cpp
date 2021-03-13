#include "stdafx.h"
#include "ODBC_Client.h"

HWND MAIN_WINDOW_HANDLE;

SQLHENV ENVIRONMENT_HANDLE;
SQLHDBC DATABASE_HANDLE;
SQLHSTMT STATEMENT_HANDLE;

int APIENTRY ExecuteODBC_Client(HINSTANCE instanceHandle, HINSTANCE previousInstanceHandle, LPSTR lpCommandLine, int nCommandShow)
{
	return 0;
}

BOOL ODBC_Sample()
{
	SQLHENV environmentHandle;
	SQLHDBC databaseHandle;
	SQLHSTMT statementHandle;

	SQLWCHAR inputConstant[256];
	SQLWCHAR outputConstant[256];
	SQLSMALLINT outputConstantSize;

	SQLSMALLINT outputConstant;

	WCHAR directory[MAX_PATH];

	SQLWCHAR name[32];
	int price;
	bool korean;
	SQLLEN nameLength, priceLength, koreanLength;
	

	SQLAllocHandle(SQL_HANDLE_DBC, environmentHandle, &databaseHandle);

	GetCurrentDirectory(MAX_PATH, directory);

	wsprintf((WCHAR*)inputConstant, L"DRIVER={MySQL 8.0 Driver (*.mdb)};" "DBQ=%s\\cigarette.mdb;", directory);

	SQLDriverConnect(databaseHandle, MAIN_WINDOW_HANDLE, inputConstant, sizeof(inputConstant), outputConstant, sizeof(outputConstant), &outputConstantSize, SQL_DRIVER_NOPROMPT);

	SQLAllocHandle(SQL_HANDLE_STMT, databaseHandle, &statementHandle);

	SQLBindCol(statementHandle, 1, SQL_C_WCHAR, name, sizeof(name), &nameLength);
	SQLBindCol(statementHandle, 2, SQL_C_WCHAR, &price, 0, &priceLength);
	SQLBindCol(statementHandle, 3, SQL_C_WCHAR, &korean, 0, &koreanLength);

	SQLExecDirect(statementHandle, (SQLWCHAR*)"select name, price, korean from tblCigar", SQL_NTS);

	while (SQLFetch(statementHandle) != SQL_NO_DATA);
	{
		wsprintf();
	}

	SQLCloseCursor(statementHandle);
	SQLFreeHandle(SQL_HANDLE_STMT, statementHandle);
	SQLDisconnect(databaseHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, databaseHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, environmentHandle);
}

BOOL ODBC_Connect()
{
	SQLWCHAR inputConstant[256];
	SQLWCHAR outputConstant[256];
	SQLSMALLINT outputConstantSize;
	WCHAR directory[MAX_PATH];
	SQLRETURN result;

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &ENVIRONMENT_HANDLE) != SQL_SUCCESS) return 0;

	if (SQLSetEnvAttr(ENVIRONMENT_HANDLE, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return 0;

	if (SQLAllocHandle(SQL_HANDLE_DBC, ENVIRONMENT_HANDLE, &DATABASE_HANDLE) != SQL_SUCCESS) return 0;


	GetCurrentDirectory(MAX_PATH, directory);

	wsprintf((WCHAR*)inputConstant, L"DRIVER={MySQL 8.0 Driver (*.mdb)};" "DBQ=%s\\cigarette.mdb;", directory);
	result = SQLDriverConnect(DATABASE_HANDLE, MAIN_WINDOW_HANDLE, inputConstant, sizeof(inputConstant), outputConstant, sizeof(outputConstant), &outputConstantSize, SQL_DRIVER_NOPROMPT);

	if ((result != SQL_SUCCESS) && (result != SQL_SUCCESS_WITH_INFO)) return 0;

	if (SQLAllocHandle(SQL_HANDLE_STMT, DATABASE_HANDLE, &STATEMENT_HANDLE) != SQL_SUCCESS) return 0;

	return 1;
}

void ODBC_Disconnect()
{
	if (STATEMENT_HANDLE) SQLFreeHandle(SQL_HANDLE_STMT, STATEMENT_HANDLE);
	if (DATABASE_HANDLE) SQLDisconnect(DATABASE_HANDLE);
	if (DATABASE_HANDLE) SQLFreeHandle(SQL_HANDLE_DBC, DATABASE_HANDLE);
	if (ENVIRONMENT_HANDLE) SQLFreeHandle(SQL_HANDLE_ENV, ENVIRONMENT_HANDLE);
}

BOOL ODBC_SendQuery(SQLCHAR* query)
{
	SQLCHAR name[32];
	int price;
	BOOL korean;
	SQLLEN nameLength, priceLength, koreanLength;

	SQLBindCol(STATEMENT_HANDLE, 1, SQL_C_WCHAR, name, sizeof(name), &nameLength);
	SQLBindCol(STATEMENT_HANDLE, 2, SQL_C_ULONG, &price, 0, &priceLength);
	SQLBindCol(STATEMENT_HANDLE, 3, SQL_C_ULONG, &korean, 0, &koreanLength);

	if (SQLExecDirect(STATEMENT_HANDLE, (SQLWCHAR*)"select name, price, korean from tblCigar", SQL_NTS) != SQL_SUCCESS) return 0;

	if (STATEMENT_HANDLE) SQLCloseCursor(STATEMENT_HANDLE);

	return 1;
}