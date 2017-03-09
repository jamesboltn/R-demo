#pragma once
#include"FutureOrderProcessor.h"
#include"FutureOrderShooter.h"
#include"FutureOrderManager.h"
#include"GlobalObject.h"

FutureOrderProcessor::FutureOrderProcessor(FutureOrderShooter* _order_shooter, OrderBroker* _order_broker, InvestAccountSync* _invest_account, unsigned int _index):
	CTPShared(_invest_account, _index),
	order_shooter(_order_shooter),
	order_broker(_order_broker)
	
{
}

FutureOrderProcessor::~FutureOrderProcessor()
{
}
																				
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void FutureOrderProcessor::OnFrontConnected()
{
	CThostFtdcReqUserLoginField field;
	ZeroMemory(&field, sizeof(field));
	strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
	strcpy_s(field.UserID, ARRAYCOUNT(field.UserID), invest_account->user_id.c_str());
	strcpy_s(field.Password, ARRAYCOUNT(field.Password), invest_account->investor_password.c_str());
	strcpy_s(field.UserProductInfo, ARRAYCOUNT(field.UserProductInfo), "EasyTrade2");
	switch (invest_account->orderapi_spi_pair.first->ReqUserLogin(&field, ++invest_account->ctp_order_request_id))
	{
	case 0:
		break;
	case -1:
	{
		msg = "failed to log in " + order_broker->broker_name + " ctp order server. network connection failed";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);	
	}
		break;
	case -2:
	{
		msg = "failed to log in " + order_broker->broker_name + " ctp order server. unprocessed requests exceed limit";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
		break;
	case -3:
	{
		msg = "failed to log in " + order_broker->broker_name + " ctp order server. requests sent per second exceed limit";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
		break;
	default:
		break;
	}
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void FutureOrderProcessor::OnFrontDisconnected(int nReason)
{
	switch (nReason)
	{
	case 0x1001:
	{
		msg = "failed to keep connection to " + order_broker->broker_name + " ctp order server. failed to read on network";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	break;
	case 0x1002:
	{
		msg = "failed to keep connection to " + order_broker->broker_name + " ctp order server. failed to write on network";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	break;
	case 0x2001:
	{
		msg = "failed to keep connection to " + order_broker->broker_name + " ctp order server. receive heartbeat timeout";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	break;
	case 0x2002:
	{
		msg = "failed to keep connection to " + order_broker->broker_name + " ctp order server. send heartbeat failed";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	break;
	case 0x2003:
	{
		msg = "failed to keep connection to " + order_broker->broker_name + " ctp order server. received errorneous packets";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	break;
	default:
		break;
	}
}

///登录请求响应,在登陆里自动做完结算确认
void FutureOrderProcessor::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to log in " + order_broker->broker_name + " ctp order server";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pRspUserLogin)
	{
		//TODO:no need to write succeed message to disk log file
		msg = "succeeded in logging in " + order_broker->broker_name + " ctp order server";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);

		invest_account->order_ref = atoi(pRspUserLogin->MaxOrderRef);

		if (strlen(order_shooter->ctp_trading_day) <= 0)
		{
			strcpy_s(order_shooter->ctp_trading_day, ARRAYCOUNT(order_shooter->ctp_trading_day), invest_account->orderapi_spi_pair.first->GetTradingDay());
		}

#pragma region 查询当日是否已经确认结算
		CThostFtdcQrySettlementInfoConfirmField field;
		ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		invest_account->orderapi_spi_pair.first->ReqQrySettlementInfoConfirm(&field, ++invest_account->ctp_order_request_id);
#pragma endregion	
	}
}

///报单录入请求响应(CTP收到报单指令，如果没有通过参数校验，拒绝接受报单指令)
void FutureOrderProcessor::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	char request_id[50];
	sprintf_s(request_id, ARRAYCOUNT(request_id), "%d", nRequestID);
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to insert order to CTP.";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pInputOrder)
	{
		msg = "succeeded in inserting order ";
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
}

///报单操作请求响应(CTP认为撤单指令有误)
void FutureOrderProcessor::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg ="failed to cancel order by CTP";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pInputOrderAction)
	{

	}
}

///投资者结算结果确认响应
void FutureOrderProcessor::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to confirm settlement info of" + order_broker->broker_name + " " + invest_account->investor_id;
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else
	{
		if (index == 0)
		{
			::SetEvent(Global::event);
		}
	}
}

///请求查询资金账户响应
void FutureOrderProcessor::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
	    msg = "failed to query account info of " + order_broker->broker_name + " " + invest_account->investor_id;
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pTradingAccount)
	{
		invest_account->available = pTradingAccount->Available;
		invest_account->close_profit = pTradingAccount->CloseProfit;
		//静态权益=上日结算-上次信用额度-上次质押金额+质押金额-出金金额+入金金额
		double prev_balance(pTradingAccount->PreBalance - pTradingAccount->PreCredit - pTradingAccount->PreMortgage + pTradingAccount->Mortgage - pTradingAccount->Withdraw + pTradingAccount->Deposit);
		//动态权益=静态权益+ 平仓盈亏+ 持仓盈亏 +资金差额- 手续费
		invest_account->balance = prev_balance
			+ pTradingAccount->CloseProfit + pTradingAccount->PositionProfit + pTradingAccount->CashIn - pTradingAccount->Commission;
		invest_account->margin = pTradingAccount->CurrMargin;
		invest_account->frozen = pTradingAccount->FrozenMargin + pTradingAccount->FrozenCommission;
		invest_account->position_profit = pTradingAccount->PositionProfit;
		invest_account->UpdateAccountInfo();
	}
}

///请求查询合约保证金率响应
void FutureOrderProcessor::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询合约手续费率响应
void FutureOrderProcessor::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询合约响应
void FutureOrderProcessor::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to query instrument";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pInstrument)
	{
		if (pInstrument->ProductClass == THOST_FTDC_PC_Futures &&
			(pInstrument->IsTrading))
		{
			if (FindContractFromNormal(pInstrument->InstrumentID, &Global::contract_dic) == Global::contract_dic.end())
			{
				ContractCredential contract_credential;
				contract_credential.exchange = pInstrument->ExchangeID;
				contract_credential.deliver_year = pInstrument->DeliveryYear;
				contract_credential.deliver_month = pInstrument->DeliveryMonth;
				contract_credential.volume_multiple = pInstrument->VolumeMultiple;
				contract_credential.price_tick = pInstrument->PriceTick;
				contract_credential.expire_date = pInstrument->ExpireDate;
				contract_credential.start_deliver_date = pInstrument->StartDelivDate;
				contract_credential.end_deliver_date = pInstrument->EndDelivDate;
				contract_credential.buy_margin_ratio = pInstrument->LongMarginRatio;
				contract_credential.sell_margin_ratio = pInstrument->ShortMarginRatio;

				ContractID contract_id;
				contract_id.contract = pInstrument->InstrumentID;
				contract_id.expire_flag = NEW;
				Global::contract_dic[contract_id] = contract_credential;//so the full contract dictionary is normal + new
			}
		}
	}
	if (bIsLast)
	{
		::SetEvent(Global::event);
	}
}

///请求查询投资者结算结果响应
void FutureOrderProcessor::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to query " + order_broker->broker_name + " " + invest_account->investor_id + " " + "settlement info";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pSettlementInfo)
	{
	}
}

///请求查询转帐银行响应
void FutureOrderProcessor::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询投资者持仓响应
void FutureOrderProcessor::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	PositionCredential pc;
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to query position of" + order_broker->broker_name + " " + invest_account->investor_id;
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pInvestorPosition && pInvestorPosition->Position > 0)
	{
		pc.contract = pInvestorPosition->InstrumentID;
		if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
		{
			pc.direction = BUY;
		}
		else if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Short)
		{
			pc.direction = SELL;
		}

		std::string exchange;
		auto it2 = FindContractFromNormalNew(pc.contract, &Global::contract_dic);
		if (it2 != Global::contract_dic.end())
		{
			exchange = it2->second.exchange;
		}
		auto it = FindPositionFromNormalNew(&pc, &invest_account->position_list);
		if (it != invest_account->position_list.end())
		{
			it->second.is_server_contained = true;
			//update profit
			if (it->second.profit != pInvestorPosition->PositionProfit)
			{
				it->second.UpdateProfit(pInvestorPosition->PositionProfit);
			}
			//update historical volume
			if (it->second.historical_volume != pInvestorPosition->YdPosition)
			{
				it->second.historical_volume = pInvestorPosition->Position - pInvestorPosition->TodayPosition;
			}
			//update volume
			if (it->second.historical_volume > 0)
			{
				it->second.UpdateVolume(it->second.volume + it->second.historical_volume);
			}
			//insert into subpositions
			if (it->second.historical_volume > 0)
			{
				it->second.sub_positions.push_back(std::make_pair(it->second.historical_volume, pInvestorPosition->PreSettlementPrice));
			}
			//update average price
			if (it->second.historical_volume > 0)
			{
				double volume_x_price = 0.0;
				for (auto it1 = it->second.sub_positions.begin(); it1 != it->second.sub_positions.end(); ++it1)
				{
					volume_x_price += (double)it1->first * it1->second;
				}
				it->second.UpdateAveragePrice(volume_x_price / it->second.volume);
			}
			//update margin
			if (it->second.margin != pInvestorPosition->UseMargin)
			{
				it->second.UpdateMargin(pInvestorPosition->UseMargin);
			}
			//update exchange
			it->second.exchange = exchange;
			if (it->first.expire_flag == NORMAL)
			{
				it->second.AddDataGridRow();
			}
		}
		else
		{
			pc.expire_flag = NEW;
			PositionDetailSync pd; 
			if (pd.AddDataGridRow() != -1)
			{
				pd.is_server_contained = true;

				pd.UpdateContract();
				pd.UpdateProfit(pInvestorPosition->PositionProfit);
				pd.UpdateDirection();
				//insert historical volume
				pd.historical_volume = pInvestorPosition->Position - pInvestorPosition->TodayPosition;
				//insert average price
				if (pd.historical_volume > 0)
				{
					pd.UpdateAveragePrice(pInvestorPosition->PreSettlementPrice);
				}
				//insert volume
				pd.volume = 0;
				if (pd.historical_volume > 0)
				{
					pd.UpdateVolume(pd.volume + pd.historical_volume);
				}
				pd.UpdateMargin(pInvestorPosition->UseMargin);
				//insert exchange
				pd.exchange = exchange;
				//insert into subpositions
				if (pd.historical_volume > 0)
				{
					pd.sub_positions.push_back(std::make_pair(pd.historical_volume, pInvestorPosition->PreSettlementPrice));
				}
				invest_account->position_list[pc] = pd;
			}
			
#pragma endregion
		}
	}
	if (bIsLast)
	{
		for (auto it2 = invest_account->position_list.begin(); it2 != invest_account->position_list.end();)
		{
			if (!it2->second.is_server_contained && it2->first.expire_flag != EXPIRE)
			{
				pc = it2->first;
				pc.expire_flag = EXPIRE;
				std::swap(invest_account->position_list[pc], it2->second);


				it2->second.DeleteDataGridRow();
				for (auto it3 = invest_account->position_list.begin(); it3 != invest_account->position_list.end(); ++it3)
				{
					if (it3->second.GetRowIndex() > it2->second.GetRowIndex())
					{
						it3->second.DecrementRowIndex();
					}
				}
				Global::RemoveContractForUnsubscribe(it2->first.contract.c_str());
				
				it2 = invest_account->position_list.erase(it2);
			}
			else
			{
				++it2;
			}
		}
	}
}

///请求查询结算信息确认响应
void FutureOrderProcessor::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to query " + order_broker->broker_name + " " + invest_account->investor_id + " " + "settlement confirm";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pSettlementInfoConfirm)
	{
		if (strcmp(pSettlementInfoConfirm->ConfirmDate, invest_account->orderapi_spi_pair.first->GetTradingDay()) == 0)
		{
			//have confirmed settlement today
			if (index == 0)
			{
				::SetEvent(Global::event);
			}
		}
		else
		{
			CThostFtdcSettlementInfoConfirmField field;
			ZeroMemory(&field, sizeof(field));
			strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
			strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
			invest_account->orderapi_spi_pair.first->ReqSettlementInfoConfirm(&field, ++invest_account->ctp_order_request_id);
		}
	}
	else
	{
		CThostFtdcSettlementInfoConfirmField field;
		ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		invest_account->orderapi_spi_pair.first->ReqSettlementInfoConfirm(&field, ++invest_account->ctp_order_request_id);
	}
}

///查询保证金监管系统经纪公司资金账户密钥响应
void FutureOrderProcessor::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///请求查询投资者品种/跨品种保证金响应
void FutureOrderProcessor::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///请求查询交易所保证金率响应
void FutureOrderProcessor::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询交易所调整保证金率响应
void FutureOrderProcessor::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询转帐流水响应
void FutureOrderProcessor::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询银期签约关系响应
void FutureOrderProcessor::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to query bound bank account";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pAccountregister)
	{
		if (pAccountregister->BankID[0] == THOST_FTDC_PID_ABCProtocal)
			invest_account->bound_bank_name = "农业银行";

		else if (pAccountregister->BankID[0] == THOST_FTDC_PID_CBCProtocal)
			invest_account->bound_bank_name = "中国银行";
		else if (pAccountregister->BankID[0] == THOST_FTDC_PID_BOCOMProtocal)
			invest_account->bound_bank_name = "交通银行";
		else if (pAccountregister->BankID[0] == THOST_FTDC_PID_CCBProtocal)
			invest_account->bound_bank_name = "建设银行";
		else if (pAccountregister->BankID[0] == THOST_FTDC_PID_ICBCProtocal)
			invest_account->bound_bank_name = "工商银行";
		else
			invest_account->bound_bank_name = "其他银行";

		invest_account->bound_bank_account = pAccountregister->BankAccount;

		/*bankID = bankID.substr(strlen(pAccountregister->BankAccount) - 4, 4);*/

	}
	else
	{
		invest_account->bound_bank_name = "no bank account bound";
		invest_account->bound_bank_account = "no bank account bound";
	}
}

///错误应答
void FutureOrderProcessor::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo)
	{
		char request_id[20];
		sprintf_s(request_id, ARRAYCOUNT(request_id), "%d", nRequestID);
		msg = std::string("failed to request ") + request_id;
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
}

///报单通知
void FutureOrderProcessor::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	//update order
	if (pOrder)
	{
		OrderCredential oc;
		oc.contract = pOrder->InstrumentID;
		oc.exchange = pOrder->ExchangeID;
		oc.order_sys_id = pOrder->OrderSysID;
		oc.front_id = pOrder->FrontID;
		oc.session_id = pOrder->SessionID;
		oc.order_ref = pOrder->OrderRef;

		auto it1 = invest_account->_order_list.find(oc);
		if (it1 != invest_account->_order_list.end())
		{
			int index1 = std::distance(invest_account->_order_list.begin(), it1);
			it1->second.UpdateTradeVolume(pOrder->VolumeTraded);
			it1->second.UpdateStatus(pOrder->StatusMsg);
		}
		else
		{
			size_t index2 = invest_account->_order_list.size();
			OrderDetailSync od;
			od.expire_flag = NEW;

			if (od.AddDataGridRow() != -1)
			{
				od.UpdateContract();
				//insert direction
				if (pOrder->Direction == THOST_FTDC_D_Sell)
				{
					od.direction = SELL;
				}
				else if (pOrder->Direction == THOST_FTDC_D_Buy)
				{
					od.direction = BUY;
				}
				od.UpdateDirection();

#pragma region insert open_flag
				if (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open)
				{
					od.open_flag = OPEN;//open position
				}
				else if (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Close || pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday || pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
				{
					od.open_flag = CLOSE;//close position
				}
				else if (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose)
				{
					od.open_flag = FORCE_CLOSE;//强平
				}
				else if (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_ForceOff)
				{
					od.open_flag = FORCE_OFF;//强减
				}
				else if (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
				{
					od.open_flag = LOCAL_FORCE_CLOSE;//本地强平
				}
				od.UpdateOpenFlag();
#pragma endregion

				od.UpdateStatus(pOrder->StatusMsg);
				od.UpdateVolume(pOrder->VolumeTotalOriginal);
				od.UpdatePrice(pOrder->LimitPrice);
				od.UpdateInsertTime(pOrder->InsertDate + std::string(" ") + pOrder->InsertTime);
				od.UpdateTradeVolume(pOrder->VolumeTraded);
				od.UpdateExchange();
				od.UpdateFrontID();
				od.UpdateSessionID();
				od.UpdateOrderRef();

				invest_account->_order_list[oc] = od;
				if (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open)
				{
					//5s后未成交自动撤单
					order_shooter->future_order_manager.RegisterOpenOrder(order_broker, invest_account, &oc);
				}
			}
		}
	}
}

///成交通知
void FutureOrderProcessor::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	if (pTrade)
	{
		if (pTrade->TradeSource == THOST_FTDC_TSRC_NORMAL)
		{
			PositionCredential pc;
			pc.contract = pTrade->InstrumentID;
			PositionListSync::iterator it;
#pragma region open position
			if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)
			{
				pc.direction = pTrade->Direction == THOST_FTDC_D_Sell ? SELL : BUY;
				it = FindPositionFromNormalNew(&pc, &invest_account->position_list);
				if (it != invest_account->position_list.end())
				{
					//insert into subpositions
					it->second.sub_positions.push_back(std::make_pair(pTrade->Volume, pTrade->Price));
					it->second.UpdateVolume(it->second.volume + pTrade->Volume);
					//update average_price
					double volume_x_price = 0.0;
					for (auto it1 = it->second.sub_positions.begin(); it1 != it->second.sub_positions.end(); ++it1)
					{
						volume_x_price += (double)it1->first * it1->second;
					}
					it->second.UpdateAveragePrice(volume_x_price / it->second.volume);
					//TODO calculate margin and render
				}
				else
				{
					pc.expire_flag = NEW;
					PositionDetailSync pd(&pc);
					if (pd.AddDataGridRow() != -1)
					{
						pd.UpdateContract();
						pd.UpdateDirection();
						pd.UpdateAveragePrice(pTrade->Price);
						pd.UpdateVolume(pTrade->Volume);
						//TODO:insert profit
						pd.profit = 0.0;
						//TODO: calculate margin and render
						pd.margin = 0.0;
						//insert into subpositions
						pd.sub_positions.push_back(std::make_pair(pd.volume, pd.average_price));

						invest_account->position_list[pc] = pd;
						it = invest_account->position_list.find(pc);
						it->second.pc = &(it->first);

						Global::AddContractForSubscribe(pTrade->InstrumentID);
					}
				}
			}
#pragma endregion
			
#pragma region close position
			else if (pTrade->OffsetFlag == THOST_FTDC_OF_Close || pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday || pTrade->OffsetFlag == THOST_FTDC_OF_CloseYesterday || pTrade->OffsetFlag == THOST_FTDC_OF_ForceClose || pTrade->OffsetFlag == THOST_FTDC_OF_ForceOff || pTrade->OffsetFlag == THOST_FTDC_OF_LocalForceClose)
			{
				pc.direction = pTrade->Direction == THOST_FTDC_D_Sell ? BUY : SELL;
				it = FindPositionFromNormalNew(&pc, &invest_account->position_list);
				if (it != invest_account->position_list.end())
				{
					//update volume
					if ((pTrade->OffsetFlag == THOST_FTDC_OF_CloseYesterday || pTrade->OffsetFlag == THOST_FTDC_OF_Close) && it->second.exchange == "SHFE")
					{
						it->second.historical_volume -= pTrade->Volume;
					}
					it->second.volume -= pTrade->Volume;

					if (it->second.volume > 0)
					{
						it->second.UpdateVolume();
					}
					else
					{
						pc = it->first;
						pc.expire_flag = EXPIRE;
						std::swap(invest_account->position_list[pc], it->second);

						it->second.DeleteDataGridRow();
						for (auto it1 = invest_account->position_list.begin(); it1 != invest_account->position_list.end(); ++it1)
						{
							if (it1->second.GetRowIndex() > it->second.GetRowIndex())
							{
								it1->second.DecrementRowIndex();
							}
						}
						Global::RemoveContractForUnsubscribe(pTrade->InstrumentID);

						invest_account->position_list.erase(it);
					}
				}
			}
#pragma endregion
		}
	}
}

///报单录入错误回报（交易所收到报单后认为报单错误）
void FutureOrderProcessor::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to insert order to exchange.";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pInputOrder)
	{

	}
}

///撤单操作错误回报（交易所收到撤单请求后认为撤单错误）
void FutureOrderProcessor::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	if (CTPShared::IsErrorRspInfo(pRspInfo))
	{
		msg = "failed to cancel by exchange.";
		CTPShared::FormatRspInfo(pRspInfo, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pOrderAction)
	{

	}
}

///合约交易状态通知
void FutureOrderProcessor::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
}

///保证金监控中心用户令牌
void FutureOrderProcessor::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken)
{
}

///请求查询签约银行响应
void FutureOrderProcessor::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询监控中心用户令牌
void FutureOrderProcessor::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///期货发起银行资金转期货通知
void FutureOrderProcessor::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	if (IsErrorRspInfo(pRspTransfer))
	{
		msg = "failed to transfer fund from bank to future";
		FormatRspInfo(pRspTransfer, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
}

///期货发起期货资金转银行通知
void FutureOrderProcessor::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	if (IsErrorRspInfo(pRspTransfer))
	{
		msg = "failed to transfer fund from future to bank";
		FormatRspInfo(pRspTransfer, &msg);
		(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
	}
	else if (pRspTransfer)
	{

	}
}

///期货发起查询银行余额通知
void FutureOrderProcessor::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
{
	invest_account->bound_bank_balance = max(pNotifyQueryAccount->BankUseAmount, pNotifyQueryAccount->BankFetchAmount);
}

///期货发起银行资金转期货错误回报
void FutureOrderProcessor::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
	msg = "failed to transfer fund from bank to future";
	CTPShared::FormatRspInfo(pRspInfo, &msg);
	(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
}

///期货发起期货资金转银行错误回报
void FutureOrderProcessor::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
	msg = "failed to transfer fund from future to bank";
	CTPShared::FormatRspInfo(pRspInfo, &msg);
	(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
}

///期货发起查询银行余额错误回报
void FutureOrderProcessor::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
{
	msg = "failed to query bound bank account balance";
	CTPShared::FormatRspInfo(pRspInfo, &msg);
	(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
}

///期货发起银行资金转期货应答
void FutureOrderProcessor::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	msg = "failed to transfer fund from bank to future";
	CTPShared::FormatRspInfo(pRspInfo, &msg);
	(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
}

///期货发起期货资金转银行应答
void FutureOrderProcessor::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	msg = "failed to transfer fund from future to bank";
	CTPShared::FormatRspInfo(pRspInfo, &msg);
	(*order_shooter->handler)(msg.c_str(), /*(size_t)index*/"", NULL);
}

///期货发起查询银行余额应答
void FutureOrderProcessor::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

bool FutureOrderProcessor::IsErrorRspInfo(CThostFtdcRspTransferField*pRspTransfer)
{
	bool IsError(((pRspTransfer) && (pRspTransfer->ErrorID != 0)));
	return IsError;
}

void FutureOrderProcessor::FormatRspInfo(CThostFtdcRspTransferField* pRspTransfer, std::string* entry_msg)
{
	*entry_msg += std::string(" error message ") + pRspTransfer->ErrorMsg + ".";
}