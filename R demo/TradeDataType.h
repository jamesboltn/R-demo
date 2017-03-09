#pragma once
#include"preinclude.h"

const std::string trade_first_start_time = "08:50:00";
const std::string trade_first_end_time = "16:00:00";
const std::string trade_second_start_time = "20:50:00";
const std::string trade_second_end_time = "02:40:00";

//定义买卖方向
typedef enum
{
	BUY,
	SELL,
	UNKNOWN
}Direction;

//定义开平方向
typedef enum
{
	OPEN,
	CLOSE,
	/*future only*/FORCE_CLOSE,//期货专有，强平
	/*future only*/FORCE_OFF,//期货专有，强减
	/*future only*/LOCAL_FORCE_CLOSE//期货专有，本地强平
}OpenFlag;

//定义期货投机套保标识
typedef enum
{
	SPECULATION,//投机
	ARBITRAGE,//套利
	HEDGE//套保
}HedgeFlag;//套保标志

//定义合约过期标志
typedef enum
{
	NORMAL,//数据库取且交易所未过期
	EXPIRE,//数据库取且交易所过期
	NEW//订阅时新增且交易所未过期
}ExpireFlag;

//定义账户类型
typedef enum
{
	REAL,//实盘账户
	SIMULATE//模拟账户
}AccountType;

//定义订阅标识
typedef enum
{
	SUBSCRIBE,//订阅标识
	UNSUBSCRIBE,//取消订阅标识
} SubscribeFlag;

typedef std::string OrderFront, QuoteFront, Front, TradeID, TradeTime, TradeDate, _OrderID/*股票专用，委托字符串*/;
typedef int Volume, OrderVolume, TradeVolume;
typedef double OpenPrice, Margin, Price, Capital;

typedef void(*AccountSpecificMessageHandler)(const char* msg, const char* table_name, const char* log_path);
typedef void(*GeneralMessageHandler)(const char* msg);




