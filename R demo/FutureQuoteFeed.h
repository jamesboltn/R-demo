#pragma once
#include"preinclude.h"
#include"Quote.h"
#include"ClientStruct.h"
#include"TradeStruct.h"

//struct CompareByValue
//{
//bool operator()(const ContractOpenInterestPair &a, const ContractOpenInterestPair &b)
//{
//return a.second > b.second;
//}
//};
//typedef std::set<ContractOpenInterestPair, CompareByValue> ContractOpenInterestPairs;

class FutureQuoteFeed :
	public Quote
{
public:
	HANDLE* event;
	QuoteBrokerList* quote_broker_list;
	GeneralMessageHandler handler;
	FutureQuoteFeed(QuoteBrokerList* _quote_broker_list, HANDLE* _event, GeneralMessageHandler _handler);
};
