#pragma once
#include"PositionListSync.h"
#include"ModelDataType.h"
#include"SQLite.h"

class FuturePositionModel
{
public:
	static void CreateTable(SQLite* db, const char* table_name, DatabaseErrorMessageHandler handler)
	{
		db->AddFieldNotNull("contract", TEXT);
		db->AddFieldNotNull("profit", _DOUBLE);
		db->AddFieldNotNull("direction", TEXT);
		db->AddFieldNotNull("average_price", _DOUBLE);
		db->AddFieldNotNull("volume", INTEGER);
		db->AddFieldNotNull("margin", _DOUBLE);
		(*handler)(db->CreateTable(table_name));
	}

	static void Retrieve(SQLite* db, const char* table_name, PositionListSync* position_list, AccountSpecificMsgHandler handler, const char* page_name)
	{
		//insert profit
		it->second.UpdateProfit();
		//insert direction
		it->second.UpdateDirection();
		//insert average_price
		it->second.UpdateAveragePrice();
		//insert volume
		it->second.UpdateVolume();
		//insert margin
		it->second.UpdateMargin();

		std::vector<std::vector<std::string>> result_set;
		std::string statement = "SELECT * FROM " + std::string(table_name);
		if (!(*handler)(db->ExecuteSelect(statement.c_str(), &result_set, 6), page_name))
		{
			if (!result_set.empty())
			{
				position_list->clear();
				PositionCredential pc;
				pc.expire_flag = NORMAL;
				PositionDetailSync pd; 
				pd.pc = &pc;
				pd.is_server_contained = false; 
				pd.historical_volume = 0;
				for (auto it = result_set.begin(); it != result_set.end(); ++it)
				{
					pc.contract = (*it)[0];
					pd.UpdateContract();

					pd.profit = atof((*it)[1].c_str());
					if ((*it)[2] == "sell")
					{
						pc.direction = SELL;
					}
					else if ((*it)[2] == "buy")
					{
						pc.direction = BUY;
					}
					pd.UpdateDirection();

					pd.average_price = atof((*it)[3].c_str());
					pd.volume = atoi((*it)[4].c_str());
					pd.margin = atof((*it)[5].c_str());
					(*position_list)[pc] = pd;
				}
			}
		}
	}

	static void Update(SQLite* db, const char* table_name, PositionListSync* position_list, AccountSpecificMsgHandler handler, const char* page_name)
	{
		for (auto it = position_list->begin(); it != position_list->end(); ++it)
		{
			//save new positions to database 
			if (it->first.expire_flag == NEW)
			{
				if (it->first.direction == SELL)
				{
					db->AddFieldForInsert("direction", "sell");
				}
				else if (it->first.direction == BUY)
				{
					db->AddFieldForInsert("direction", "buy");
				}

				db->AddFieldForInsert("contract", it->first.contract.c_str());
				db->AddFieldForInsert("profit", it->second.profit);
				db->AddFieldForInsert("average_price", it->second.average_price);
				db->AddFieldForInsert("volume", it->second.volume);
				db->AddFieldForInsert("margin", it->second.margin);
				(*handler)(db->Insert(table_name), page_name);
			}
			//delete expire positions from database
			else if (it->first.expire_flag == EXPIRE)
			{
				std::string direction;
				char statement[MAX_PATH];
				if (it->first.direction == SELL)
				{
					direction = "sell";
				}
				else if (it->first.direction == BUY)
				{
					direction = "buy";
				}
				sprintf_s(statement, ARRAYCOUNT(statement), "DELETE FROM %s WHERE contract = '%s' AND direction = '%s';", table_name, it->first.contract.c_str(), direction.c_str());
				(*handler)(db->Delete(statement), page_name);
			}
			//update position list to database
			else if (it->first.expire_flag == NORMAL)
			{
				std::string direction;
				char statement[MAX_PATH];
				if (it->first.direction == SELL)
				{
					direction = "sell";
				}
				else if (it->first.direction == BUY)
				{
					direction = "buy";
				}
				sprintf_s(statement, ARRAYCOUNT(statement), "UPDATE %s SET profit = %.2f, margin = %.2f, average_price = %.3f, volume = %d WHERE contract = '%s' AND direction = '%s';", table_name, it->second.profit, it->second.margin, it->second.average_price, it->second.volume, it->first.contract.c_str(), direction.c_str());
				(*handler)(db->Update(statement), page_name);
			}
		}
	}
};