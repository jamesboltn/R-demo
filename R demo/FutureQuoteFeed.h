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
	BrokerInfoList* broker_info_list;
	GeneralMessageHandler handler;
	FutureQuoteFeed(BrokerInfoList* _broker_info_list, HANDLE* _event, GeneralMessageHandler _handler);
};
