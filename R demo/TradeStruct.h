#pragma once
#include"OrderListSync.h"
#include"PositionListSync.h"
#include"InvestAccountSync.h"
#include"SocketBase.h"

#pragma region struct for account
typedef InvestAccountSync QuoteAccountSync;
typedef std::vector<QuoteAccountSync> QuoteAccountList;
typedef std::string AccountID, BrokerUniqueName;//the BrokerUniqueName is a broker_name+broker_id combination
typedef std::unordered_map<AccountID, InvestAccountSync> InvestAccountList;

typedef struct
{
	std::string broker_name;
	std::string broker_id;
	AccountType account_type;
	InvestAccountList invest_account_list;
	std::vector<std::pair<OrderFront, TCPPortConnectionState>> order_front_list;
}OrderBroker;
typedef std::vector<OrderBroker> OrderBrokerList;
typedef std::unordered_map<BrokerUniqueName, OrderBroker> OrderBrokerLst;

typedef struct
{
	std::string broker_name;
	std::string broker_id;
	QuoteAccountList quote_account_list;//if one or more of the quote fronts are connectable,it is 'CONNECTABLE', otherwise 'UNCONNECTABLE'
	std::pair<std::vector<std::pair<QuoteFront, TCPPortConnectionState>>, TCPPortConnectionState> quote_front;
}QuoteBroker;
typedef std::vector<QuoteBroker> QuoteBrokerList;
#pragma endregion

#pragma region struct for contract
struct ContractID
{
	std::string contract;
	ExpireFlag expire_flag;
	bool operator==(const ContractID &id) { return contract == id.contract && expire_flag == id.expire_flag; }
};

bool operator==(const ContractID &id1, const ContractID &id2);

namespace std {
	template <>
	struct hash<ContractID>
	{
		std::size_t operator()(const ContractID& id) const
		{
			std::size_t hash1 = std::hash<std::string>()(id.contract);
			std::size_t hash2 = std::hash<int>()(id.expire_flag);
			return (hash1 ^ (hash2 << 1)) >> 1;
		}
	};
}

typedef struct
{
	std::string exchange;//交易所代码
	double price_tick;//合约(股票)最小变动价位
	double open_interest;//在期货中指持仓量，在股票中指流通股数 
	/*future only*/int volume_multiple;//期货专有，合约乘数，股票始终为1
	/*future only*/int deliver_year;//期货专有，交割年份
	/*future only*/int deliver_month;//期货专有，交割月份
	/*future only*/std::string expire_date;//期货专有，到期日
	/*future only*/std::string start_deliver_date;//期货专有，开始交割日
	/*future only*/std::string end_deliver_date;//期货专有，结束交割日 
	/*future only*/double buy_margin_ratio;//期货专有，买方保证金率
	/*future only*/double sell_margin_ratio;//期货专有，卖方保证金率
}ContractCredential;
typedef std::unordered_map<ContractID, ContractCredential> ContractDictionary;
 
ContractDictionary::iterator FindContract(const std::string &contract, ContractDictionary* contract_dic);
ContractDictionary::iterator FindContractFromNormalNew(const std::string &contract, ContractDictionary* contract_dic);
ContractDictionary::iterator FindContractFromNormal(const std::string &contract, ContractDictionary* contract_dic);
ContractDictionary::iterator FindContractFromNew(const std::string &contract, ContractDictionary* contract_dic);
#pragma endregion