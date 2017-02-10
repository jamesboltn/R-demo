#pragma once
#include"GlobalObject.h"
#include"FutureQuoteFeedForTrade.h"
InvestAccontList Global::invest_accont_list;
ContractDictionary Global::contract_dic;
FutureAccountManager Global::future_account_manager;
SQLite Global::db;
HANDLE Global::event;
Memory Global::memory;

void Global::AddContractForSubscribe(const char* contract)
{
	if (memory.quote_feed_trade->subscribe_contract_list.find(contract) == memory.quote_feed_trade->subscribe_contract_list.end())
	{
		memory.quote_feed_trade->subscribe_contract_list.insert(std::make_pair(contract, 1));
		memory.quote_feed_trade->SubscribeQuote(contract);
	}
	else
	{
		++memory.quote_feed_trade->subscribe_contract_list[contract];
	}
}
void Global::RemoveContractForUnsubscribe(const char* contract)
{
	auto it = memory.quote_feed_trade->subscribe_contract_list.find(contract);
	if (it != memory.quote_feed_trade->subscribe_contract_list.end())
	{
		if (--it->second == 0)
		{
			memory.quote_feed_trade->subscribe_contract_list.erase(it);
			memory.quote_feed_trade->UnsubscribeQuote(contract);
		}
	}
}
