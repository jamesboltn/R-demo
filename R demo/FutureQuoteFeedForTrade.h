#pragma once
#include"FutureQuoteFeed.h"

#define TICK_CACHE_SIZE 20
#define MSG_REFRESH_QUOTE WM_APP+1//自定义行情来临消息

typedef int SubscribeCount;
typedef std::map<std::string, SubscribeCount> SubscribeContractList;
class CThostFtdcMdApi;
class FutureQuoteProcessorForTrade;
class FutureQuoteFeedForTrade:
	public FutureQuoteFeed
{
private:
	std::vector<std::pair<CThostFtdcMdApi*, FutureQuoteProcessorForTrade*>> mdapi_spi_set;

public:
	DWORD* thread_id;
	int tick_cache_store_index = 0;
	RefreshQuote tick_cache[TICK_CACHE_SIZE];
	SubscribeContractList subscribe_contract_list;
	FutureQuoteFeedForTrade(QuoteBrokerList* _quote_broker_list, DWORD* _thread_id, HANDLE* _event, GeneralMessageHandler _handler);
	virtual ~FutureQuoteFeedForTrade();
	CThostFtdcMdApi* GetMdApiInstance(size_t index);
	void ConnectQuoteServer();
	void SubscribeQuote(ContractList* contract_list);
	void SubscribeQuote(const char* contract_id);
	void UnsubscribeQuote(ContractList* contract_list);
	void UnsubscribeQuote(const char* contract_id);
};
