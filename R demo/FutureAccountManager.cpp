#pragma once
#include"FutureAccountManager.h"
#include"Utilities.h"
#include"ThostFtdcUserApiStruct.h"
#pragma warning(push)
#pragma warning(disable:4100)
#pragma warning(disable:4820)
#pragma warning(disable:4265)
#include"ThostFtdcTraderApi.h"
#pragma warning(pop)
#include"ClientDataType.h"

void FutureAccountManager::UpdateAccount(OrderBroker* order_broker, InvestAccountSync* invest_account)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcQryTradingAccountField field;
		::ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		(invest_account->orderapi_spi_pair.first)->ReqQryTradingAccount(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::UpdatePosition(OrderBroker* order_broker, InvestAccountSync* invest_account)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcQryInvestorPositionField field;
		::ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		(invest_account->orderapi_spi_pair.first)->ReqQryInvestorPosition(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::TransferCapitalFromBankToBroker(OrderBroker* order_broker, InvestAccountSync* invest_account, double amount)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcReqTransferField field;
		ZeroMemory(&field, sizeof(field));

		strcpy_s(field.BankID, ARRAYCOUNT(field.BankID), invest_account->bound_bank_account.c_str());
		strcpy_s(field.BankBranchID, ARRAYCOUNT(field.BankBranchID), "0000");		//必须有
		strcpy_s(field.BankPassWord, ARRAYCOUNT(field.BankPassWord), "399884");

		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.AccountID, ARRAYCOUNT(field.AccountID), invest_account->investor_id.c_str());
		strcpy_s(field.Password, ARRAYCOUNT(field.Password), invest_account->investor_password.c_str());
		field.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;	//明文核对
		strcpy_s(field.CurrencyID, ARRAYCOUNT(field.CurrencyID), "RMB"); 					//币种：RMB-人民币 USD-美圆 HKD-港元=
		field.TradeAmount = amount;

		(invest_account->orderapi_spi_pair.first)->ReqFromBankToFutureByFuture(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::TransferCapitalFromBrokerToBank(OrderBroker* order_broker, InvestAccountSync* invest_account, double amount)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcReqTransferField field;
		ZeroMemory(&field, sizeof(field));

		strcpy_s(field.BankID, ARRAYCOUNT(field.BankID), invest_account->bound_bank_account.c_str());
		strcpy_s(field.BankBranchID, ARRAYCOUNT(field.BankBranchID), "0000");		//必须有
		strcpy_s(field.BankPassWord, ARRAYCOUNT(field.BankPassWord), "399884");

		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.AccountID, ARRAYCOUNT(field.AccountID), invest_account->investor_id.c_str());
		strcpy_s(field.Password, ARRAYCOUNT(field.Password), invest_account->investor_password.c_str());
		field.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;	//明文核对
		strcpy_s(field.CurrencyID, ARRAYCOUNT(field.CurrencyID), "RMB"); 					//币种：RMB-人民币 USD-美圆 HKD-港元=
		field.TradeAmount = amount;

		(invest_account->orderapi_spi_pair.first)->ReqFromFutureToBankByFuture(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::QueryPositionDetail(OrderBroker* order_broker, InvestAccountSync* invest_account)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcQryInvestorPositionDetailField field;
		::ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), order_broker->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		(invest_account->orderapi_spi_pair.first)->ReqQryInvestorPositionDetail(&field, ++invest_account->ctp_order_request_id);
	}
}