#pragma once
#include"TradeStruct.h"
#include"ModelDataType.h"

class SQLite;
class FutureContractDictionaryModel
{
public:
	static void CreateTable(SQLite* db, DatabaseErrorMessageHandler handler);
	static void Retrieve(SQLite* db, ContractDictionary* contract_dic, DatabaseErrorMessageHandler handler);
	static void Delete(SQLite* db, ContractDictionary* contract_dic, DatabaseErrorMessageHandler handler);
	static void Insert(SQLite* db, ContractDictionary* contract_dic, DatabaseErrorMessageHandler handler);
};

