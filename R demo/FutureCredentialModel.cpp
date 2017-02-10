#pragma once
#include"FutureCredentialModel.h"
#include"SQLite.h"

void FutureCredentialModel::CreateTable(SQLite* db, const char* table_name, DatabaseErrorMessageHandler handler)
{
	db->AddFieldNotNull("investor_id", TEXT);
	db->AddFieldNotNull("balance", _DOUBLE);
	db->AddFieldNotNull("position_profit", _DOUBLE);
	db->AddFieldNotNull("close_profit", _DOUBLE);
	db->AddFieldNotNull("margin", _DOUBLE);
	db->AddFieldNotNull("frozen", _DOUBLE);
	db->AddFieldNotNull("available", _DOUBLE);
	(*handler)(db->CreateTable(table_name));
}

void FutureCredentialModel::Insert(SQLite* db, const char* table_name, InvestAccountSync* invest_account, DatabaseErrorMessageHandler handler)
{
	db->AddFieldForInsert("investor_id", invest_account->investor_id.c_str());
	db->AddFieldForInsert("balance", invest_account->balance);
	db->AddFieldForInsert("position_profit", invest_account->position_profit);
	db->AddFieldForInsert("close_profit", invest_account->close_profit);
	db->AddFieldForInsert("margin", invest_account->margin);
	db->AddFieldForInsert("frozen", invest_account->frozen);
	db->AddFieldForInsert("available", invest_account->available);
	(*handler)(db->Insert(table_name));
}

void FutureCredentialModel::Retrieve(SQLite* db, const char* table_name, InvestAccountSync* invest_account, AccountSpecificMsgHandler handler, unsigned int index)
{
	std::vector<std::vector<std::string>> result_set;
	std::string statement = "SELECT * FROM " + std::string(table_name);
	if (!(*handler)(db->ExecuteSelect(statement.c_str(), &result_set, 7), (size_t)index))
	{
		if (!result_set.empty())
		{
			std::wstring account_info;
			for (auto it = result_set.begin(); it != result_set.end(); ++it)
			{
				invest_account->balance = atof(result_set[0][1].c_str());
				invest_account->position_profit = atof(result_set[0][2].c_str());
				invest_account->close_profit = atof(result_set[0][3].c_str());
				invest_account->margin = atof(result_set[0][4].c_str());
				invest_account->frozen = atof(result_set[0][5].c_str());;
				invest_account->available = atof(result_set[0][6].c_str());
			}
		}
	}
}