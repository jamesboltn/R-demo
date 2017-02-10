#pragma once
#include"GlobalObject.h"
#include"FutureOrderShooter.h"
#include"FutureAccountManager.h"
#include"Utilities.h"
#include"ThostFtdcUserApiStruct.h"
#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4820)
#pragma warning(disable:4265)
#include"ThostFtdcTraderApi.h"
#pragma warning(pop)
#include"FutureOrderProcessor.h"
#include"FutureQuoteFeed.h"

FutureOrderShooter::FutureOrderShooter(AccountSpecificMessageHandler _handler):
#pragma warning(push)
#pragma warning(disable:4355)
	future_order_manager(this)
#pragma warning(pop)
{
	handler = _handler;
	ctp_trading_day[0] = '\0';
}

FutureOrderShooter::~FutureOrderShooter()
{
}

void FutureOrderShooter::ConnectOrderServer()
{
	std::string folder_path;
	std::string broker_suffix;
	char investor_id[18];
	std::string folder_path_a;
	std::string instance_log_path;
	CThostFtdcTraderApi* orderapi_instance;
	FutureOrderProcessor* orderspi_instance;
	std::string msg;
	size_t index = 0;
	
	for (auto it = Global::memory.broker_info_list.begin(); it != Global::memory.broker_info_list.end(); ++it)
	{
		folder_path = "Log" + std::string("\\");
		broker_suffix = it->broker_name.c_str();
		folder_path += broker_suffix;
		::CreateDirectoryA(folder_path.c_str(), NULL);
		folder_path += "\\";
#pragma region check for future accounts with no order server supplied
		if (it->order_front.first.empty())
		{
			for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
			{
				strcpy_s(investor_id, ARRAYCOUNT(investor_id), it1->investor_id.c_str());
				folder_path += investor_id;
				CreateDirectoryA(folder_path.c_str(), NULL);

				instance_log_path = "./Log/" + it->broker_name + "/" + it1->investor_id + "/";
				it1->orderapi_spi_pair = std::make_pair(nullptr, nullptr);

				msg = "you haven't specified any CTP future order server of " + Global::memory.broker_info_list[index].broker_name;

				//show error message on window
				(*handler)(msg.c_str(), index++, instance_log_path.c_str());
			}

			continue;
		}
#pragma endregion
#pragma region check for future accounts with no order server connectable
		else if (it->order_front.second == UNCONNECTABLE)
		{
			for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
			{
				strcpy_s(investor_id, ARRAYCOUNT(investor_id), it1->investor_id.c_str());
				folder_path += investor_id;
				CreateDirectoryA(folder_path.c_str(), NULL);

				instance_log_path = "./Log/" + it->broker_name + "/" + it1->investor_id + "/";

				it1->orderapi_spi_pair = std::make_pair(nullptr, nullptr);
			}

			continue;
		}
#pragma endregion

		folder_path = "CTPOrderCon" + std::string("\\") + broker_suffix;
		::CreateDirectoryA(folder_path.c_str(), NULL);
		folder_path += "\\";

		folder_path_a = ".//CTPOrderCon//" + it->broker_name + "//";

#pragma region connect order server
		for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
		{
#pragma region create ctp order con investor id path, should be placed first
			strcpy_s(investor_id, ARRAYCOUNT(investor_id), it1->investor_id.c_str());
			folder_path += investor_id;
			::CreateDirectoryA(folder_path.c_str(), NULL);
#pragma endregion
#pragma region create investor id log path, set log path of each edit
			folder_path = "Log" + std::string("\\");
			broker_suffix = it->broker_name;
			folder_path += broker_suffix + "\\" + investor_id;
			::CreateDirectoryA(folder_path.c_str(), NULL);

			instance_log_path = "./Log/" + it->broker_name + "/" + it1->investor_id + "/";

			//set log path of edit
			(*handler)(NULL, index, instance_log_path.c_str());
#pragma endregion
#pragma region instantiate api_spi pairs
			folder_path_a += it1->investor_id;
			folder_path_a += "//";
			orderapi_instance = CThostFtdcTraderApi::CreateFtdcTraderApi(folder_path_a.c_str());
			//if two points doesn't work,you have to omit one
			orderspi_instance = new FutureOrderProcessor(this, &(*it), &(*it1), index++);

			orderapi_instance->RegisterSpi(orderspi_instance);
			for (auto it2 = it->order_front.first.begin(); it2 != it->order_front.first.end(); ++it2)
			{
				orderapi_instance->RegisterFront(&it2->first[0]);
			}
			orderapi_instance->SubscribePublicTopic(THOST_TERT_QUICK);
			orderapi_instance->SubscribePrivateTopic(THOST_TERT_RESTART);
			orderapi_instance->Init();
			it1->orderapi_spi_pair = std::make_pair(orderapi_instance, orderspi_instance);
#pragma endregion
		}
#pragma endregion 
	}
}

void FutureOrderShooter::Shoot(char* contract, double price, Direction direction, OpenFlag open_flag, int volume, BrokerInfo* broker_info, InvestAccountSync* invest_account)
{
	CThostFtdcTraderApi* orderapi_instance = invest_account->orderapi_spi_pair.first;
	if (orderapi_instance)
	{
		CThostFtdcInputOrderField field;
		ZeroMemory(&field, sizeof(field));

		field.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//1投机
		field.ContingentCondition = THOST_FTDC_CC_Immediately;//立即触发
		field.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;//不强平
		field.IsAutoSuspend = 0;//自动挂起标志
		field.VolumeCondition = THOST_FTDC_VC_AV;//全部成交或部分成交
		field.MinVolume = 1;//最小成交量
		field.OrderPriceType = THOST_FTDC_OPT_LimitPrice;		//限价2
		field.TimeCondition = THOST_FTDC_TC_GFD;//挂单当日有效
		field.LimitPrice = price;
		strcpy_s(field.GTDDate, ARRAYCOUNT(field.GTDDate), "");//GTD日期

		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		strcpy_s(field.InstrumentID, ARRAYCOUNT(field.InstrumentID), contract);
		strcpy_s(field.UserID, ARRAYCOUNT(field.UserID), invest_account->user_id.c_str());

		//注意：平仓时要在这里修改上期所 平今 平昨
		bool SHFE_historical = false;//设置上期所合约历史仓标识
		if (direction == BUY)
		{
			field.Direction = THOST_FTDC_D_Buy;
		}
		else if (direction == SELL)
		{
			field.Direction = THOST_FTDC_D_Sell;
		}

		PositionListSync::iterator it;
		if (open_flag == OPEN)
		{
			field.CombOffsetFlag[0] = THOST_FTDC_OF_Open;//开仓
		}
		else if (open_flag == CLOSE)
		{
			PositionCredential pc; pc.contract = contract; pc.direction = (direction == SELL) ? BUY : SELL;
			it = FindPositionFromNormalNew(&pc, &invest_account->position_list);
			if (it != invest_account->position_list.end())
			{
				if (it->second.exchange != "SHFE")
				{
					field.CombOffsetFlag[0] = THOST_FTDC_OF_Close;	//平仓
				}
				else
				{
					SHFE_historical = true;
				}
			}
			else
			{
				field.CombOffsetFlag[0] = THOST_FTDC_OF_Close;	//平仓
			}
		}

		if (!SHFE_historical)
		{
			field.VolumeTotalOriginal = volume;//数量
			sprintf_s(field.OrderRef, ARRAYCOUNT(field.OrderRef), "%012d", ++invest_account->order_ref);//OrderRef
			orderapi_instance->ReqOrderInsert(&field, ++invest_account->ctp_order_request_id);
		}
		else
		{
			if (it->second.historical_volume > 0)
			{
				field.VolumeTotalOriginal = it->second.historical_volume;//平昨数量
				field.CombOffsetFlag[0] = THOST_FTDC_OF_Close;	//平昨
				sprintf_s(field.OrderRef, ARRAYCOUNT(field.OrderRef), "%012d", ++invest_account->order_ref);//OrderRef
				orderapi_instance->ReqOrderInsert(&field, ++invest_account->ctp_order_request_id);
			}
			
			int today_volume = volume - it->second.historical_volume;
			if (today_volume > 0)
			{
				field.VolumeTotalOriginal = today_volume;//平今数量
				field.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;	//平今
				sprintf_s(field.OrderRef, ARRAYCOUNT(field.OrderRef), "%012d", ++invest_account->order_ref);//OrderRef
				orderapi_instance->ReqOrderInsert(&field, ++invest_account->ctp_order_request_id);
			}
		}
	}
}

void FutureOrderShooter::Cancel(BrokerInfo* broker_info, InvestAccountSync* invest_account, OrderCredential* oc)
{
	CThostFtdcInputOrderActionField field;
	ZeroMemory(&field, sizeof(field));
	field.ActionFlag = THOST_FTDC_AF_Delete; 	//THOST_FTDC_AF_Modify
	strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
	strcpy_s(field.InstrumentID, ARRAYCOUNT(field.InstrumentID), oc->contract.c_str());
	strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());

	//FrontID + SessionID + OrderRef
	field.FrontID = oc->front_id;
	field.SessionID = oc->session_id;
	strcpy_s(field.OrderRef, ARRAYCOUNT(field.OrderRef), oc->order_ref.c_str());

	(invest_account->orderapi_spi_pair.first)->ReqOrderAction(&field, ++invest_account->ctp_order_request_id);
}
