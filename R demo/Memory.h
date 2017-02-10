#pragma once
#include"FutureAccountManager.h"

class FutureQuoteFeedForTrade;
class FutureOrderShooter;
class Memory
{
public:
	FutureQuoteFeedForTrade* quote_feed_trade;
	FutureOrderShooter* future_order_shooter;
	BrokerInfoList broker_info_list;
	Memory();
	virtual ~Memory();
};
