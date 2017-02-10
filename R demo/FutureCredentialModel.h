#pragma once
#include"TradeStruct.h"
#include"ModelDataType.h"

class SQLite;
class FutureCredentialModel
{
public:
	static void CreateTable(SQLite* db, const char* table_name, DatabaseErrorMessageHandler handler);
	static void Insert(SQLite* db, const char* table_name, InvestAccountSync* invest_account, DatabaseErrorMessageHandler handler);
	static void Retrieve(SQLite* db, const char* table_name, InvestAccountSync* invest_account, AccountSpecificMsgHandler handler, unsigned int index);
};
