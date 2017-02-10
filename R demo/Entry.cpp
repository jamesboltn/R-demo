#define R_DEMO
//#define CONSOLE1
//#define CONSOLE2
//#define _FIGHT_UNTIL_WIN
//#define SELL_MACHINE
//#define BUY_MACHINE



#if defined R_DEMO
#pragma comment( linker, "/subsystem:windows" ) 
#pragma once
#include"Utilities.h"
#include"WndMain.h"
#include"WndTest.h"
#define COMMERCIAL
#undef COMMERCIAL

OS::InstanceLimit lmt(1);
int _stdcall wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (lmt.IsExceedLimit())
	{
		::MessageBox(NULL, L"instances of R exceed limit", L"", 0);
		return 0;
	}

#ifdef COMMERCIAL
#else
	WndTest wnd_test(L"R", NULL, 219, 403, DIALOG_UNIT);
	wnd_test.SetAsMainWindow();
	//WndMain wnd_main(L"R", NULL, 219, 403, DIALOG_UNIT);
	//wnd_main.SetAsMainWindow();
#endif
	return Window::PumpMessages();
}
#elif defined CONSOLE1
#pragma comment( linker, "/subsystem:console" )
#include"Utilities.h"
int main()
{	
	std::string source = "|1||2|3|4|";
	std::vector<std::string> tokens;
	String::Split(&source, '|', &tokens);
	getchar();
	return 0;
}
#elif defined CONSOLE2
#pragma comment( linker, "/subsystem:console" )
#include"ClientSocket.h"
#include<Windows.h>
int main()
{
	ClientSocket cli_socket;
	if (cli_socket.Connect("192.168.1.100", 4962))
	{
		strcpy(cli_socket.recv_buffer, "hello");
		cli_socket.SendStringData(cli_socket.recv_buffer);
		printf("sent to server: hello\n");
	}
	while (1)
	{
		if (cli_socket.ReceiveStringData(cli_socket.recv_buffer, ARRAYCOUNT(cli_socket.recv_buffer)))
		{
			printf("received from server: %s\n", cli_socket.recv_buffer);
		}
		else
		{
			printf("connection reset by peer or server aborted\n");
			break;
		}
	}
	getchar();
	return 0;
}
#elif defined _FIGHT_UNTIL_WIN
#pragma comment( linker, "/subsystem:console" )
#include"preinclude.h"
#include"StockOrderShooter.h"
#include"SQLite.h"
#include"Utilities.h"
#include"StockPositionModel.h"

void CALLBACK FightUntilWin(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
}
int main()
{
	StockOrderShooter order_shooter("300550", 206850.81, 3.0 / 100.0);
	// 从配置文件获取价格游标
	if (!PathFileExists(L"config.ini"))
	{
		FileIO::SetIniValue(L"fields", L"price_cursor", L"0.0");
	}
	else
	{
		wchar_t buffer[8];
		FileIO::GetIniValue(L"fields", L"price_cursor", buffer, ARRAYCOUNT(buffer));
		order_shooter.price_cursor = _wtof(buffer);
	}
	//database init
	SQLite db;
	db.Connect();
	//create tables
	if (!db.IsTableExist("position_GuoJinSecurities_39011807"))
	{
		StockPositionModel::CreateTable(&db, "position_GuoJinSecurities_39011807");
	}
	//retrieve position list from database
	StockPositionModel::Retrieve(&db, "position_GuoJinSecurities_39011807", &order_shooter.position_list);
	order_shooter.ConnectOrderServer();

	//计算本地总持仓
	for (auto it = order_shooter.position_list.begin(); it != order_shooter.position_list.end(); ++it)
	{
		order_shooter.position_local += it->volume;
	}

	order_shooter.RefreshAccount();//查询资金
	order_shooter.RefreshVolume();//查询股份
	order_shooter.RefreshLastPrice();//查询五档报价
	order_shooter.ComparePositionBetweenServerLocal();
	order_shooter.init = true;

	::SetTimer(NULL, 0, 1000 * 5, (TIMERPROC)&FightUntilWin);
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	while (::GetMessage(&msg, NULL, NULL, NULL))
	{
		switch (msg.message)
		{
		case WM_TIMER:
		{
			order_shooter.curr_time = DateTime::GetCurrentTimeString();
			printf("%s timer invoked\n", order_shooter.curr_time.c_str());
			if (order_shooter.curr_time > "11:30:00" && order_shooter.curr_time <= "12:59:59")
			{
				printf("no business time at noon\n");
				continue;
			}
			if (order_shooter.curr_time > "15:00:00")
			{
				//收盘后退出
				StockPositionModel::Update(&db, "position_GuoJinSecurities_39011807", &order_shooter.position_list); //保存持仓列表
				wchar_t buffer[8];
				swprintf(buffer, L"%.2f", order_shooter.price_cursor);//保存价格游标
				FileIO::SetIniValue(L"fields", L"price_cursor", buffer);
				return 0;
			}
			
			order_shooter.RefreshLastPrice();//查询五档报价
			order_shooter.RefreshAccount();//查询资金
			if (order_shooter.task == EXECUTE_BUY_PLAN)
			{
				order_shooter.ExecuteOrderPlan();//TODO:后续需要加n秒后不能成交，撤单后重下的逻辑
			}
			else if(order_shooter.task == FIGHT_UNTIL_WIN)
			{
				order_shooter.FightUntilWin();
			}
			order_shooter.Print();
		}
		break;
		}
	}
	return 0;
}
#elif defined SELL_MACHINE
#pragma comment( linker, "/subsystem:console" )
#include"preinclude.h"
#include"StockOrderShooter.h"
#include"SQLite.h"
#include"Utilities.h"
#include"StockPositionModel.h"
#include"MathUtils.h"
void CALLBACK FightUntilWin(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
}
int main()
{
	StockOrderShooter order_shooter("300550", 206850.81, 3.0 / 100.0);
	order_shooter.ConnectOrderServer();
	order_shooter.RefreshVolume();//更新可卖数量
	order_shooter.RefreshOrders();//更新当日委托
	order_shooter.init = true;
	::SetTimer(NULL, 0, 1000 * 3, (TIMERPROC)&FightUntilWin);
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	std::string curr_time;
	bool test = true;
	while (::GetMessage(&msg, NULL, NULL, NULL))
	{
		switch (msg.message)
		{
		case WM_TIMER:
		{
			curr_time = DateTime::GetCurrentTimeString();
			printf("%s timer invoked\n", curr_time.c_str());
			if (curr_time > "11:30:00" && curr_time <= "12:59:59")
			{
				printf("non business time at noon\n");
				continue;
			}
			if (curr_time > "15:00:00")
			{
#ifndef _DEBUG
				return 0;
#endif
			}

			order_shooter.RefreshLastPrice();
			printf("last price %.2f\n", order_shooter.last_price);
			order_shooter.RefreshAccount();
			printf("total capital %.2f\n", order_shooter.total_capital);
			printf("available sell volume %d\n", order_shooter.volume);
			if (test)
			{
				//sell all
				if (order_shooter.pending_orders == 0)
				{
					order_shooter.SellAll(101.86);
				}
				test = false;
			}
		}
		break;
		}
	}
	return 0;
}

#elif defined BUY_MACHINE
#pragma comment( linker, "/subsystem:console" )
#include"preinclude.h"
#include"StockOrderShooter.h"
#include"SQLite.h"
#include"Utilities.h"
#include"StockPositionModel.h"

void CALLBACK FightUntilWin(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{

}
int main()
{
	StockOrderShooter order_shooter("300550", 206866.1, 3.0 / 100.0);
	order_shooter.ConnectOrderServer();
	::SetTimer(NULL, 0, 1000 * 5, (TIMERPROC)&FightUntilWin);
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	bool test = true;
	std::string curr_time;
	while (::GetMessage(&msg, NULL, NULL, NULL))
	{
		switch (msg.message)
		{
		case WM_TIMER:
		{
			curr_time = DateTime::GetCurrentTimeString();
			printf("%s timer invoked\n", curr_time.c_str());
			if (curr_time > "11:30:00" && curr_time <= "12:59:59")
			{
				printf("non business time at noon\n");
				continue;
			}
			if (curr_time > "15:00:00")
			{
#ifndef _DEBUG
				return 0;
#endif
			}

			order_shooter.RefreshLastPrice();
			printf("last price %.2f\n", order_shooter.last_price);
			order_shooter.RefreshAccount();
			printf("total capital %.2f\n", order_shooter.total_capital);

			if (test)
			{
				//buy half
				order_shooter.BuyHalf();
				test = false;
			}
		}
		break;
		}
	}
	return 0;
}
#endif
