#pragma once
#include"InvestAccountSync.h"
void InvestAccountSync::UpdateAccountInfo()
{
	std::string account_info;

	account_info = "available ";
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", available);
	account_info += buffer + std::string(" ");

	account_info += "close profit ";
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", close_profit);
	account_info += buffer + std::string(" ");

	account_info += "balance ";
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", balance);
	account_info += buffer + std::string(" ");

	account_info += "margin ";
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", margin);
	account_info += buffer + std::string(" ");

	account_info += "frozen ";
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", frozen);
	account_info += buffer + std::string(" ");

	account_info += "risk ";
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f%%", balance == 0.0 ? 0.0 : (margin / balance * 100.0));
	account_info += buffer + std::string(" ");

	RefreshControl("static_account_info", account_info.c_str());
}