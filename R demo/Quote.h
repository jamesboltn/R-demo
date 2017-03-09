#pragma once
#include"preinclude.h"
#include"TradeDataType.h"

typedef std::unordered_set<std::string> ContractList;
class Quote
{
protected:
	virtual void ConnectQuoteServer() = 0;
	virtual void SubscribeQuote(ContractList* contract_list) = 0;
	virtual void SubscribeQuote(const char* contract_id) = 0;
	virtual void UnsubscribeQuote(ContractList* contract_list) = 0;
	virtual void UnsubscribeQuote(const char* contract_id) = 0;
};