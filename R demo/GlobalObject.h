#pragma once
#include"TradeStruct.h"
#include"Memory.h"
#include"SQLite.h"
//note that only those objects accessed by multiple source files can be declared in the global namespace, otherwise declare them in a scope as lower as possible
namespace Global
{
	
	extern SQLite db;
	extern HANDLE event;
	extern Memory memory;

	extern void AddContractForSubscribe(const char* contract);
	extern void RemoveContractForUnsubscribe(const char* contract);
	extern void LoadModules();

	namespace TradeModule
	{
		extern OrderBrokerList order_broker_list;
		extern OrderBrokerLst order_broker_lst;
		extern QuoteBrokerList quote_broker_list;
		extern ContractDictionary contract_dic;
		extern FutureAccountManager future_account_manager;
	}
}

