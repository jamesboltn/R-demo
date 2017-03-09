#pragma once
#include"FutureAccountManager.h"

class FutureQuoteFeedForTrade;
class FutureOrderShooter;
class Memory
{
public:
	FutureQuoteFeedForTrade* quote_feed_trade = NULL;
	FutureOrderShooter* future_order_shooter = NULL;
	Memory();
	virtual ~Memory();
};
