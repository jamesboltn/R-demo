#pragma once
#include"preinclude.h"

struct OrderID
{
	std::string  contract;
	std::string exchange;
	bool operator==(const OrderID &id) { return contract == id.contract && exchange == id.exchange; }
};

bool operator==(const OrderID &id1, const OrderID &id2);

