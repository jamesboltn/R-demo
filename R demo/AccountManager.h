#pragma once
#include"TradeStruct.h"
class AccountManager
{
protected:
	virtual bool RetrieveAccountList(BrokerInfoList* broker_info_list) = 0;
	virtual void UpdateAccount(BrokerInfo* broker_info, InvestAccountSync* invest_account) = 0;
	virtual void UpdatePosition(BrokerInfo* broker_info, InvestAccountSync* invest_account) = 0;
	virtual void TransferCapitalFromBrokerToBank(BrokerInfo* broker_info, InvestAccountSync* invest_account, double amount) = 0;//证银，期银转账
	virtual void TransferCapitalFromBankToBroker(BrokerInfo* broker_info, InvestAccountSync* invest_account, double amount) = 0;//银证，银期转账
	virtual void QueryPositionDetail(BrokerInfo* broker_info, InvestAccountSync* invest_account) = 0;//仅作期货测试用，后续删除
};
