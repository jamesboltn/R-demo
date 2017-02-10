#pragma once
#include"Order.h"
#include"FutureOrderManager.h"

class FutureOrderProcessor;
class FutureAccountManager;
class CThostFtdcTraderApi;
class FutureOrderShooter :
	public Order
{
private:
public:
	AccountSpecificMessageHandler handler;
	FutureOrderManager future_order_manager;
	char ctp_trading_day[9];
	FutureOrderShooter(AccountSpecificMessageHandler _handler);
	virtual ~FutureOrderShooter();

	void ConnectOrderServer();
	void Shoot(char* contract, double price, Direction direction, OpenFlag open_flag, int volume, BrokerInfo* broker_info, InvestAccountSync* invest_account);
	void Cancel(BrokerInfo* broker_info, InvestAccountSync* invest_account, OrderCredential* oc);
};

