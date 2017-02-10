#pragma once
#include"FutureOrderID.h"

bool operator==(const OrderID &id1, const OrderID &id2)
{
	return id1.contract == id2.contract &&
		id1.exchange == id2.exchange && id1.broker_order_seq == id2.broker_order_seq;
}