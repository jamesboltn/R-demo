#pragma once
#include"preinclude.h"

struct OrderID
{
	std::string  contract;
	std::string exchange;
	std::string order_sys_id;//期货专有，交易所订单编号
	std::string broker_order_seq;//期货专有，经纪公司报单编号
	bool operator==(const OrderID &id) { return contract == id.contract && exchange == id.exchange && broker_order_seq == id.broker_order_seq; }
};

bool operator==(const OrderID &id1, const OrderID &id2);
