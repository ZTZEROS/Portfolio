#include "stdafx.h"
#include "MySQL_Client.h"

int ExecuteMySQL_Example(int argumentCount, char* argumentVector[])
{
	MYSQL* mysqlAddress;
	MYSQL_RES* mysqlResult;
	MYSQL_ROW mysqlRow;

	mysqlAddress = new MYSQL;

	mysql_init(mysqlAddress);

	mysql_real_connect(mysqlAddress, "127.0.0.1", "logAdministrator", "2705", "log", 3306, (char*)NULL, 0);

	if (!mysqlAddress)
	{
		printf("Mysql connection error : %s", mysql_error(mysqlAddress));

		return 1;
	}

	mysql_set_character_set(mysqlAddress, "utf8");

	char query[] = "SELECT now()"; //char* query = "SELECT now()";
	string queryText = "SELECT";

	queryText.append(" now()");	//queryText + to_string(x) + "now()";

	if (mysql_query(mysqlAddress, queryText.c_str()))
	{
		printf("Mysql query error : %s", mysql_error(mysqlAddress));

		return 1;
	}

	mysqlResult = mysql_store_result(mysqlAddress); //store all result row
	//mysqlResult = mysql_use_result(mysqlAddress); //store only one result row

	mysqlRow = mysql_fetch_row(mysqlResult);

	while (mysqlRow)
	{
		printf(mysqlRow[0]);
		printf(CRALF);

		mysqlRow = mysql_fetch_row(mysqlResult);
	}

	mysql_free_result(mysqlResult);

	mysql_close(mysqlAddress);

	delete mysqlAddress;

	return 0;
}