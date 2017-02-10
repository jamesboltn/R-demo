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

bool FutureAccountManager::RetrieveAccountList(BrokerInfoList* broker_info_list)
{
#ifdef  _DEBUG                   
	FILE* fp;
	std::string current_time_str = DateTime::GetCurrentTimeString();
	//TODO:the trading start and end time will be set by user in the menu
	if((current_time_str >= trade_first_start_time && current_time_str < trade_first_end_time)
		||(current_time_str >= trade_second_start_time && current_time_str <= "23:59:59")
		|| (current_time_str >= "00:00:00" && current_time_str <= trade_second_end_time))
	{
		fopen_s(&fp, (debug_exe_dir + "\\future_credential_simnow24.csv").c_str(), "r");
	}
	else
	{
		fopen_s(&fp, (debug_exe_dir + "\\future_credential_simnow24.csv").c_str(), "r");
	}
	
#else
	FILE* fp;
	fopen_s(&fp, "future_credential.csv", "r");
#endif 

	int line_count(0);
	char* line;
	char buffer[1024];
	std::string token;
	std::vector<std::string> tokens;
	std::vector<Front> fronts;
	size_t account_num;

	BrokerInfo broker_info; 
	broker_info.quote_front.second = broker_info.order_front.second = TESTING; 
	
	InvestAccountSync invest_account; 
	invest_account.bound_bank_name = ""; 
	invest_account.bound_bank_account = ""; 
	invest_account.ctp_order_request_id = 0; 
	invest_account.ctp_quote_request_id = 0;
	invest_account.balance = 0.0;
	invest_account.position_profit = 0.0;
	invest_account.close_profit = 0.0;
	invest_account.margin = 0.0;
	invest_account.frozen = 0.0;
	invest_account.available = 0.0;

	if (fp != NULL)
	{
		while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)
		{
			++line_count;
			if (line_count == 1)continue;

			String::CharStr::RemoveLastChar(line);
			broker_info.invest_account_list.clear();

			//retrieve broker_name, broker_id 
			FileIO::CSV::GetToken(&token, line, 1);
			broker_info.broker_name = token;

			FileIO::CSV::GetToken(&token, line, 2);
			broker_info.broker_id = token;
			//retrieve order_front, quote_front
			FileIO::CSV::GetToken(&token, line, 5);
			String::Split(&token, ';', &fronts);
			for (auto it = fronts.begin(); it != fronts.end(); ++it)
			{
				broker_info.order_front.first.push_back(std::make_pair(*it, TESTING));
			}

			FileIO::CSV::GetToken(&token, line, 6);
			String::Split(&token, ';', &fronts);

			for (auto it = fronts.begin(); it != fronts.end(); ++it)
			{
				broker_info.quote_front.first.push_back(std::make_pair(*it, TESTING));
			}
			//retrieve invest_account(investor_id, user_id, allow_login, investor_password), account_type
			FileIO::CSV::GetToken(&token, line, 7);
			if (token == "real")
			{
				broker_info.account_type = REAL;
			}
			else if (token == "simulate")
			{
				broker_info.account_type = SIMULATE;
			}

			FileIO::CSV::GetToken(&token, line, 3);
			String::Split(&token, ';', &tokens);
			account_num = tokens.size();
			for (auto it = tokens.begin(); it != tokens.end(); ++it)
			{
				invest_account.investor_id = *it;
				invest_account.user_id = invest_account.investor_id;
				broker_info.invest_account_list.push_back(invest_account);
			}

			FileIO::CSV::GetToken(&token, line, 4);
			String::Split(&token, ';', &tokens);
			for (size_t i = 0; i < min(account_num, tokens.size()); ++i)
			{
				broker_info.invest_account_list[i].investor_password = tokens[i];
			}

			broker_info_list->push_back(broker_info);
			broker_info.order_front.first.clear();
			broker_info.quote_front.first.clear();
		}

		if (!broker_info_list->empty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void FutureAccountManager::UpdateAccount(BrokerInfo* broker_info, InvestAccountSync* invest_account)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcQryTradingAccountField field;
		::ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		(invest_account->orderapi_spi_pair.first)->ReqQryTradingAccount(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::UpdatePosition(BrokerInfo* broker_info, InvestAccountSync* invest_account)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcQryInvestorPositionField field;
		::ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		(invest_account->orderapi_spi_pair.first)->ReqQryInvestorPosition(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::TransferCapitalFromBankToBroker(BrokerInfo* broker_info, InvestAccountSync* invest_account, double amount)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcReqTransferField field;
		ZeroMemory(&field, sizeof(field));

		strcpy_s(field.BankID, ARRAYCOUNT(field.BankID), invest_account->bound_bank_account.c_str());
		strcpy_s(field.BankBranchID, ARRAYCOUNT(field.BankBranchID), "0000");		//必须有
		strcpy_s(field.BankPassWord, ARRAYCOUNT(field.BankPassWord), "399884");

		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());
		strcpy_s(field.AccountID, ARRAYCOUNT(field.AccountID), invest_account->investor_id.c_str());
		strcpy_s(field.Password, ARRAYCOUNT(field.Password), invest_account->investor_password.c_str());
		field.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;	//明文核对
		strcpy_s(field.CurrencyID, ARRAYCOUNT(field.CurrencyID), "RMB"); 					//币种：RMB-人民币 USD-美圆 HKD-港元=
		field.TradeAmount = amount;

		(invest_account->orderapi_spi_pair.first)->ReqFromBankToFutureByFuture(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::TransferCapitalFromBrokerToBank(BrokerInfo* broker_info, InvestAccountSync* invest_account, double amount)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcReqTransferField field;
		ZeroMemory(&field, sizeof(field));

		strcpy_s(field.BankID, ARRAYCOUNT(field.BankID), invest_account->bound_bank_account.c_str());
		strcpy_s(field.BankBranchID, ARRAYCOUNT(field.BankBranchID), "0000");		//必须有
		strcpy_s(field.BankPassWord, ARRAYCOUNT(field.BankPassWord), "399884");

		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());
		strcpy_s(field.AccountID, ARRAYCOUNT(field.AccountID), invest_account->investor_id.c_str());
		strcpy_s(field.Password, ARRAYCOUNT(field.Password), invest_account->investor_password.c_str());
		field.SecuPwdFlag = THOST_FTDC_BPWDF_BlankCheck;	//明文核对
		strcpy_s(field.CurrencyID, ARRAYCOUNT(field.CurrencyID), "RMB"); 					//币种：RMB-人民币 USD-美圆 HKD-港元=
		field.TradeAmount = amount;

		(invest_account->orderapi_spi_pair.first)->ReqFromFutureToBankByFuture(&field, ++invest_account->ctp_order_request_id);
	}
}

void FutureAccountManager::QueryPositionDetail(BrokerInfo* broker_info, InvestAccountSync* invest_account)
{
	if (invest_account->orderapi_spi_pair.first)
	{
		CThostFtdcQryInvestorPositionDetailField field;
		::ZeroMemory(&field, sizeof(field));
		strcpy_s(field.BrokerID, ARRAYCOUNT(field.BrokerID), broker_info->broker_id.c_str());
		strcpy_s(field.InvestorID, ARRAYCOUNT(field.InvestorID), invest_account->investor_id.c_str());
		(invest_account->orderapi_spi_pair.first)->ReqQryInvestorPositionDetail(&field, ++invest_account->ctp_order_request_id);
	}
}