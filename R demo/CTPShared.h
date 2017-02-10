#pragma once
#include"TradeStruct.h"
struct CThostFtdcRspInfoField;
class CTPShared
{
protected:
	std::string msg;
	///determine if pRspInfo reports error
	static bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
	static void FormatRspInfo(CThostFtdcRspInfoField* pRspInfo, std::string* entry_msg);
public:
	unsigned int index;
	InvestAccountSync* invest_account;
	BrokerInfo* broker_info;
};
