#pragma once
#include"DataBind.h"
#include"OrderListSync.h"
#include"PositionListSync.h"

//UI synchronized invest account list
class CThostFtdcTraderApi;
class FutureOrderProcessor;
class InvestAccountSync :
	public DataBind
{
public:
	std::string investor_id;
	std::string investor_password;
	double balance;//在期货中指动态权益
	double position_profit;//持仓盈亏
	double close_profit;//平仓盈亏
	double frozen;//下单冻结
	double available;//可用资金
	OrderListSync _order_list;
	PositionListSync position_list;
	std::string bound_bank_name;//三方存管银行名称
	std::string bound_bank_account;//三方存管银行账户
	double bound_bank_balance;//三方存管银行活期余额
//TODO:please solve the problem:close the app in initialization phase will crash
//TODO:use file system to replace database
	/*future only*/int order_ref;//期货专有，报单引用
	/*future only*/double margin;//期货专有，占用保证金
	/*future only*/std::string user_id;//期货专有
	//*future only*//double prev_balance;//期货专有，静态权益
	/*future only*/int ctp_order_request_id;//期货专有
	/*future only*/int ctp_quote_request_id;//期货专有
	/*future only*/std::pair<CThostFtdcTraderApi*, FutureOrderProcessor* > orderapi_spi_pair;//期货专有，交易实例
	void UpdateAccountInfo();

};



