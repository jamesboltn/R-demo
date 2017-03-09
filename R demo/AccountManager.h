#pragma once
#include"TradeStruct.h"
class AccountManager
{
protected:
	virtual bool RetrieveAccountList(OrderBrokerList* order_broker_list) = 0;
	virtual void UpdateAccount(OrderBroker* order_broker, InvestAccountSync* invest_account) = 0;
	virtual void UpdatePosition(OrderBroker* order_broker, InvestAccountSync* invest_account) = 0;
	virtual void TransferCapitalFromBrokerToBank(OrderBroker* order_broker, InvestAccountSync* invest_account, double amount) = 0;//证银，期银转账
	virtual void TransferCapitalFromBankToBroker(OrderBroker* order_broker, InvestAccountSync* invest_account, double amount) = 0;//银证，银期转账
	virtual void QueryPositionDetail(OrderBroker* order_broker, InvestAccountSync* invest_account) = 0;//仅作期货测试用，后续删除
};
