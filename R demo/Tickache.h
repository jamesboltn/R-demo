#pragma once
#include"preinclude.h"
#include"TradeStruct.h"
#include"ThostFtdcUserApiStruct.h"

typedef std::string Contract;

class FutureQuoteFeedForCache;
class Tickache
{
private:
	ContractDictionary* contract_dic;
	std::string tick_store_dir;
	void TickFile(std::vector<CThostFtdcDepthMarketDataField> &market_data_vec);
public:
	FutureQuoteFeedForCache* quote_feed;
	int start_count;
	std::unordered_map<Contract, std::pair<FILE*, int>> contract_file_handle_map;
	
	static Tickache* local_tickache;
	Tickache(FutureQuoteFeedForCache* _quote_feed, ContractDictionary* _contract_dic);
	void TickFile();
	
};