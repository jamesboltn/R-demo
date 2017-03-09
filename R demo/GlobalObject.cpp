#pragma once
#include"GlobalObject.h"
#include"FutureQuoteFeedForTrade.h"

//definition for Global namespace
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

//definition for Global::TradeModule namespace
OrderBrokerList Global::TradeModule::order_broker_list;
OrderBrokerLst Global::TradeModule::order_broker_lst;
QuoteBrokerList Global::TradeModule::quote_broker_list;
ContractDictionary Global::TradeModule::contract_dic;
FutureAccountManager Global::TradeModule::future_account_manager;




