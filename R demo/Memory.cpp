#pragma once
#include"Memory.h"
#include"FutureOrderProcessor.h"
#include"FutureQuoteFeed.h"
#include"FutureOrderShooter.h"
#include"FutureOrderManager.h"
#include"FutureQuoteFeedForTrade.h"

Memory::Memory()
{
}

Memory::~Memory()
{
	if (quote_feed_trade != NULL)
	{
		delete quote_feed_trade;
		quote_feed_trade = NULL;
	}

	if (future_order_shooter != NULL)
	{
		delete future_order_shooter;
		future_order_shooter = NULL;
	}

	for (auto it = order_broker_list.begin(); it != order_broker_list.end(); ++it)
	{
		for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
		{
			if (it1->second.orderapi_spi_pair.first)
			{
				(it1->second.orderapi_spi_pair.first)->RegisterSpi(NULL);
				(it1->second.orderapi_spi_pair.first)->Release();
			}
			if (it1->second.orderapi_spi_pair.second)
			{
				delete it1->second.orderapi_spi_pair.second;
				it1->second.orderapi_spi_pair.second = NULL;
			}
		}
	}
}