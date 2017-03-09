#pragma once
#include"Order.h"

bool operator==(const OrderID &id1, const OrderID &id2)
{
	return id1.contract == id2.contract &&
		id1.exchange == id2.exchange;
}
