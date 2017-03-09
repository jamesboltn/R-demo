#pragma once
#include"TradeStruct.h"
#include"ModelDataType.h"
class SQLite;
class FutureOrderModel
{
public:
	static void Retrieve(SQLite* db, const char* table_name, OrderListSync* order_list, AccountSpecificMsgHandler handler, const char* page_name);
	static void Update(SQLite* db, const char* table_name, OrderListSync* order_list, AccountSpecificMsgHandler handler, const char* page_name);
};

