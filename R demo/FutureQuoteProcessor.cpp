#pragma once
#include"preinclude.h"
#include"FutureQuoteProcessor.h"
#include"ClientDataType.h"

FutureQuoteProcessor::FutureQuoteProcessor(BrokerInfo* _broker_info, InvestAccountSync* _invest_account, unsigned int _index)
{
	broker_info = _broker_info;
	invest_account = _invest_account;
	index = _index;
}

void FutureQuoteProcessor::FormatRspInfo(CThostFtdcRspInfoField* pRspInfo, CThostFtdcSpecificInstrumentField *pSpecificInstrument, SubscribeFlag subscribe_flag, std::string* msg)
{
	if (subscribe_flag == SUBSCRIBE)
	{
		*msg = std::string("failed to subcribe ") + pSpecificInstrument->InstrumentID;
	}
	else if (subscribe_flag == UNSUBSCRIBE)
	{
		*msg = std::string("failed to unsubcribe ") + pSpecificInstrument->InstrumentID;
	}
	char error_code_str[10];
	sprintf_s(error_code_str, ARRAYCOUNT(error_code_str), "%d", pRspInfo->ErrorID);
	*msg += std::string(". error code ") + error_code_str + ". error message " + pRspInfo->ErrorMsg + ".";
}
