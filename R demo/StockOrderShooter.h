#pragma once
#include"Order.h"
#include"Utilities.h"

typedef bool(__stdcall* LoginDelegate)(char* IP, int Port, char* User, char* PassWord, char* TXPass, char* Yyb);//登陆
typedef char*(__stdcall* QueryDataDelegate)(char* User, int Category);//查询
typedef char*(__stdcall* CancelOrderDelegate)(char* User, char* Bh);//撤单
typedef char*(__stdcall* GetPriceDelegate)(char* Gpdm);//获取五档报价
typedef char*(__stdcall* SendOrderDelegate)(int Fx, char* User, char* Gddm, char* Gpdm, int Quantity, float Price);//下单
typedef std::string OrderTime;
typedef enum
{
	EXECUTE_BUY_PLAN,
	FIGHT_UNTIL_WIN
}Task;
typedef enum
{
	EXECUTED,
	NOT_EXECUTED

}ExecuteFlag;
struct CompareByValue
{
	bool operator()(const std::tuple<Price, OrderVolume, TradeVolume, _OrderID> &a, const std::tuple<Price, OrderVolume, TradeVolume, _OrderID> &b)
	{
		return std::get<0>(a) > std::get<0>(b);
	}

	bool operator()(const OrderDetailSync &a, const OrderDetailSync &b)
	{
		return a.price > b.price;
	}

	bool operator()(const PositionDetailSync &a, const PositionDetailSync &b)
	{
		return a.trade_date < b.trade_date;
	}
};
class StockOrderShooter:
	public Order
{
private:
	HMODULE hmodule = LoadLibraryA("JLAPI.dll");
	char return_char_arr[1024 * 10];
	std::string return_string;
	std::vector<std::string> tokens;
	FILE* fp;
	bool is_order_placed;//订单是否被成功提交
	bool all_positions_current = false;//所有持仓是否全为今仓
	OrderDetailSync od;
	PositionDetailSync pd;
	int per_share;//每份买入的份额
private:
	void GenerateRandomTradeTime(std::string &trade_time);
	void GenerateRandomTradeTimeUnique(std::string &trade_time);
	int MakeVolume(double capital, double price);
#ifndef _DEBUG
	void MakeOrderPlan(double half_capital);//生成一个交易日资金拆单入场计划
#endif // !_DEBUG
	void PlaceLadderSellOrders(bool first_run);//根据拆分的服务器持仓下阶梯卖单
	void PlaceLadderBuyOrders(bool first_run);//下阶梯买单
	void SplitPositions(std::string date = "");//用于将服务器持仓拆分成阶梯持仓
	void ModifyLocalPositionBasedOnServer();
	void PrintExecuteOrderPlan();
	void PrintFightUntilWin();

public:
	LoginDelegate Login = (LoginDelegate)GetProcAddress(hmodule, "JL_Login");
	QueryDataDelegate QueryData = (QueryDataDelegate)GetProcAddress(hmodule, "JL_QueryData");
	CancelOrderDelegate CancelOrder = (CancelOrderDelegate)GetProcAddress(hmodule, "JL_CancelOrder");
	GetPriceDelegate GetPrice = (GetPriceDelegate)GetProcAddress(hmodule, "JL_GetPrice");
	SendOrderDelegate SendOrder = (SendOrderDelegate)GetProcAddress(hmodule, "JL_SendOrder");
	char stock_code[7];
	double total_capital;//当前总资金
	double profit_rate;//目标盈利率
	double init_capital;//初始投入资金
	double available;//可用资金
	double last_price;//最新价
	double cost_price;//成本价
	double yest_close_price;//昨收盘价
	double upper_limit_price;//涨停板价
	double lower_limit_price;//跌停板价
	double price_cursor = 0.0;//价格游标
	unsigned int volume;//t+1可卖数量
	int position_local = 0;//本地总持仓
	int position_server = 0;//服务器总持仓
	double price_step;//当日价格步长
	std::multiset<PositionDetailSync, CompareByValue> position_list;//仓量，成交日期,报单编号组合,1.每一张卖单要和每一份持仓对应2.当成交日期为当日时，报单编号为空
	std::set<std::tuple<Price, OrderVolume, TradeVolume, _OrderID>, CompareByValue> order_list_buy_plan;//买单列表：阶梯价格，报单量，成交量，报单编号组合
	std::map<Price, OrderDetailSync> order_list_fight;//订单信息只用到以下字段：阶梯价格，方向，报单量，成交量，报单编号
	std::map<OrderTime, Capital> buy_plan;//买单计划：下单时间，下单量
	std::map<_OrderID, std::pair<Price, ExecuteFlag>> orderid_price_map;//报单编号，价格阶梯线map
	std::map<_OrderID, TradeTime> orderid_tradetime_map;//当日成交的订单的委托编号和最后一笔成交时间的组合集
	Task task;
	bool init = false;
	std::string curr_time;//当前时间字符串
	std::string curr_date = DateTime::GetCurrentDateString();//当前日期字符串//TODO:curr_date应该用完整的日期+时间，否则跨年的时候会产生bug
public:
	StockOrderShooter(char* _stock_code, double _init_capital, double _profit_rate);
	virtual ~StockOrderShooter();
	void ConnectOrderServer();
	void Shoot(char* contract, double price, Direction direction, OpenFlag open_flag, int volume, OrderBroker* order_broker, InvestAccountSync* invest_account);
	void SellAll(double price = 0.0);
	void BuyHalf();
	void Cancel(OrderBroker* order_broker, InvestAccountSync* invest_account, OrderCredential* oc);
	void RefreshAccount();//查询资金
	void RefreshLastPrice();//查询五档报价
	void RefreshVolume();//查询股份
	double RefreshOrders();//查询当日委托
	void RefreshTradedOrders();//查询当日成交
	void ComparePositionBetweenServerLocal();//首次启动从数据库获取持仓队列后，该函数从服务器获取持仓队列，对比修正
#ifdef _DEBUG
	void MakeOrderPlan(double half_capital);//生成一个交易日资金拆单入场计划
#endif // !_DEBUG
	void ExecuteOrderPlan();
	void FightUntilWin();
	void Print();//打印当前总资本，目标资本，任务路由，持仓列表，订单列表，价格游标
};
