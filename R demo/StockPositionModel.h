#pragma once
#include"ModelDataType.h"
#include"TradeDataType.h"
#include"StockOrderShooter.h"
class SQLite;
class StockPositionModel
{
public:
	static void CreateTable(SQLite* db, const char* table_name, DatabaseErrorMessageHandler handler = NULL);
	static void Retrieve(SQLite* db, const char* table_name, std::multiset<PositionDetailSync, CompareByValue>* position_list, AccountSpecificMsgHandler handler = NULL, unsigned int index = 0);
	static void Update(SQLite* db, const char* table_name, std::multiset<PositionDetailSync, CompareByValue>* position_list, AccountSpecificMsgHandler handler = NULL, unsigned int index = 0);
};