#pragma once
#include"FutureContractDictionaryModel.h"
#include"SQLite.h"
#include"Utilities.h"

void FutureContractDictionaryModel::Retrieve(SQLite* db, ContractDictionary* contract_dic, DatabaseErrorMessageHandler handler)
{
	int current_date_int = DateTime::GetCurrentDateInt();
	std::vector<std::vector<std::string>> result_set;
	if (!(*handler)(db->ExecuteSelect("SELECT * FROM contract_dictionary", &result_set, 11)))
	{
		if (!result_set.empty())
		{
			ContractCredential contract_credential;
			ContractID contract_id;
			for (auto it = result_set.begin(); it != result_set.end(); ++it)
			{
				contract_id.contract = (*it)[0];
				contract_credential.exchange = (*it)[1];
				contract_credential.deliver_year = atoi((*it)[2].c_str());
				contract_credential.deliver_month = atoi((*it)[3].c_str());
				contract_credential.volume_multiple = atoi((*it)[4].c_str());
				contract_credential.price_tick = atof((*it)[5].c_str());

				contract_credential.expire_date = (*it)[6];
				contract_credential.start_deliver_date = (*it)[7];
				contract_credential.end_deliver_date = (*it)[8];
				contract_credential.buy_margin_ratio = atof((*it)[9].c_str());
				contract_credential.sell_margin_ratio = atof((*it)[10].c_str());

				//将数据库里过期的合约缓存，以便后续从数据库里删除
				if (current_date_int >= atoi(contract_credential.expire_date.c_str()))
				{
					contract_id.expire_flag = EXPIRE;
					(*contract_dic)[contract_id] = contract_credential;
				}
				else
				{
					contract_id.expire_flag = NORMAL;
					(*contract_dic)[contract_id] = contract_credential;
				}
			}
		}
	}

}

void FutureContractDictionaryModel::Delete(SQLite* db, ContractDictionary* contract_dic, DatabaseErrorMessageHandler handler)
{
	if (!contract_dic->empty())
	{
		char statement[MAX_PATH];
		for (auto it = contract_dic->begin(); it != contract_dic->end(); ++it)
		{
			if (it->first.expire_flag == EXPIRE)
			{
				sprintf_s(statement, ARRAYCOUNT(statement), "DELETE FROM contract_dictionary WHERE contract_id = '%s';", it->first.contract.c_str());
				(*handler)(db->Delete(statement));
			}
		}
	}
}

void FutureContractDictionaryModel::Insert(SQLite* db, ContractDictionary* contract_dic, DatabaseErrorMessageHandler handler)
{
	if (!contract_dic->empty())
	{
		for (auto it = contract_dic->begin(); it != contract_dic->end(); ++it)
		{
			if (it->first.expire_flag == NEW)
			{
				db->AddFieldForInsert("contract_id", it->first.contract.c_str());
				db->AddFieldForInsert("exchange", it->second.exchange.c_str());
				db->AddFieldForInsert("deliver_year", it->second.deliver_year);
				db->AddFieldForInsert("deliver_month", it->second.deliver_month);
				db->AddFieldForInsert("volume_multiple", it->second.volume_multiple);
				db->AddFieldForInsert("price_tick", it->second.price_tick);
				db->AddFieldForInsert("expire_date", it->second.expire_date.c_str());
				db->AddFieldForInsert("start_deliver_date", it->second.start_deliver_date.c_str());
				db->AddFieldForInsert("end_deliver_date", it->second.end_deliver_date.c_str());
				db->AddFieldForInsert("buy_margin_ratio", it->second.buy_margin_ratio);
				db->AddFieldForInsert("sell_margin_ratio", it->second.sell_margin_ratio);
				(*handler)(db->Insert("contract_dictionary"));
			}
		}
	}
}
