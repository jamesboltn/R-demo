#pragma once
#include"TradeStruct.h"

class Order
{
public:
	virtual void Cancel(BrokerInfo* broker_info, InvestAccountSync* invest_account, OrderCredential* oc) = 0;
protected:
	virtual void Shoot(char* contract, double price, Direction direction, OpenFlag open_flag, int volume, BrokerInfo* broker_info, InvestAccountSync* invest_account) = 0;
	virtual void ConnectOrderServer() = 0;
};
