#pragma once
#include"DataBind.h"
#include"TradeDataType.h"
//UI synchronized order list
#pragma region  data struct for order
class OrderCredential
{
private:
public:
	std::string contract;
	int front_id;//期货专有，前置编号
	int session_id;//期货专有，会话编号
	std::string order_ref;//期货专有，报单引用
	std::string exchange;
	std::string order_sys_id;//期货指订单在交易所的编号，股票指订单编号
	bool operator==(const OrderCredential &oc) { return front_id == oc.front_id && session_id == oc.session_id && order_ref == oc.order_ref; }
};

bool operator==(const OrderCredential &oc1, const OrderCredential &oc2);

namespace std {
	template <>
	struct hash<OrderCredential>
	{
		std::size_t operator()(const OrderCredential& oc) const
		{
			std::size_t hash1 = std::hash<int>()(oc.front_id);
			std::size_t hash2 = std::hash<int>()(oc.session_id);
			std::size_t hash3 = std::hash<std::string>()(oc.order_ref);
			return ((hash1 ^ (hash2 << 1)) >> 1) ^ (hash3 << 1);
		}
	};
}

class OrderDetailSync :
	public DataBind
{
private:
	int row_index;
public:
	Direction direction;
	OpenFlag open_flag;//开平标志
	std::string status;//挂单状态
	int volume;//报单手数
	int trade_volume;//成交手数
	double price;//报单价格
	std::string insert_time;//报单时间
	ExpireFlag expire_flag;
	_OrderID order_id;
	const OrderCredential* oc;
	void UpdateContract();//update control only
	void UpdateDirection();
	void UpdateOpenFlag();
	void UpdateStatus();
	void UpdateStatus(const char* _status);//update control and its internal corresponding member
	void UpdateStatus(const std::string &_status);
	void UpdateVolume();
	void UpdateVolume(int _volume);
	void UpdatePrice();
	void UpdatePrice(double _price);
	void UpdateInsertTime();
	void UpdateInsertTime(const std::string &_insert_time);
	void UpdateTradeVolume();
	void UpdateTradeVolume(int _trade_volume);
	void UpdateExchange();
	void UpdateFrontID();
	void UpdateSessionID();
	void UpdateOrderRef();
	int AddDataGridRow();
};

typedef std::unordered_map<OrderCredential, OrderDetailSync> OrderListSync;
#pragma endregion
