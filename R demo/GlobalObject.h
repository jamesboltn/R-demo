#pragma once
#include"TradeStruct.h"
#include"Memory.h"
#include"SQLite.h"
//take care that only those objects accessed by multiple source files can be declared in the global namespace, otherwise declare them in a lower scope as much as you can
namespace Global
{
	extern InvestAccontList invest_accont_list;
	extern ContractDictionary contract_dic;
	extern FutureAccountManager future_account_manager;
	extern SQLite db;
	extern HANDLE event;
	extern Memory memory;
	extern void AddContractForSubscribe(const char* contract);
	extern void RemoveContractForUnsubscribe(const char* contract);
}
