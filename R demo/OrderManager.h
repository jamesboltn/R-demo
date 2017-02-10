#pragma once
#include"TradeStruct.h"

class Order;
class OrderManager
{
public:
	Order* order;
	OrderManager(Order* _order);
protected:
	virtual void RegisterOpenOrder(BrokerInfo* broker_info, InvestAccountSync* invest_account, OrderCredential* oc) = 0;
	virtual void RegisterCloseOrder() = 0;
};

