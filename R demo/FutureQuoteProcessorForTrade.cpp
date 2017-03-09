#pragma once
#include"FutureQuoteProcessorForTrade.h"
#include"FutureQuoteFeedForTrade.h"

FutureQuoteProcessorForTrade::FutureQuoteProcessorForTrade(FutureQuoteFeedForTrade* _quote_feed, QuoteBroker* _quote_broker, InvestAccountSync* _invest_account, unsigned int _index):
	FutureQuoteProcessor(_quote_broker, _invest_account, _index), quote_feed(_quote_feed)
{
}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void FutureQuoteProcessorForTrade::OnFrontConnected()
{
	CThostFtdcReqUserLoginField field;
	::ZeroMemory(&field, sizeof(field));
	strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), quote_broker->broker_id.c_str());
	strcpy_s(field.UserID, ARRAYCOUNT(field.UserID), invest_account->user_id.c_str());
	strcpy_s(field.Password, ARRAYCOUNT(field.Password), invest_account->investor_id.c_str());
	strcpy_s(field.UserProductInfo, ARRAYCOUNT(field.UserProductInfo), "EasyTrade2");
	switch (quote_feed->GetMdApiInstance((size_t)index)->ReqUserLogin(&field, ++invest_account->ctp_quote_request_id))
	{
	case 0:
		break;
	case -1:
	{
		msg = "failed to log in " + quote_broker->broker_name + " ctp quote server. network connection failed";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	case -2:
	{
		msg = "failed to log in " + quote_broker->broker_name + " ctp quote server. unprocessed requests exceed limit";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	case -3:
	{
		msg = "failed to log in " + quote_broker->broker_name + " ctp quote server. requests sent per second exceed limit";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	default:
		break;
	}
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
///this callback will be invoked only when there is a created connection 
void FutureQuoteProcessorForTrade::OnFrontDisconnected(int nReason)
{
	switch (nReason)
	{
	case 0x1001:
	{
		msg = "failed to keep connection to " + quote_broker->broker_name + " ctp quote server. failed to read on network";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	case 0x1002:
	{
		msg = "failed to keep connection to " + quote_broker->broker_name + " ctp quote server. failed to write on network";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	case 0x2001:
	{
		msg = "failed to keep connection to " + quote_broker->broker_name + " ctp quote server. receive heartbeat timeout";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	case 0x2002:
	{
		msg = "failed to keep connection to " + quote_broker->broker_name + " ctp quote server. send heartbeat failed";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	case 0x2003:
	{
		msg = "failed to keep connection to " + quote_broker->broker_name + " ctp quote server. received errorneous packets";
		(*quote_feed->handler)(msg.c_str());
	}
	break;
	default:
		break;
	}
}

///登录请求响应
void FutureQuoteProcessorForTrade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to log in " + quote_broker->broker_name + " CTP future quote server";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*quote_feed->handler)(msg.c_str());
	}
	else
	{
		//TODO:no need to write succeed message to disk log file
		msg = "succeeded in logging in " + quote_broker->broker_name + " ctp quote server";
		(*quote_feed->handler)(msg.c_str());
		if (index == 0)
		{
			::SetEvent(*quote_feed->event);
		}
	}
}

///错误应答
void FutureQuoteProcessorForTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		msg = "you've passed errorneous arguments to CTP functions";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*quote_feed->handler)(msg.c_str());
	}
}

///订阅行情应答
void FutureQuoteProcessorForTrade::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		FormatRspInfo(pRspInfo, pSpecificInstrument, SUBSCRIBE, &msg);
		(*quote_feed->handler)(msg.c_str());
	}
	else
	{

		//quote_feed->wnd_main->edit_app_log.AppendLineA(pSpecificInstrument->InstrumentID);
		std::string contract_id(pSpecificInstrument->InstrumentID);
		if (quote_feed->subscribe_contract_list.find(contract_id) == quote_feed->subscribe_contract_list.end())
		{
			quote_feed->subscribe_contract_list.insert(std::make_pair(contract_id, 1));
		}
	}
}

///取消订阅行情应答
void FutureQuoteProcessorForTrade::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{
		FormatRspInfo(pRspInfo, pSpecificInstrument, UNSUBSCRIBE, &msg);
		(*quote_feed->handler)(msg.c_str());
	}
	else
	{
		std::string contract_id(pSpecificInstrument->InstrumentID);
		if (quote_feed->subscribe_contract_list.find(contract_id) != quote_feed->subscribe_contract_list.end())
		{
			quote_feed->subscribe_contract_list.erase(contract_id);
		}
	}
}

///深度行情通知
void FutureQuoteProcessorForTrade::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//check if is valid quote
	if (pDepthMarketData->LastPrice < pDepthMarketData->UpperLimitPrice)
	{
		if (quote_feed->tick_cache_store_index >= TICK_CACHE_SIZE)
		{
			quote_feed->tick_cache_store_index = 0;
		}
		memcpy(&quote_feed->tick_cache[quote_feed->tick_cache_store_index].depth_market_data, pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		::PostThreadMessageA(*quote_feed->thread_id, MSG_REFRESH_QUOTE, NULL, (LPARAM)&quote_feed->tick_cache[quote_feed->tick_cache_store_index++]);
	}
}
