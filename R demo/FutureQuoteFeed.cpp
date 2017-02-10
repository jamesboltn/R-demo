#pragma once
#include"FutureQuoteFeed.h"
#include"Utilities.h"

FutureQuoteFeed::FutureQuoteFeed(BrokerInfoList* _broker_info_list, HANDLE* _event, GeneralMessageHandler _handler)
{
	broker_info_list = _broker_info_list;
	handler = _handler;
	event = _event;
}



