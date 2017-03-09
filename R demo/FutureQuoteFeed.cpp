#pragma once
#include"FutureQuoteFeed.h"
#include"Utilities.h"

FutureQuoteFeed::FutureQuoteFeed(QuoteBrokerList* _quote_broker_list, HANDLE* _event, GeneralMessageHandler _handler):
	quote_broker_list(_quote_broker_list), handler(_handler), event(_event)
{
}



