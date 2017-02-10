#pragma once
#include"AccountManager.h"

class FutureAccountManager :
	public AccountManager
{
private:
public:
	bool RetrieveAccountList(BrokerInfoList* broker_info_list);
	void UpdateAccount(BrokerInfo* broker_info, InvestAccountSync* invest_account);
	void UpdatePosition(BrokerInfo* broker_info, InvestAccountSync* invest_account);
	void TransferCapitalFromBrokerToBank(BrokerInfo* broker_info, InvestAccountSync* invest_account, double amount);
	void TransferCapitalFromBankToBroker(BrokerInfo* broker_info, InvestAccountSync* invest_account, double amount);
	void QueryPositionDetail(BrokerInfo
		* broker_info, InvestAccountSync* invest_account);//仅作期货测试用，后续删除
};
