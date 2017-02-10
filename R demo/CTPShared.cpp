#pragma once
#include"CTPShared.h"
#include"ThostFtdcUserApiStruct.h"

void CTPShared::FormatRspInfo(CThostFtdcRspInfoField* pRspInfo, std::string* entry_msg)
{
	*entry_msg += std::string(" error message ") + pRspInfo->ErrorMsg + ".";
}

bool CTPShared::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	bool IsError(((pRspInfo) && (pRspInfo->ErrorID != 0)));
	return IsError;
}

