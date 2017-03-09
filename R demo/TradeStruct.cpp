#include"TradeStruct.h"

bool operator==(const ContractID &id1, const ContractID &id2)
{
	return id1.contract == id2.contract &&
		id1.expire_flag == id2.expire_flag;
}

ContractDictionary::iterator FindContract(const std::string &contract, ContractDictionary* contract_dic)
{
	ContractID contract_id;
	ContractDictionary::iterator it;

	contract_id.contract = contract;
	contract_id.expire_flag = NEW;
	it = contract_dic->find(contract_id);
	if (it == contract_dic->end())
	{
		contract_id.expire_flag = NORMAL;
		it = contract_dic->find(contract_id);
		if (it == contract_dic->end())
		{
			contract_id.expire_flag = EXPIRE;
			return contract_dic->find(contract_id);
		}
		else
		{
			return it;
		}
	}
	else
	{
		return it;
	}
}

ContractDictionary::iterator FindContractFromNormalNew(const std::string &contract, ContractDictionary* contract_dic)
{
	ContractID contract_id;
	ContractDictionary::iterator it;

	contract_id.contract = contract;
	contract_id.expire_flag = NEW;
	it = contract_dic->find(contract_id);
	if (it == contract_dic->end())
	{
		contract_id.expire_flag = NORMAL;
		return contract_dic->find(contract_id);
	}
	else
	{
		return it;
	}
}

ContractDictionary::iterator FindContractFromNormal(const std::string &contract, ContractDictionary* contract_dic)
{
	ContractID contract_id;
	contract_id.contract = contract;
	contract_id.expire_flag = NORMAL;
	return contract_dic->find(contract_id);
}

ContractDictionary::iterator FindContractFromNew(const std::string &contract, ContractDictionary* contract_dic)
{
	ContractID contract_id;
	contract_id.contract = contract;
	contract_id.expire_flag = NEW;
	return contract_dic->find(contract_id);
}


