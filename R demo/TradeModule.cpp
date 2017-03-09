#pragma once
#include"TradeModule.h"
#include"SQLite.h"
#include"GlobalObject.h"

void TradeModule::Load()
{
	CreateFolders();
	CreateDataBaseTables();
	GetInvestAccountListFromDataBase();
	GetOrderFrontListFromDataBase();
}
TradeModule &TradeModule::CreateFolders()
{
	::CreateDirectoryW(L"CTPOrderCon", NULL);
	::CreateDirectoryW(L"CTPQuoteConForTrade", NULL);
	return *this;
}
TradeModule &TradeModule::CreateDataBaseTables()
{
	using Global::db;
	//table: broker
	db.Field("id", TEXT).Field("name", TEXT).Create("broker");
	//table: contract
	db.Field("id", TEXT).Field("name", TEXT).Field("exchange", TEXT).Field("deliver_year", INTEGER).Field("deliver_month", INTEGER).Field("volume_multiple", INTEGER).Field("price_tick", _DOUBLE).Field("expire_date", TEXT).Field("start_deliver_date", TEXT).Field("end_deliver_date", TEXT).Field("buy_margin_ratio", _DOUBLE).Field("sell_margin_ratio", _DOUBLE).Create("contract");
	//table: investor
	db.Field("broker_id", INTEGER).Field("id", TEXT).Field("password", TEXT).Field("balance", _DOUBLE).Field("position_profit", _DOUBLE).Field("close_profit", _DOUBLE).Field("margin", _DOUBLE).Field("frozen", _DOUBLE).Field("available", _DOUBLE).Create("investor");
	//table: order
	db.Field("broker_id", INTEGER).Field("investor_id", TEXT).Field("contract_id", TEXT).Field("direction", TEXT).Field("open_flag", TEXT).Field("status", TEXT).Field("volume", INTEGER).Field("price", _DOUBLE).Field("insert_time", TEXT).Field("trade_volume", INTEGER).Field("exchange", TEXT).Field("front_id", INTEGER).Field("session_id", INTEGER).Create("order");
	//table: order_front
	db.Field("broker_id", INTEGER).Field("front_addr", TEXT).Create("order_front");
	//table: position
	db.Field("broker_id", INTEGER).Field("investor_id", TEXT).Field("contract_id", TEXT).Field("profit", _DOUBLE).Field("direction", TEXT).Field("average_price", _DOUBLE).Field("volume", INTEGER).Field("margin", _DOUBLE).CreateTable("position");
	//table: quote_account
	db.Field("broker_id", INTEGER).Field("id", TEXT).Field("password", TEXT).Create("quote_account");
	//table: quote_front
	db.Field("broker_id", INTEGER).Field("front_addr", TEXT).Create("quote_front");
	return *this;
}
TradeModule &TradeModule::GetInvestAccountListFromDataBase()
{
	using Global::TradeModule::order_broker_lst;
	std::vector<std::vector<std::string>> result_set;
	std::string statement = "select a.*, b.name as broker_name from Investor a inner join Broker b  on b.id = a.broker_id";
	/*if (!(*handler)(db->ExecuteSelect(statement.c_str(), &result_set, 10), page_name))*/
	//TODO:use the commented if instead
	if(Global::db.ExecuteSelect(statement.c_str(), &result_set, 10) == "ok")
	{
		if (!result_set.empty())
		{
			OrderBroker order_broker;
			char broker_id[8];
			std::string broker_unique_name;
			InvestAccountSync invest_account;
			for (auto it = result_set.begin(); it != result_set.end(); ++it)
			{
				broker_unique_name = (*it)[9] + (*it)[0];
				if (order_broker_lst.find(broker_unique_name) == order_broker_lst.end())
				{
					order_broker_lst[broker_unique_name] = order_broker;
					order_broker_lst[broker_unique_name].broker_id = atoi((*it)[0].c_str());
					order_broker_lst[broker_unique_name].broker_name = (*it)[9];
				}
				invest_account.investor_id = (*it)[1];
				invest_account.investor_password = (*it)[2];
				invest_account.balance = atof((*it)[3].c_str());
				invest_account.position_profit = atof((*it)[4].c_str());
				invest_account.close_profit = atof((*it)[5].c_str());
				invest_account.margin = atof((*it)[6].c_str());
				invest_account.frozen = atof((*it)[7].c_str());
				invest_account.available = atof((*it)[8].c_str());
				order_broker_lst[broker_unique_name].invest_account_list[invest_account.investor_id] = invest_account;
			}
		}
	}
	return *this;
}
TradeModule &TradeModule::GetOrderFrontListFromDataBase()
{
	using Global::TradeModule::order_broker_lst;
	std::vector<std::vector<std::string>> result_set;
	std::string statement = "select a.*, b.name as broker_name from Order_Front a inner join Broker b  on b.id = a.broker_id";
	//TODO:use the commented if instead
	/*if (!(*handler)(db->ExecuteSelect(statement.c_str(), &result_set, 10), page_name))*/
	if(Global::db.ExecuteSelect(statement.c_str(), &result_set, 3) == "ok")
	{
		if (!result_set.empty())
		{
			std::string broker_unique_name;
			for (auto it = result_set.begin(); it != result_set.end(); ++it)
			{
				broker_unique_name = (*it)[2] + (*it)[0];
				if (order_broker_lst.find(broker_unique_name) != order_broker_lst.end())
				{
					order_broker_lst[broker_unique_name].order_front_list.push_back(std::make_pair((*it)[1], TESTING));
				}
			}
		}
	}
	return *this;
}
