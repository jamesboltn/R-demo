#pragma once
#include"WndMain.h"
#include"Utilities.h"
#include"FutureOrderProcessor.h"
#include"FutureOrderShooter.h"
#include"Static.h"
#include"DataGrid.h"
#include"TreeView.h"
#include"WndChooseAccount.h"
#include"ClientDataType.h"
#include"FutureContractDictionaryModel.h"
#include"FutureOrderModel.h"
#include"FutureCredentialModel.h"
#include"FuturePositionModel.h"
#include"FutureQuoteFeedForTrade.h"
#include"GlobalObject.h"

time_t now;
time_t reboot_entry;
EditWithLog* local_edit_app_log;

void CALLBACK TimerRebootAtSpecificTime(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	now = ::time(0);
	if (::difftime(now, reboot_entry) > 0.0)
	{
#ifdef _DEBUG
#else
		OS::Reboot();
		std::string msg("will reboot at " + DateTime::GetSpecificDateTimeString(&reboot_entry));
		WndMain::local_wnd_main->edit_app_log.AppendLineA(msg.c_str());
#endif
	}
}
DWORD WINAPI ThreadMessageLoop(LPVOID lpParam);
DWORD WINAPI ThreadTradeInit(LPVOID lpParam)
{
 	WndMain* wnd_main = (WndMain*)lpParam;
	wnd_main->CheckBrokerConnection(&Global::memory.broker_info_list);
	//instantiating future_quote_feed
	Global::memory.quote_feed_trade = new FutureQuoteFeedForTrade(&Global::memory.broker_info_list, &wnd_main->refresh_ui_thread_id, &Global::event, WndMain::FutureQuoteHandler);
	Global::event = ::CreateEvent(NULL, false, false, L"OnRspQuoteLogin");
	Global::memory.quote_feed_trade->ConnectQuoteServer();
	if (Global::event)
	{
		::WaitForSingleObject(Global::event, INFINITE);
		::CloseHandle(Global::event);
	}

	//instantiating future_order_shooter
	Global::memory.future_order_shooter = new FutureOrderShooter(WndMain::FutureOrderHandler);
	Global::event = ::CreateEvent(NULL, false, false, L"OnRspOrderLogin");
	if (Global::event)
	{
		Global::memory.future_order_shooter->ConnectOrderServer();
		::WaitForSingleObject(Global::event, INFINITE);
		::CloseHandle(Global::event);
	}

	//retrieve contract dictionary from broker server if it is empty
	if(Global::contract_dic.empty())
	{
		wnd_main->RetrieveContractDictionaryFromBrokerServer();
		wnd_main->is_contract_dic_retrieved = true;
	}
	
	::CloseHandle(::CreateThread(NULL, 0, ThreadMessageLoop, wnd_main, 0, &wnd_main->refresh_ui_thread_id));

	//retrieve position list from broker server
	int index = 0;
	::Sleep(1000);
	for (auto it = Global::memory.broker_info_list.begin(); it != Global::memory.broker_info_list.end(); ++it)
	{
		for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
		{
			Global::future_account_manager.UpdatePosition(&*it, &*it1);
			++index;
		}
	}

	//retrieve broker info from broker server
	::Sleep(1000);
	index = 0;
	for (auto it = Global::memory.broker_info_list.begin(); it != Global::memory.broker_info_list.end(); ++it)
	{
		for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
		{
			Global::future_account_manager.UpdateAccount(&*it, &*it1);
			++index;
		}
	}

	//delete expire contracts from local database
	wnd_main->status_bar.SetBlockText(0, L"delete expire contracts from database");
	FutureContractDictionaryModel::Delete(&Global::db, &Global::contract_dic, WndMain::ShowDatabaseErrorMessage);
	::Sleep(5000);
	wnd_main->status_bar.SetBlockText(0, L"");
	
#ifdef  _DEBUG
	//save new contracts to local database
	wnd_main->status_bar.SetBlockText(0, L"saving new contracts to database");
	FutureContractDictionaryModel::Insert(&Global::db, &Global::contract_dic, WndMain::ShowDatabaseErrorMessage);
	wnd_main->status_bar.SetBlockText(0, L"save new contracts to database-done");
	::Sleep(5000);
	wnd_main->status_bar.SetBlockText(0, L"");
#endif //  _DEBUG

	return 0;
}
DWORD WINAPI ThreadMessageLoop(LPVOID lpParam)
{
	MSG msg;
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	wchar_t wbuffer[50];
	RefreshQuote* refresh_quote;
	PositionCredential pc;
	double account_position_profit;
	typedef std::string Contract;
	typedef double Price;
	std::unordered_map<Contract, Price> contract_price_map;

	//typedef struct
	//{
	//	std::string broker_name;
	//	std::string broker_id;
	//	AccountType account_type;
	//	InvestAccountList invest_account_list;
	//	std::pair<std::vector<std::pair<OrderFront, TCPPortConnectionState>>, TCPPortConnectionState> order_front;//if one or more of the order fronts are connectable,it is 'CONNECTABLE', otherwise 'UNCONNECTABLE'
	//	std::pair<std::vector<std::pair<QuoteFront, TCPPortConnectionState>>, TCPPortConnectionState> quote_front;
	//}BrokerInfo;
	 
	BrokerInfoList::iterator it;
	PositionListSync::iterator it2;
	ContractDictionary::iterator it3;
	std::unordered_map<Contract, Price>::iterator it4;
	InvestAccountList::iterator it5;
	WndMain* wnd_main = (WndMain*)lpParam;
	while (::GetMessage(&msg, NULL, NULL, NULL))
	{
		switch (msg.message)
		{
#pragma region refresh quote
		case MSG_REFRESH_QUOTE:
		{
			refresh_quote = (RefreshQuote*)msg.lParam;
			if (refresh_quote == NULL)
			{
				continue;
			}
			pc.contract = refresh_quote->depth_market_data.InstrumentID;
			//if price of the up coming tick equals the one before then do nothing
			it4 = contract_price_map.find(pc.contract);
			if (it4 != contract_price_map.end())
			{
				if (it4->second != refresh_quote->depth_market_data.LastPrice)
				{
					it4->second = refresh_quote->depth_market_data.LastPrice;
				}
				else
				{
					continue;
				}
			}
			else
			{
				contract_price_map[pc.contract] = refresh_quote->depth_market_data.LastPrice;
			}
			//TODO:减少数据库表contract dictionary字段数，deliver year和deliver month可以不要

			for (it = Global::memory.broker_info_list.begin(); it != Global::memory.broker_info_list.end(); ++it)
			{
				for (it5 = it->invest_account_list.begin(); it5 != it->invest_account_list.end(); ++it5)
				{
					pc.direction = SELL;
					it2 = FindPositionFromNormalNew(&pc, &it5->position_list);
					//update price
					it2->second.UpdateCurrentPrice(refresh_quote->depth_market_data.LastPrice);
					it3 = FindContractFromNormalNew(pc.contract, &Global::contract_dic);
					if (it3 != Global::contract_dic.end())
					{
						//update profit
						it2->second.UpdateProfit((it2->second.average_price - refresh_quote->depth_market_data.LastPrice) * it3->second.price_tick * it3->second.volume_multiple * it2->second.volume);

						//refresh account position profit
						//TODO:refresh account profit
						/*account_position_profit = 0.0;
						for (it2 = wnd_main->pages[tab_index].invest_account->position_list.begin(); it2 != wnd_main->pages[tab_index].invest_account->position_list.end(); ++it2)
						{
						if (it2->first.expire_flag == NORMAL || it2->first.expire_flag == NEW)
						{
						account_position_profit += it2->second.profit;
						}

						}
						sprintf_s(buffer, ARRAYCOUNT(buffer), "position profit %.2f", account_position_profit);
						wnd_main->pages[tab_index].static_position_profit->SetTextA(buffer);*/
					}


					pc.direction = BUY;
					it2 = FindPositionFromNormalNew(&pc, &it5->position_list);
					//update price
					it2->second.UpdateCurrentPrice(refresh_quote->depth_market_data.LastPrice);
					it3 = FindContractFromNormalNew(pc.contract, &Global::contract_dic);
					if (it3 != Global::contract_dic.end())
					{
						//update profit
						it2->second.UpdateProfit(-(it2->second.average_price - refresh_quote->depth_market_data.LastPrice) * it3->second.price_tick * it3->second.volume_multiple * it2->second.volume);//the above line uses the SELL way to calculate profit, so you should get opposite
																																													//refresh account position profit																															//TODO:refresh account profit
																																													/*account_position_profit = 0.0;
																																																		for (it2 = wnd_main->pages[tab_index].invest_account->position_list.begin(); it2 != wnd_main->pages[tab_index].invest_account->position_list.end(); ++it2)
																																																		{
																																																		if (it2->first.expire_flag == NORMAL || it2->first.expire_flag == NEW)
																																																		{
																																																		account_position_profit += it2->second.profit;
																																																		}

																																																		}
																																																		sprintf_s(buffer, ARRAYCOUNT(buffer), "position profit %.2f", account_position_profit);
																																																		wnd_main->pages[tab_index].static_position_profit->SetTextA(buffer);*/
					}
				}
			}

			//refresh on edit_price
			if (wnd_main->input_contract == pc.contract)
			{
				swprintf(wbuffer, 50, L"%.3f", refresh_quote->depth_market_data.LastPrice);
				wnd_main->edit_price.SetMask(wbuffer);
			}
		}
		break;
#pragma endregion 
		default:
			break;
		}

	}
#pragma endregion
	return 0;
}

WndMain* WndMain::local_wnd_main = NULL;
WndMain::WndMain(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type)
{
	InitWindow(window_caption, parent_hwnd, height, width, type);
	wnd_choose_account = NULL;
	input_contract = "";
	is_contract_dic_retrieved = false;
}
WndMain::~WndMain()
{
	if (wnd_choose_account != NULL)
	{
		delete wnd_choose_account;
		wnd_choose_account = NULL;
	}
}

long WndMain::OnCreate(unsigned int wParam, long lParam)
{
	InitComponents();
	DoInit();
	return 0;
}
long WndMain::OnClose(unsigned int wParam, long lParam)
{
	int ret(MessageBox(L"Are you sure to quit?", MB_YESNO));

	if (ret == IDYES)
	{
		for (auto it = Global::memory.quote_feed_trade->subscribe_contract_list.begin(); it != Global::memory.quote_feed_trade->subscribe_contract_list.end(); ++it)
		{
			Global::memory.quote_feed_trade->UnsubscribeQuote(it->first.c_str());
		}
		::DestroyWindow(hwnd);
	}
	return 0;
}
long WndMain::OnMenuClickRebootOS(unsigned int wParam, long lParam)
{
	if (IDOK == MessageBox(L"Are you sure you want to reboot OS right now?", MB_OKCANCEL))
	{
		OS::Reboot();
	}
	return 0;
}
long WndMain::OnMenuClickChooseAccount(unsigned int wParam, long lParam)
{
	if (wnd_choose_account == NULL)
	{
		wnd_choose_account = new WndChooseAccount(L"choose account", hwnd, 230, 173, DIALOG_UNIT);
	}
	else if (!::IsWindowVisible(wnd_choose_account->GetHWND()))
	{
		wnd_choose_account->Show(wnd_choose_account->GetHWND());
	}
	return 0;
}
long WndMain::OnMenuClickQueryPositionDetail(unsigned int wParam, long lParam)
{
	//FutureOrderProcessor* spi = pages[listbox_select_index].invest_account->orderapi_spi_pair.second;
	//if (spi)
	//{
	//	memory.future_account_manager.QueryPositionDetail(spi->broker_info, pages[listbox_select_index].invest_account);
	//}
	return 0;
}
long WndMain::OnTreeClick(unsigned int wParam, long lParam)
{
	//this function will hide current page and show intended page
	char* intended_page_name;
	ShowPage(intended_page_name);
	return 0;
}
long WndMain::OnNCDestroy(unsigned int wParam, long lParam)
{
	DoCleanUp();
	PostQuitMessage(0);
	return 0;
}
long WndMain::OnButtonClickGo(unsigned int wParam, long lParam)
{
	char price_str[11];
	wchar_t price_wstr[11];
	char volume_str[11];
	Direction direction;
	OpenFlag open_flag;
	double price;
	int volume;
	if (input_contract == "")
	{
		std::string msg("failed to insert order. contract null");
		edit_app_log.AppendLineA(msg.c_str());
	}
	else if (combo_direction.GetSelectIndex() == 0)
	{
		std::string msg("failed to insert order. haven't selected direction");
		edit_app_log.AppendLineA(msg.c_str());
	}
	else if (combo_open_flag.GetSelectIndex() == 0)
	{
		std::string msg("failed to insert order. haven't selected open flag");
		edit_app_log.AppendLineA(msg.c_str());
	}
	else
	{
		edit_price.GetTextA(price_str, 11); edit_price.GetMask(price_wstr, 11);

		if (strcmp(price_str, "") == 0 && wcscmp(price_wstr, L"") == 0)
		{
			std::string msg("failed to insert order. price null");
			edit_app_log.AppendLineA(msg.c_str());
		}
		else
		{
			edit_volume.GetTextA(volume_str, 11);
			if (strcmp(volume_str, "") == 0)
			{
				std::string msg("failed to insert order. volume null");
				edit_app_log.AppendLineA(msg.c_str());
			}
			else
			{
				//write order insert logic here
				FutureOrderProcessor* order_spi(Global::invest_accont_list[current_page].orderapi_spi_pair.second);
				if (order_spi)
				{
					if (combo_direction.GetSelectIndex() == 1)
					{
						direction = SELL;
					}
					else if (combo_direction.GetSelectIndex() == 2)
					{
						direction = BUY;
					}
					if (combo_open_flag.GetSelectIndex() == 1)
					{
						open_flag = CLOSE;
					}
					else if (combo_open_flag.GetSelectIndex() == 2)
					{
						open_flag = OPEN;
					}

					if (strcmp(price_str, "") != 0)
					{
						price = atoi(price_str);
					}
					else
					{
						price = _wtoi(price_wstr);
					}
					volume = atoi(volume_str);
					Global::memory.future_order_shooter->Shoot(&input_contract[0], price, direction, open_flag, volume, order_spi->broker_info, order_spi->invest_account);
				}
			}
		}
	}
	return 0;
}
long WndMain::OnEditContractKillFocus(unsigned int wParam, long lParam)
{
	char buffer[8];
	edit_contract.GetTextA(buffer, 8);
	std::string contract = buffer;
	//select_contract是旧的，contract是新的
	if (contract != input_contract)
	{
		//unsubscribe old contract
		if (Global::memory.quote_feed_trade->subscribe_contract_list.find(input_contract) != Global::memory.quote_feed_trade->subscribe_contract_list.end())
		{
			if (--Global::memory.quote_feed_trade->subscribe_contract_list[input_contract] == 0)
			{
				Global::memory.quote_feed_trade->subscribe_contract_list.erase(input_contract);
				Global::memory.quote_feed_trade->UnsubscribeQuote(input_contract.c_str());
			}
		}
		//subscribe new contract
		if (Global::memory.quote_feed_trade->subscribe_contract_list.find(contract) == Global::memory.quote_feed_trade->subscribe_contract_list.end())
		{
			Global::memory.quote_feed_trade->subscribe_contract_list.insert(std::make_pair(contract, 1));
			Global::memory.quote_feed_trade->SubscribeQuote(contract.c_str());
		}
		else
		{
			++Global::memory.quote_feed_trade->subscribe_contract_list[contract];
		}
		input_contract = contract;
	}
	return 0;
}

void WndMain::InitComponents()
{
	Wnd::CenterWindow(hwnd);
	Menu menu;
	menu.InsertTtem(L"&reboot");
	menu.InsertTtem(L"&reboot OS", L"&reboot");
	AddHandler(MN_CLICKED, menu.GetResourceID(L"&reboot OS") , static_cast<MessageHandler>(&WndMain::OnMenuClickRebootOS));
	menu.InsertTtem(L"&reboot software", L"&reboot");
	menu.InsertTtem(L"&query");
	menu.InsertTtem(L"&query", L"&query position detail");
	AddHandler(MN_CLICKED, menu.GetResourceID(L"&query position detail"), static_cast<MessageHandler>(&WndMain::OnMenuClickQueryPositionDetail));
	menu.InsertTtem(L"&settings");
	menu.InsertTtem(L"&choose account", L"&settings");
	AddHandler(MN_CLICKED, menu.GetResourceID(L"&choose account"), static_cast<MessageHandler>(&WndMain::OnMenuClickChooseAccount));
	
	TreeView tree_modules = TreeView(7, 0, 61, 115, DIALOG_UNIT);
	AddHandler(TVN_SELCHANGED, &tree_modules, static_cast<MessageHandler>(&WndMain::OnTreeClick));
	LoadModules(&tree_modules);
	
	edit_app_log = EditWithLog(7, 123, 389, 54, DIALOG_UNIT);

	edit_contract = Edit(69, 101, 35, 12, SINGLE_LINE, NO_SCROLLBAR, DIALOG_UNIT);
	AddHandler(EN_KILLFOCUS, &edit_contract, static_cast<MessageHandler>(&WndMain::OnEditContractKillFocus));
	edit_contract.SetMask(L"contract");

	combo_direction = ComboBox(104, 101, 30, 14, DIALOG_UNIT);
	combo_direction.AddString(L"direction");
	combo_direction.AddString(L"sell");
	combo_direction.AddString(L"buy");
	combo_direction.SetSelection(0);

	combo_open_flag = ComboBox(134, 101, 30, 14, DIALOG_UNIT);
	combo_open_flag.AddString(L"open flag");
	combo_open_flag.AddString(L"close");
	combo_open_flag.AddString(L"open");
	combo_open_flag.SetSelection(0);

	edit_price = UnsignedFloatEdit(164, 101, 30, 12, DIALOG_UNIT);
	edit_price.SetMask(L"price");

	edit_volume = DigitalEdit(194, 101, 30, 12, DIALOG_UNIT);
	edit_volume.SetMask(L"volume");

	Button button_go = Button(L"Go", 224, 101, 12, 12, DIALOG_UNIT);
	AddHandler(BN_CLICKED, &button_go, static_cast<MessageHandler>(&WndMain::OnButtonClickGo));

	std::vector<WidthPercent> width_percents({ 50, 50 });
	status_bar = StatusBar(&width_percents);

	local_wnd_main = this;
	local_edit_app_log = &edit_app_log;
}
void WndMain::InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type)
{
	lpWindowName = window_caption;
	y = x = CW_USEDEFAULT;
	Height = height;
	Width = width;
	hwndParent = parent_hwnd;
	AddHandler(WM_CREATE, static_cast<MessageHandler>(&WndMain::OnCreate));
	AddHandler(WM_CLOSE, static_cast<MessageHandler>(&WndMain::OnClose));
	AddHandler(WM_NCDESTROY, static_cast<MessageHandler>(&WndMain::OnNCDestroy));
	_CreateWindow(type);
}

void WndMain::DatabaseInit()
{
	if (!ShowDatabaseErrorMessage(Global::db.Connect()))
	{
		//step 1:for first use
		if (!Global::db.IsTableExist("machine_list"))
		{
			
		}
		//step 2:trade initialization of database section
		//TODO:move function RetrieveAccountList from class FutureAccountManager to class FutureCredentialModel
		
		Global::future_account_manager.RetrieveAccountList(&Global::memory.broker_info_list);
		FutureContractDictionaryModel::CreateTable(&Global::db, ShowDatabaseErrorMessage);

		bool is_table_credential_created;
		std::string _msg;
		unsigned int index = 0;
		for (auto it = Global::memory.broker_info_list.begin(); it != Global::memory.broker_info_list.end(); ++it)
		{
			//create table credential
			_msg = "credential_" + it->broker_name;
			if (!Global::db.IsTableExist(_msg.c_str()))
			{
				is_table_credential_created = false;
				FutureCredentialModel::CreateTable(&Global::db, _msg.c_str(), ShowDatabaseErrorMessage);
			}
			else
			{
				is_table_credential_created = true;
			}

			for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
			{
				//insert into credential
				if (!is_table_credential_created)
				{
					_msg = "credential_" + it->broker_name;
					FutureCredentialModel::Insert(&Global::db, _msg.c_str(), &*it1, ShowDatabaseErrorMessage);
				}

				//create table position list
				_msg = "position_" + it->broker_name + "_" + it1->investor_id;
				FuturePositionModel::CreateTable(&Global::db, _msg.c_str(), ShowDatabaseErrorMessage);

				//retrieve position list from database
				_msg = "position_" + it->broker_name + "_" + it1->investor_id;
				FuturePositionModel::Retrieve(&Global::db, _msg.c_str(), &it1->position_list, WndMain::ShowDatabaseErrorMessage, index);

				

				FuturePositionController::RenderOnWndMain(&it1->position_list);
				for (auto it2 = it1->position_list.begin(); it2 != it1->position_list.end(); ++it2)
				{
					it2->second.volume = 0;
				}

				//create table order list
				_msg = "order_" + it->broker_name + "_" + it1->investor_id;
				FutureOrderModel::CreateTable(&Global::db, _msg.c_str(), ShowDatabaseErrorMessage);

				//retrieve order list from database
				_msg = "order_" + it->broker_name + "_" + it1->investor_id;
				FutureOrderModel::Retrieve(&Global::db, _msg.c_str(), &it1->_order_list, WndMain::ShowDatabaseErrorMessage, index);
				

				//retrieve broker info from database 
				_msg = "credential_" + it->broker_name + " WHERE investor_id = '" + it1->investor_id + "';";
				FutureCredentialModel::Retrieve(&Global::db, _msg.c_str(), &*it1, WndMain::ShowDatabaseErrorMessage, index);
				it1->UpdateAccountInfo();

				++index;
			}
		}
		//retrieve contract dictionary from local database
		FutureContractDictionaryModel::Retrieve(&Global::db, &Global::contract_dic, ShowDatabaseErrorMessage);
		::CloseHandle(::CreateThread(NULL, 0, ThreadTradeInit, this, 0, 0));
	}
}
void WndMain::DoCleanUp()
{
	//note:this function is executed in the WM_NCDESTROY handler, all UI related operations should be removed
	edit_app_log.WriteLogLocal("close");
	if (Global::memory.future_order_shooter && Global::db.IsConnected())
	{
		char statement[MAX_PATH];
		std::string direction;

#ifndef  _DEBUG
		//retrieve contract dict from broker server if haven't been retrieved in system initialization
		if (!is_contract_dic_retrieved)
		{
			RetrieveContractDictionaryFromBrokerServer();
		}
		//save new contracts to local database
		FutureContractDictionaryModel::Insert(&memory.db, &memory.contract_dic, WndMain::ShowDatabaseErrorMessage);
#endif //  _DEBUG

		for (auto it2 = Global::memory.broker_info_list.begin(); it2 != Global::memory.broker_info_list.end(); ++it2)
		{
			for (auto it = it2->invest_account_list.begin(); it != it2->invest_account_list.end(); ++it)
			{
				if (it->orderapi_spi_pair.second)
				{
					//update credential from local database
					sprintf_s(statement, ARRAYCOUNT(statement), "UPDATE credential_%s SET balance = %.2f, position_profit = %.2f, close_profit = %.2f, margin = %.2f, frozen = %.2f, available = %.2f WHERE investor_id = '%s';", it->orderapi_spi_pair.second->broker_info->broker_name.c_str(), it->orderapi_spi_pair.second->invest_account->balance, it->orderapi_spi_pair.second->invest_account->position_profit, it->orderapi_spi_pair.second->invest_account->close_profit, it->orderapi_spi_pair.second->invest_account->margin, it->orderapi_spi_pair.second->invest_account->frozen, it->orderapi_spi_pair.second->invest_account->available, it->orderapi_spi_pair.second->invest_account->investor_id.c_str());
					ShowDatabaseErrorMessage(Global::db.Update(statement));

					//update order list to database
					sprintf_s(statement, ARRAYCOUNT(statement), "order_%s_%s", it2->broker_name.c_str(), it->investor_id.c_str());
					FutureOrderModel::Update(&Global::db, statement, &it->_order_list, ShowDatabaseErrorMessage, it->orderapi_spi_pair.second->index);

					//update position list to database
					sprintf_s(statement, ARRAYCOUNT(statement), "position_%s_%s", it2->broker_name.c_str(), it->investor_id.c_str());
					FuturePositionModel::Update(&Global::db, statement, &it->position_list, ShowDatabaseErrorMessage, it->orderapi_spi_pair.second->index);
				}
			}
		}
	}

	std::wstring ctp_con_dir;
	//clean up ctp order con
	OS::CommonDirectory::GetExeDirW(&ctp_con_dir);
	FileIO::ClearDirectory((ctp_con_dir + L"\\CTPOrderCon").c_str(), (ctp_con_dir + +L"\\CTPOrderCon").c_str());
	//clean up ctp quote con
	OS::CommonDirectory::GetExeDirW(&ctp_con_dir);
	FileIO::ClearDirectory((ctp_con_dir + L"\\CTPQuoteConForTrade").c_str(), (ctp_con_dir + +L"\\CTPQuoteConForTrade").c_str());
}
void WndMain::DoInit()
{
	//create folders
	::CreateDirectoryW(L"Log", NULL);
	::CreateDirectoryW(L"CTPOrderCon", NULL);
	::CreateDirectoryW(L"CTPQuoteConForTrade", NULL);
	::CreateDirectoryW(L"Tick", NULL);
	::CreateDirectoryW(L"Tick\\Future", NULL);
	std::string current_year(DateTime::GetCurrentYearString());
	std::string current_month(DateTime::GetCurrentMonthString());
	std::string current_day(DateTime::GetCurrentDayString());
	::CreateDirectoryA(("Tick\\Future\\" + current_year).c_str(), NULL);
	::CreateDirectoryA(("Tick\\Future\\" + current_year + "\\" + current_month).c_str(), NULL);
	::CreateDirectoryA(("Tick\\Future\\" + current_year + "\\" + current_month + "\\" + current_day).c_str(), NULL);

	edit_app_log.WriteLogLocal("open");
#pragma region fire reboot
	//set Window::reboot_entry
	now = ::time(0);
	char ini_reboot_time1[20];
	char ini_reboot_time[20];
	struct tm tm;
	localtime_s(&tm, &now);
	time_t reboot_time1, reboot_time2, reboot_time3;
	std::vector<std::string>tokens;

	FileIO::GetIniValueA("App", "reboot_time1", ini_reboot_time1, 20);
	String::Split(&std::string(ini_reboot_time1), ':', &tokens);
	if (tokens.size() == 3)
	{
		tm.tm_hour = atoi(tokens[0].c_str());
		tm.tm_min = atoi(tokens[1].c_str());
		tm.tm_sec = atoi(tokens[2].c_str());
	}
	reboot_time1 = ::mktime(&tm);

	FileIO::GetIniValueA("App", "reboot_time2", ini_reboot_time, 20);
	String::Split(&std::string(ini_reboot_time), ':', &tokens);
	if (tokens.size() == 3)
	{
		tm.tm_hour = atoi(tokens[0].c_str());
		tm.tm_min = atoi(tokens[1].c_str());
		tm.tm_sec = atoi(tokens[2].c_str());
	}
	reboot_time2 = ::mktime(&tm);

	FileIO::GetIniValueA("App", "reboot_time3", ini_reboot_time, 20);
	String::Split(&std::string(ini_reboot_time), ':', &tokens);
	if (tokens.size() == 3)
	{
		tm.tm_hour = atoi(tokens[0].c_str());
		tm.tm_min = atoi(tokens[1].c_str());
		tm.tm_sec = atoi(tokens[2].c_str());
	}
	reboot_time3 = ::mktime(&tm);

	if (difftime(now, reboot_time1) <= 0.0)
		reboot_entry = reboot_time1;
	else if (difftime(now, reboot_time1) > 0.0 && difftime(now, reboot_time2) <= 0.0)
		reboot_entry = reboot_time2;
	else if (difftime(now, reboot_time2) > 0.0 && difftime(now, reboot_time3) <= 0.0)
		reboot_entry = reboot_time3;
	else
	{
		++tm.tm_mday;
		now = ::mktime(&tm);
		localtime_s(&tm, &now);
		String::Split(&std::string(ini_reboot_time1), ':', &tokens);
		if (tokens.size() == 3)
		{
			tm.tm_hour = atoi(tokens[0].c_str());
			tm.tm_min = atoi(tokens[1].c_str());
			tm.tm_sec = atoi(tokens[2].c_str());
		}
		reboot_entry = ::mktime(&tm);
	}

	//set a timer to reboot at specific time 
	::SetTimer(NULL, 0, MILLIPERMINUTE * MINUTES, (TIMERPROC)&TimerRebootAtSpecificTime);
#pragma endregion 

	DatabaseInit();
}

bool WndMain::ShowDatabaseErrorMessage(const std::string &msg)
{
	if (msg != "ok")
	{
		local_edit_app_log->WriteLogLocal(msg.c_str());
		return true;
	}
	else
	{
		return false;
	}
}
bool WndMain::ShowDatabaseErrorMessage(const std::string &msg, size_t index)
{
	//TODO:amend this funcion
	if (msg != "ok")
	{
		if (index < local_wnd_main->pages.size())
		{
			local_wnd_main->pages[index].edit->WriteLogLocal(msg.c_str());
		}
		return true;
	}
	else
	{
		return false;
	}
}

void WndMain::FutureOrderHandler(const char* msg, size_t index, const char* log_path)
{
	//TODO:amend this function
	if (msg == NULL)
	{
		if (index < local_wnd_main->pages.size())
		{
			local_wnd_main->pages[index].edit->log_path = log_path;
		}
	}
	else
	{
		local_wnd_main->pages[index].edit->WriteLog(msg, log_path);
	}
}
void WndMain::FutureQuoteHandler(const char* msg)
{
	local_wnd_main->edit_app_log.WriteLog(msg);
}

void WndMain::RetrieveContractDictionaryFromBrokerServer()
{
	FutureOrderProcessor* order_spi = Global::memory.broker_info_list[0].invest_account_list[0].orderapi_spi_pair.second;
	if (order_spi)
	{
		//ReqQryInstrument参数填null时，实盘比Simnow多出来的是大连的组合合约，郑州的组合合约，上期的EFP合约
		CThostFtdcTraderApi* order_api = Global::memory.broker_info_list[0].invest_account_list[0].orderapi_spi_pair.first;
		if (order_api)
		{
			status_bar.SetBlockText(0, L"query instrument");
			CThostFtdcQryInstrumentField field;
			ZeroMemory(&field, sizeof(field));
			Global::event = ::CreateEvent(NULL, false, false, L"QueryInstrument");
			::Sleep(1000);
			order_api->ReqQryInstrument(&field, ++order_spi->invest_account->ctp_order_request_id);
			if (Global::event)
			{
				::WaitForSingleObject(Global::event, 1000 * 10);
				::CloseHandle(Global::event);
			}
		}
	}
}

void WndMain::CheckBrokerConnection(BrokerInfoList* broker_info_list)
{
	status_bar.SetBlockText(0, L"check broker connection 0%");
	double server_total = 0.0;

	for (auto it = broker_info_list->begin(); it != broker_info_list->end(); ++it)
	{
		server_total += it->order_front.first.size() + it->quote_front.first.size();
	}

	double server_count = 0.0;
	char msg[50];
	std::string front;
	std::vector<std::string> tokens;
	for (auto it = broker_info_list->begin(); it != broker_info_list->end(); ++it)
	{
		//check order front connection
		for (auto it1 = it->order_front.first.begin(); it1 != it->order_front.first.end(); ++it1)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "check %s order connection %.2f%%", it->broker_name.c_str(), (++server_count) / server_total * 100);
			status_bar.SetBlockTextA(0, msg);
			front = it1->first;
#pragma warning(push)
#pragma warning(disable:4309)
#pragma warning(disable:4305)
			String::RemoveAllChars(&front, '//');
#pragma warning(pop)
			String::Split(&front, ':', &tokens);
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4242)
#pragma warning(disable:4365)
			if (client_socket.TestConnect(tokens[1].c_str(), atoi(tokens[2].c_str())))
#pragma warning(pop)
			{
				it->order_front.second = it->order_front.second | CONNECTABLE;
				it1->second = CONNECTABLE;
			}
			else
			{
				it1->second = UNCONNECTABLE;
			}
			if (CONNECTABLE & it->order_front.second)
			{
				it->order_front.second = CONNECTABLE;
			}
			else
			{
				it->order_front.second = UNCONNECTABLE;
			}
		}
		//check quote front connection
		for (auto it1 = it->quote_front.first.begin(); it1 != it->quote_front.first.end(); ++it1)
		{
			sprintf_s(msg, ARRAYCOUNT(msg), "check %s quote connection %.2f%%", it->broker_name.c_str(), (++server_count) / server_total * 100);
			status_bar.SetBlockTextA(0, msg);
			front = it1->first;
#pragma warning(push)
#pragma warning(disable:4309)
#pragma warning(disable:4305)
			String::RemoveAllChars(&front, '//');
#pragma warning(pop)
			String::Split(&front, ':', &tokens);
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4242)
#pragma warning(disable:4365)
			if (client_socket.TestConnect(tokens[1].c_str(), atoi(tokens[2].c_str())))
#pragma warning(pop)
			{
				it->quote_front.second = it->quote_front.second | CONNECTABLE;
				it1->second = CONNECTABLE;
			}
			else
			{
				it1->second = UNCONNECTABLE;
			}
			if (CONNECTABLE & it->quote_front.second)
			{
				it->quote_front.second = CONNECTABLE;
				if (it->account_type == REAL)
				{

				}
			}
			else
			{
				it->quote_front.second = UNCONNECTABLE;
			}
		}
	}
}

void WndMain::PageFutureTrade(char* page_name)
{
	std::vector<wchar_t*> column_headers_position
	({
		L"position_contract",
		L"profit",
		L"direction",
		L"current_price",
		L"average_price",
		L"volume",
		L"margin"
	});

	std::vector<wchar_t*> column_headers_order
	({
		L"order_contract",
		L"direction",
		L"open_flag",
		L"volume",
		L"trade_volume",
		L"status",
		L"price",
		L"insert_time",
		L"exchange",
		L"front_id",
		L"session_id",
		L"order_ref"
	});
	Edit edit = EditWithLog(250, 51, 146, 50, DIALOG_UNIT);
	DataGrid datagrid_position = DataGrid(&column_headers_position, 69, 0, 180, 51, DIALOG_UNIT);
	datagrid_position.AutoSize();
	DataGrid datagrid_order = DataGrid(&column_headers_order, 69, 51, 180, 50, DIALOG_UNIT);
	datagrid_order.AutoSize();
	Static _static = Static(L"", 7, 115, 30, 30, DIALOG_UNIT);//_static
	Static static_position_profit = Static(L"", 249, 0, 154, 30, DIALOG_UNIT);//static_position_profit
	AddPage(page_name, 5, &edit, &datagrid_position, &datagrid_order, &_static, &static_position_profit);
}

void WndMain::LoadModules(TreeView* tree_modules)
{
	std::vector<std::string> tree_nodes;
	std::vector<InvestAccountSync>invest_account_list;
	/*filter future accounts that are not allowed to log in, create tab pages in parent window*/
	for (auto it = Global::memory.broker_info_list.begin(); it != Global::memory.broker_info_list.end(); ++it)
	{
		for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end();)
		{
			tree_nodes.push_back(it->broker_name);
			invest_account_list.push_back(*it1);
			++it1;
		}
	}

	auto it1 = invest_account_list.begin();
	for (auto it = tree_nodes.begin(); it != tree_nodes.end(); ++it, ++it1)
	{
		//tab_page.invest_account = *it1;
		tree_modules->InsertItemA(&(*it)[0], "Trade");
	}
}