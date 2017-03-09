#pragma once
#include"FutureQuoteFeedForTrade.h"
#include"FutureQuoteProcessorForTrade.h"

FutureQuoteFeedForTrade::FutureQuoteFeedForTrade(QuoteBrokerList* _quote_broker_list, DWORD* _thread_id, HANDLE* _event, GeneralMessageHandler _handler):
	FutureQuoteFeed(_quote_broker_list, _event, _handler), thread_id(_thread_id)
{
}

FutureQuoteFeedForTrade::~FutureQuoteFeedForTrade()
{
	for (size_t i = 0; i < mdapi_spi_set.size(); ++i)
	{
		if (mdapi_spi_set[i].first != NULL)
		{
			mdapi_spi_set[i].first->RegisterSpi(NULL);
			mdapi_spi_set[i].first->Release();
		}

		if (mdapi_spi_set[i].second != NULL)
		{
			delete mdapi_spi_set[i].second;
			mdapi_spi_set[i].second = NULL;
		}
	}
}

void FutureQuoteFeedForTrade::ConnectQuoteServer()
{
	std::string folder_path;
	std::string broker_suffix;
	std::string folder_path_a;
	std::string msg;
	CThostFtdcMdApi* mdapi_instance;
	FutureQuoteProcessorForTrade* mdspi_instance;

	unsigned int index = 0;
	for (auto it = quote_broker_list->begin(); it != quote_broker_list->end(); ++it)
	{
		if (it->quote_front.first.empty())
		{
			msg = "you haven't specified any CTP quote server of " + it->broker_name;
			(*handler)(msg.c_str());
			continue;
		}
#ifdef _DEBUG
		if (it->quote_front.second == UNCONNECTABLE/* || it->account_type == SIMULATE*/)
#else
		if (it->quote_front.second == UNCONNECTABLE || it->account_type == SIMULATE)
#endif // DEBUG
		{
			continue;
		}

		if (index >= 2)
		{
			break;
		}

#ifdef _DEBUG
		if (it->quote_front.second == CONNECTABLE/* && it->account_type == REAL*/)
#else
		if (it->quote_front.second == CONNECTABLE && it->account_type == REAL)
#endif // DEBUG
		{
			folder_path = std::string("CTPQuoteConForTrade") + "\\";
			broker_suffix = it->broker_name;
			folder_path += broker_suffix;
			::CreateDirectoryA(folder_path.c_str(), NULL);

			folder_path_a = ".//CTPQuoteConForTrade//" + it->broker_name + "//";
			mdapi_instance = CThostFtdcMdApi::CreateFtdcMdApi(folder_path_a.c_str());

			//if two points doesn't work,you have to omit one
			if (it->invest_account_list.empty())
			{
				static InvestAccountSync invest_account;
				invest_account.investor_id = "106436";
				invest_account.user_id = "106436";
				invest_account.investor_password = "106436";

				mdspi_instance = new FutureQuoteProcessorForTrade(this, &(*it), &invest_account, index++);
			}
			else
			{
				mdspi_instance = new FutureQuoteProcessorForTrade(this, &(*it), &(it->invest_account_list[0]), index++);
			}

			mdapi_instance->RegisterSpi(mdspi_instance);

			for (auto it1 = it->quote_front.first.begin(); it1 != it->quote_front.first.end(); ++it1)
			{
				mdapi_instance->RegisterFront(&it1->first[0]);
			}
			mdapi_instance->Init();
			mdapi_spi_set.push_back(std::make_pair(mdapi_instance, mdspi_instance));
		}
	}
}

void FutureQuoteFeedForTrade::SubscribeQuote(ContractList* contract_list)
{
	if (mdapi_spi_set.empty())
	{
		(*handler)("you should connect to some quote server first before you subscribe any future contract");
		return;
	}

	size_t contract_count = contract_list->size();
	if (contract_count > 0)
	{
		size_t loop_count = 0;
		char** contract_array = new char*[contract_count];
		auto it = contract_list->begin();
		while (loop_count < contract_count)
		{
			contract_array[loop_count] = const_cast<char *>(it->c_str());
			++it;
			++loop_count;
		}

		switch (GetMdApiInstance(0)->SubscribeMarketData(contract_array, (int)contract_count))
		{
		case 0:
			break;
		case -1:
		{
			(*handler)("failed to subscribe future quote. network connection failed");
		}
		break;
		case -2:
		{
			(*handler)("failed to subscribe future quote. unprocessed requests exceed limit");
		}
		break;
		case -3:
		{
			(*handler)("failed to subscribe future quote. requests sent per second exceed limit");
		}
		break;
		default:
			break;
		}

		delete[]contract_array;
	}
	else
	{
		(*handler)("failed to subscribe CTP future quote. you haven't specified any contract");
	}
}

void FutureQuoteFeedForTrade::SubscribeQuote(const char* _contract_id)
{
	if (mdapi_spi_set.empty())
	{
		(*handler)("you should connect to some quote server first before you subscribe any future contract");
		return;
	}

	char* array_of_contract[1];
	array_of_contract[0] = const_cast<char*>(_contract_id);

	switch (GetMdApiInstance(0)->SubscribeMarketData(array_of_contract, 1))
	{
	case 0:
		break;
	case -1:
	{
		(*handler)("failed to subscribe future quote. network connection failed");
	}
	break;
	case -2:
	{
		(*handler)("failed to subscribe future quote. unprocessed requests exceed limit");
	}
	break;
	case -3:
	{
		(*handler)("failed to subscribe future quote. requests sent per second exceed limit");
	}
	break;
	default:
		break;
	}
}

void FutureQuoteFeedForTrade::UnsubscribeQuote(ContractList* contract_list)
{
	size_t contract_count = contract_list->size();
	if (contract_count > 0)
	{
		size_t loop_count = 0;
		char** array_of_contract = new char*[contract_count];
		auto it(contract_list->begin());
		while (loop_count < contract_count)
		{
			array_of_contract[loop_count] = const_cast<char *>(it->c_str());;
			++it;
			++loop_count;
		}

		switch (GetMdApiInstance(0)->UnSubscribeMarketData(array_of_contract, (int)contract_count))
		{
		case 0:
			break;
		case -1:
			(*handler)("failed to unsubscribe future quote. network connection failed");
			break;
		case -2:
			(*handler)("failed to unsubscribe future quote. unprocessed requests exceed limit");
			break;
		case -3:
			(*handler)("failed to unsubscribe future quote. requests sent per second exceed limit");
			break;
		default:
			break;
		}
		delete[]array_of_contract;
	}
	else
	{
		(*handler)("failed to subscribe CTP future quote. you haven't specified any contract");
	}
}

void FutureQuoteFeedForTrade::UnsubscribeQuote(const char* contract_id)
{
	if (mdapi_spi_set.empty())
	{
		(*handler)("you should connect to some quote server first before you unsubscribe any future contract");
		return;
	}

	char* array_of_contract[1];
	array_of_contract[0] = const_cast<char*>(contract_id);
	switch (mdapi_spi_set[0].first->UnSubscribeMarketData(array_of_contract, 1))
	{
	case 0:
		break;
	case -1:
		(*handler)("failed to unsubscribe future quote. network connection failed");
		break;
	case -2:
		(*handler)("failed to unsubscribe future quote. unprocessed requests exceed limit");
		break;
	case -3:
		(*handler)("failed to unsubscribe future quote. requests sent per second exceed limit");
		break;
	default:
		break;
	}
}

CThostFtdcMdApi* FutureQuoteFeedForTrade::GetMdApiInstance(size_t index)
{
	if (index < mdapi_spi_set.size())
	{
		return mdapi_spi_set[index].first;
	}
	else
	{
		return NULL;
	}
}