#pragma once
#include"preinclude.h"
#include"TradeDataType.h"
#include"ThostFtdcUserApiStruct.h"
//定义行情刷新结构
typedef struct
{
	//TODO:modify here, use InstrumentID and LastPrice only
	CThostFtdcDepthMarketDataField depth_market_data;
	Direction direction;
}RefreshQuote;
