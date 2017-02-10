#pragma once
#include"FutureOrderModel.h"
#include"SQLite.h"
#include"FutureOrderManager.h"

void FutureOrderModel::CreateTable(SQLite* db, const char* table_name, DatabaseErrorMessageHandler handler)
{
	db->AddFieldNotNull("contract", TEXT);
	db->AddFieldNotNull("direction", TEXT);
	db->AddFieldNotNull("open_flag", TEXT);
	db->AddFieldNotNull("status", TEXT);
	db->AddFieldNotNull("volume", INTEGER);
	db->AddFieldNotNull("price", _DOUBLE);
	db->AddFieldNotNull("insert_time", TEXT);
	db->AddFieldNotNull("trade_volume", INTEGER);
	db->AddFieldNotNull("exchange", TEXT);
	db->AddFieldForInsert("front_id", INTEGER);
	db->AddFieldForInsert("session_id", INTEGER);
	db->AddFieldForInsert("order_ref", TEXT);
	(*handler)(db->CreateTable(table_name));
}

void FutureOrderModel::Retrieve(SQLite* db, const char* table_name, OrderListSync* order_list, AccountSpecificMsgHandler handler, const char* page_name)
{
	std::vector<std::vector<std::string>> result_set;
	std::string statement = "SELECT * FROM " + std::string(table_name);
	if (!(*handler)(db->ExecuteSelect(statement.c_str(), &result_set, 12), page_name))
	{
		if (!result_set.empty())
		{
			order_list->clear();
			OrderCredential oc;
			OrderDetailSync od; 
			od.oc = &oc;
			OrderListSync::iterator it2;
			for (auto it = result_set.begin(); it != result_set.end(); ++it)
			{
				oc.contract = (*it)[0];
				od.UpdateContract();

				if ((*it)[1] == "sell")
				{
					od.direction = SELL;
				}
				else if ((*it)[1] == "buy")
				{
					od.direction = BUY;
				}
				od.UpdateDirection();

				if ((*it)[2] == "open")
				{
					od.open_flag = OPEN;
				}
				else if ((*it)[2] == "close")
				{
					od.open_flag = CLOSE;
				}
				else if ((*it)[2] == "force_close")
				{
					od.open_flag = FORCE_CLOSE;
				}
				else if ((*it)[2] == "force_off")
				{
					od.open_flag = FORCE_OFF;
				}
				else if ((*it)[2] == "local_force_close")
				{
					od.open_flag = LOCAL_FORCE_CLOSE;
				}
				od.UpdateOpenFlag();
				od.UpdateStatus((*it)[3]);
				od.UpdateVolume(atoi((*it)[4].c_str()));
				od.UpdatePrice(atof((*it)[5].c_str()));
				od.UpdateInsertTime((*it)[6]);
				od.UpdateTradeVolume(atoi((*it)[7].c_str()));

				oc.exchange = (*it)[8];
				od.UpdateExchange();

				oc.front_id = atoi((*it)[9].c_str());
				od.UpdateFrontID();

				oc.session_id = atoi((*it)[10].c_str());
				od.UpdateSessionID();

				oc.order_ref = (*it)[11].c_str();
				od.UpdateOrderRef();

				(*order_list)[oc] = od;
				it2 = order_list->find(oc);
				it2->second.oc = &(it2->first)
			}
		}
	}
}

void FutureOrderModel::Update(SQLite* db, const char* table_name, OrderListSync* order_list, AccountSpecificMsgHandler handler, const char* page_name)
{
	char statement[MAX_PATH];
	for (auto it = order_list->begin(); it != order_list->end(); ++it)
	{
		if (FutureOrderManager::IsOrderExpire(it->second.insert_time))
		{
			//delete expire orders from database
			sprintf_s(statement, ARRAYCOUNT(statement), "DELETE FROM %s WHERE front_id = %d AND session_id = %d AND order_ref = '%s';", table_name, it->first.front_id, it->first.session_id, it->first.order_ref.c_str());
			(*handler)(db->Delete(statement), page_name);
		}
		else if(it->second.expire_flag == NEW)
		{
			//save new orders to local database
			if (it->second.direction == SELL)
			{
				db->AddFieldForInsert("direction", "sell");
			}
			else if (it->second.direction == BUY)
			{
				db->AddFieldForInsert("direction", "buy");
			}

			if (it->second.open_flag == OPEN)
			{
				db->AddFieldForInsert("open_flag", "open");
			}
			else if (it->second.open_flag == CLOSE)
			{
				db->AddFieldForInsert("open_flag", "close");
			}
			else if (it->second.open_flag == FORCE_CLOSE)
			{
				db->AddFieldForInsert("open_flag", "force_close");
			}
			else if (it->second.open_flag == FORCE_OFF)
			{
				db->AddFieldForInsert("open_flag", "force_off");
			}
			else if (it->second.open_flag == LOCAL_FORCE_CLOSE)
			{
				db->AddFieldForInsert("open_flag", "local_force_close");
			}
			db->AddFieldForInsert("status", it->second.status.c_str());
			db->AddFieldForInsert("volume", it->second.volume);
			db->AddFieldForInsert("price", it->second.price);
			db->AddFieldForInsert("insert_time", it->second.insert_time.c_str());
			db->AddFieldForInsert("trade_volume", it->second.trade_volume);
			db->AddFieldForInsert("exchange", it->first.exchange.c_str());
			db->AddFieldForInsert("contract", it->first.contract.c_str());
			db->AddFieldForInsert("front_id", it->first.front_id);
			db->AddFieldForInsert("session_id", it->first.session_id);
			db->AddFieldForInsert("order_ref", it->first.order_ref.c_str());

			(*handler)(db->Insert(table_name), page_name);
		}
		else
		{
			//update order list to database
			sprintf_s(statement, ARRAYCOUNT(statement), "UPDATE %s SET status = '%s', trade_volume = %d WHERE front_id = %d AND session_id = %d AND order_ref = '%s';", table_name, it->second.status.c_str(), it->second.trade_volume, it->first.front_id, it->first.session_id, it->first.order_ref.c_str());
			(*handler)(db->Update(statement), page_name);
		}
	}
}