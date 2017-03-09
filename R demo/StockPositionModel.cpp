#pragma once
#include"StockPositionModel.h"
#include"SQLite.h"

std::string database_error_message;
void StockPositionModel::CreateTable(SQLite* db, const char* table_name, DatabaseErrorMessageHandler handler)
{
	db->AddFieldNotNull("volume", INTEGER);
	db->AddFieldNotNull("trade_date", TEXT);
	database_error_message = db->CreateTable(table_name);
	if (handler != NULL)
	{
		(*handler)(database_error_message);
	}
}
void StockPositionModel::Retrieve(SQLite* db, const char* table_name, std::multiset<PositionDetailSync, CompareByValue>* position_list, AccountSpecificMsgHandler handler, const char* page_name)
{
	std::vector<std::vector<std::string>> result_set;
	std::string statement = "SELECT * FROM " + std::string(table_name);
	database_error_message = db->ExecuteSelect(statement.c_str(), &result_set, 2);

	if (database_error_message != "ok")
	{
		if (handler != NULL)
		{
			(*handler)(database_error_message, page_name);
		}
	}
	else
	{
		if (!result_set.empty())
		{
			position_list->clear();
			PositionDetailSync pd;
			pd.sell_order_id = "";
			for (auto it = result_set.begin(); it != result_set.end(); ++it)
			{
				pd.volume = atoi((*it)[0].c_str());
				pd.trade_date = (*it)[1].c_str();
				position_list->insert(pd);
			}
		}
	}
}
void StockPositionModel::Update(SQLite* db, const char* table_name, std::multiset<PositionDetailSync, CompareByValue>* position_list, AccountSpecificMsgHandler handler, const char* page_name)
{
	//delete before insertion
	std::string statement = "DELETE FROM " + std::string(table_name);
	database_error_message = db->Delete(statement.c_str());
	if (database_error_message != "ok")
	{
		if (handler != NULL)
		{
			(*handler)(database_error_message, page_name);
		}
		return;
	}
	for (auto it = position_list->begin(); it != position_list->end(); ++it)
	{
		db->AddFieldForInsert("volume", it->volume);
		db->AddFieldForInsert("trade_date", it->trade_date.c_str());
		database_error_message = db->Insert(table_name);
	}
}

