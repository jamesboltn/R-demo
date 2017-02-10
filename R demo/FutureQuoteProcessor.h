#pragma once
#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4265)
#include "ThostFtdcMdApi.h"
#pragma warning(pop)


#include"CTPShared.h"

class FutureQuoteProcessor :
	public CThostFtdcMdSpi,
	public CTPShared
{
private:
public:
	FutureQuoteProcessor(BrokerInfo* _broker_info, InvestAccountSync* _invest_account, unsigned int _index);
	virtual ~FutureQuoteProcessor() {};
	/*///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);*/

	static void FormatRspInfo(CThostFtdcRspInfoField* pRspInfo, CThostFtdcSpecificInstrumentField *pSpecificInstrument, SubscribeFlag subscibe_flag, std::string* msg);
};
