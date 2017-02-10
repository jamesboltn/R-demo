#pragma once
#include"FutureQuoteFeed.h"

class CThostFtdcMdApi;
class FutureQuoteProcessorForCache;
class FutureQuoteFeedForCache:
	public FutureQuoteFeed
{
private:
	std::vector<std::pair<CThostFtdcMdApi*, FutureQuoteProcessorForCache*>> mdapi_spi_set;
public:
	int tick_cache_index;
	std::vector<CThostFtdcDepthMarketDataField> market_data_vec1;
	std::vector<CThostFtdcDepthMarketDataField> market_data_vec2;
	FutureQuoteFeedForCache(BrokerInfoList* _broker_info_list, HANDLE* _event, GeneralMessageHandler _handler);
	virtual ~FutureQuoteFeedForCache();
	CThostFtdcMdApi* GetMdApiInstance(size_t index);
	void ConnectQuoteServer();
	void SubscribeQuote(ContractList* contract_list);
	void SubscribeQuote(const std::string* contract_id);
	void UnsubscribeQuote(ContractList* contract_list);
	void UnsubscribeQuote(const std::string* contract_id);
};
