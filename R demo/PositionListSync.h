#pragma once
#include"DataBind.h"
#include"TradeDataType.h"
//UI synchronized position list
#pragma region struct for position
struct PositionCredential
{
	std::string contract;
	Direction direction;
	ExpireFlag expire_flag;
	bool operator==(const PositionCredential &pc) { return contract == pc.contract && direction == pc.direction && expire_flag == pc.expire_flag; }
	PositionCredential &operator=(const PositionCredential &right);
};
bool operator==(const PositionCredential &pc1, const PositionCredential &pc2);
namespace std {
	template <>
	struct hash<PositionCredential>
	{
		std::size_t operator()(const PositionCredential& id) const
		{
			std::size_t hash1 = std::hash<std::string>()(id.contract);
			std::size_t hash2 = std::hash<int>()(id.direction);
			std::size_t hash3 = std::hash<int>()(id.expire_flag);
			return ((hash1 ^ (hash2 << 1)) >> 1) ^ (hash3 << 1);
		}
	};
}

class PositionDetailSync:
	public DataBind
{
private: 
	int row_index;
public:
	double profit;
	double average_price;//持仓均价
	int volume;//期货是手数，在期货里的意思是总仓
	std::string exchange;
	bool is_server_contained;//持仓是否能被服务器查询到
	const PositionCredential* pc;
	/*stock only*/TradeDate trade_date;//股票专有，成交日期
	/*stock only*/_OrderID sell_order_id;//股票专有，持仓对应的卖单报单编号
	/*future only*/int historical_volume;//期货专有，历史仓位
	/*future only*/double margin;//期货专有，占用保证金	//TODO:后面还是要想办法用ifdef来控制
	/*future only*/std::vector<std::pair<Volume, OpenPrice>> sub_positions;
	/*future only*/void UpdateMargin();
	/*future only*/void UpdateMargin(double _margin);
	PositionDetailSync();
	PositionDetailSync(const PositionCredential* _pc);
	void UpdateCurrentPrice(double price);
	void UpdateProfit(double _profit);//update control and its internal corresponding member
	void UpdateProfit();//update control only
	void UpdateAveragePrice();
	void UpdateAveragePrice(double _average_price);
	void UpdateVolume();
	void UpdateVolume(int _volume);
	void UpdateDirection();
	void UpdateContract();
	/*void UpdateContract(const char* contract);*/
	int AddDataGridRow();
	void DeleteDataGridRow();
	inline int GetRowIndex() { return row_index; }
	inline void DecrementRowIndex() { --row_index; }
};
typedef std::unordered_map<PositionCredential, PositionDetailSync> PositionListSync;
PositionListSync::iterator FindPositionFromSellBuy(const std::string &contract, PositionListSync* p);
PositionListSync::iterator FindPositionFromSell(const std::string &contract, PositionListSync* p);
PositionListSync::iterator FindPositionFromBuy(const std::string &contract, PositionListSync* p);
PositionListSync::iterator FindPositionFromNormalNew(PositionCredential* pc, PositionListSync* p);
#pragma endregion

