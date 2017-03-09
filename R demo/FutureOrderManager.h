#pragma once
#include"OrderManager.h"

class Order;
class FutureOrderManager :
	public OrderManager
{
public:
	FutureOrderManager(Order* _order);
	void RegisterOpenOrder(OrderBroker* order_broker, InvestAccountSync* invest_account, OrderCredential* oc);//期货里可以是过多少秒未成交就撤单，股票里可以使
	void RegisterCloseOrder();
	static bool IsOrderExpire(const std::string &insert_time);
};
