#include "stdafx.h"
#include "ODBC_Client.h"

SQLHENV ENVIRONMENT_HANDLE;
SQLHDBC DATABASE_HANDLE;
SQLHSTMT STATEMENT_HANDLE;

int ExecuteODBC_Client(int argumentCount, char* argumentVector[])
{
	setlocale(LC_ALL, "");

	WCHAR state[1024];
	WCHAR message[1024];
	SQLSMALLINT messageLength;
	int nativeErrorCode;

	WCHAR realnameBuffer[48];
	WCHAR passwordBuffer[48];
	WCHAR nicknameBuffer[48];
	WCHAR scoreBuffer[48];

	SQLLEN realnameLength;
	SQLLEN passwordLength;
	SQLLEN nicknameLength;
	SQLLEN scoreLength;

	SQLLEN size = SQL_LEN_DATA_AT_EXEC(512);
	SQLPOINTER token;

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &ENVIRONMENT_HANDLE) != SQL_SUCCESS) return 0;
	if (SQLSetEnvAttr(ENVIRONMENT_HANDLE, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return 0;

	if (SQLAllocHandle(SQL_HANDLE_DBC, ENVIRONMENT_HANDLE, &DATABASE_HANDLE) != SQL_SUCCESS) return 0;
	
	memset(message, 0, 1024);
	messageLength = 0;
	SQLGetDiagRec(SQL_HANDLE_DBC, DATABASE_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);
	wprintf(L"%s\n", message);

	SQLConnect(DATABASE_HANDLE, (SQLWCHAR*)"guest", SQL_NTS, (SQLWCHAR*)NULL, SQL_NTS, (SQLWCHAR*)NULL, SQL_NTS);

	SQLWCHAR inputConstant[1024] = L"Driver={MySQL ODBC 8.0 Unicode Driver}; Server=127.0.0.1; Database=homework; User=guest; Password=; Option=3;";
	SQLWCHAR outputConstant[256];
	SQLSMALLINT outputConstantSize;

	SQLDriverConnect(DATABASE_HANDLE, NULL, (SQLWCHAR*)inputConstant, wcslen(inputConstant), outputConstant, sizeof(outputConstant) / sizeof(SQLWCHAR), &outputConstantSize, SQL_DRIVER_NOPROMPT);


	SQLGetDiagRec(SQL_HANDLE_DBC, DATABASE_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);
	wprintf(L"%s\n", message);

	if (SQLAllocHandle(SQL_HANDLE_STMT, DATABASE_HANDLE, &STATEMENT_HANDLE) != SQL_SUCCESS) return 0;

	SQLBindCol(STATEMENT_HANDLE, 1, SQL_C_WCHAR, realnameBuffer, 48, (SQLLEN*)&realnameLength);
	SQLBindCol(STATEMENT_HANDLE, 2, SQL_C_WCHAR, passwordBuffer, 48, (SQLLEN*)&passwordBuffer);
	SQLBindCol(STATEMENT_HANDLE, 3, SQL_C_WCHAR, nicknameBuffer, 48, (SQLLEN*)&nicknameLength);
	SQLBindCol(STATEMENT_HANDLE, 4, SQL_C_WCHAR, scoreBuffer, 48, (SQLLEN*)scoreLength);
	SQLGetDiagRec(SQL_HANDLE_STMT, STATEMENT_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);

	SQLExecDirect(STATEMENT_HANDLE, (SQLWCHAR*)L"SELECT * from account", SQL_NTS);
	SQLGetDiagRec(SQL_HANDLE_STMT, STATEMENT_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);

	while (SQLFetch(STATEMENT_HANDLE) != SQL_NO_DATA)
	{
		wprintf(L"blahblah");
		SQLGetDiagRec(SQL_HANDLE_STMT, STATEMENT_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);

		SQLHSTMT statementHandle;
		HRESULT resultHandle;
		WCHAR buffer[48];
		SQLLEN bufferLength;

		if (SQLAllocHandle(SQL_HANDLE_STMT, DATABASE_HANDLE, &statementHandle) != SQL_SUCCESS) return 0;
		resultHandle = SQLBindCol(statementHandle, 1, SQL_C_WCHAR, buffer, 48, (SQLLEN*)&bufferLength);

		resultHandle = SQLExecDirect(statementHandle, (SQLWCHAR*)"SELECT accoutkey FROM account", SQL_NTS);
		SQLGetDiagRec(SQL_HANDLE_STMT, STATEMENT_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);

		while (SQLFetch(statementHandle) != SQL_NO_DATA)
		{
			wprintf(L"blahblah");
		}

		SQLCloseCursor(statementHandle);
		SQLFreeHandle(SQL_HANDLE_STMT, statementHandle);
	}

	SQLCloseCursor(STATEMENT_HANDLE);
	SQLFreeHandle(SQL_HANDLE_STMT, STATEMENT_HANDLE);
	SQLDisconnect(DATABASE_HANDLE);
	SQLFreeHandle(SQL_HANDLE_DBC, DATABASE_HANDLE);
	SQLFreeHandle(SQL_HANDLE_ENV, ENVIRONMENT_HANDLE);

	//SQLGetDiagRec(SQL_HANDLE_STMT, STATEMENT_HANDLE, 1, (SQLWCHAR*)state, (SQLINTEGER*)&nativeErrorCode, (SQLWCHAR*)message, 1024, &messageLength);

	//SQLNumResultCols(STATEMENT_HANDLE, (SQLSMALLINT*)&columnCount);

	//SQLDescribeCol(STATEMENT_HANDLE, columnIndex, buffer, sizeof(buffer), NULL, NULL, NULL, NULL, NULL);

	return 0;
}