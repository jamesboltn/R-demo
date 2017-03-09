#pragma once
#include"FutureOrderManager.h"
#include"ThostFtdcUserApiStruct.h"
#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4820)
#pragma warning(disable:4265)
#include"ThostFtdcTraderApi.h"
#pragma warning(pop)
#include"FutureOrderShooter.h"
#include"Utilities.h"

static FutureOrderManager* local_future_order_manager;

typedef struct 
{
	OrderBroker* order_broker;
	InvestAccountSync* invest_account;
	OrderCredential* order_credential;
}OrderOperationCredential;

static DWORD WINAPI CancelOrderThread(LPVOID lpParam)
{
	OrderOperationCredential* order_operation_credential = (OrderOperationCredential*)lpParam;
	::Sleep(5000);
	auto it = order_operation_credential->invest_account->_order_list.find(*(order_operation_credential->order_credential));
	if (it != order_operation_credential->invest_account->_order_list.end())
	{
		if (it->second.trade_volume < it->second.volume && !String::Contains(it->second.status, "³·"))
		{
			local_future_order_manager->order->Cancel(order_operation_credential->order_broker, order_operation_credential->invest_account, order_operation_credential->order_credential);
		}
	}

	if (order_operation_credential != NULL)
	{
		delete order_operation_credential;
		order_operation_credential = NULL;
	}
	
	return 0;
}

FutureOrderManager::FutureOrderManager(Order* _order)
	:OrderManager(_order)
{
	local_future_order_manager = this;
}

void FutureOrderManager::RegisterCloseOrder()
{

}

void FutureOrderManager::RegisterOpenOrder(OrderBroker* _order_broker, InvestAccountSync* invest_account, OrderCredential* _oc)
{
	//fire a thread to cancel order after 5 seconds if not fully traded 
	OrderOperationCredential* order_operation_credential = new OrderOperationCredential;
	OrderCredential* oc = new OrderCredential;
	memcpy(oc, _oc, sizeof(OrderCredential));

	order_operation_credential->order_broker = _order_broker;
	order_operation_credential->invest_account = invest_account;
	order_operation_credential->order_credential = oc;

	::CloseHandle(::CreateThread(NULL, 0, CancelOrderThread, order_operation_credential, 0, NULL));
}

bool FutureOrderManager::IsOrderExpire(const std::string &insert_time)
{
	std::string current_date = DateTime::GetCurrentDateString();
	String::RemoveAllChars(&current_date, '-');
	std::string current_time = DateTime::GetCurrentTimeString();
	
	if (current_time > trade_second_start_time)
	{
		if (insert_time < current_date + " " + trade_first_start_time)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		std::string yester_date = DateTime::GetYesterDateString();
		String::RemoveAllChars(&yester_date, '-');
		if (insert_time > yester_date + " " + trade_first_start_time)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}


