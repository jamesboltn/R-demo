#pragma once
#include"WndChooseAccount.h"
#include"WndMain.h"
#include"FutureOrderShooter.h"
#include"FutureQuoteFeed.h"
#include"Utilities.h"
#include"Static.h"
#include"GlobalObject.h"
#include"ClientDataType.h"

static DWORD WINAPI ThreadCheckBrokerConnection(LPVOID lpParam)
{
	using namespace Global;
	WndChooseAccount* wnd_choose_account((WndChooseAccount*)lpParam);
	ToolTip tooltip;
	std::wstring str_w;
#pragma region generate listview tooltip
	for (auto it =TradeModule::order_broker_list.begin(); it != TradeModule::order_broker_list.end(); ++it)
	{
		tooltip = "order front\n";
		for (auto it1 = it->order_front_list.begin(); it1 != it->order_front_list.end(); ++it1)
		{
			tooltip += it1->first;
			if (it1->second == CONNECTABLE)
			{
				tooltip += " \n";
			}
			else
			{
				tooltip += " x\n";
			}
		}
		wnd_choose_account->tooltip_map.insert(std::pair<BrokerID, ToolTip>(it->broker_id, tooltip));
	}

	for (auto it = TradeModule::quote_broker_list.begin(); it != TradeModule::quote_broker_list.end(); ++it)
	{
		tooltip += "quote front\n";
		for (auto it1 = it->quote_front.first.begin(); it1 != it->quote_front.first.end(); ++it1)
		{
			tooltip += it1->first;
			if (it1->second == CONNECTABLE)
			{
				tooltip += " \n";
			}
			else
			{
				tooltip += " x\n";
			}
		}
		wnd_choose_account->tooltip_map.insert(std::pair<BrokerID, ToolTip>(it->broker_id, tooltip));
	}
#pragma endregion 
	int check_state(false);
#pragma region enable login button after check

	for (unsigned int i = 0; i < wnd_choose_account->datagrid_account_list.GetRowCount(); ++i)
	{
		if (wnd_choose_account->datagrid_account_list.GetCheckBoxState(i))
		{
			check_state = true;
			break;
		}
	}
	if (!check_state)
	{
		wnd_choose_account->button_login.Disable(wnd_choose_account->button_login.GetHWND());
	}
	wnd_choose_account->all_ports_checked = true;
#pragma endregion  

	return 0;
}

WndChooseAccount::WndChooseAccount(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type)
{
	InitWindow(window_caption, parent_hwnd, height, width, type);
}

WndChooseAccount::~WndChooseAccount()
{
}

long WndChooseAccount::OnCreate(unsigned int wParam, long lParam)
{
	InitComponents();
	::CloseHandle(::CreateThread(NULL, 0, ThreadCheckBrokerConnection, this, 0, NULL));
	return 0;
}

long WndChooseAccount::OnClose(unsigned int wParam, long lParam)
{
	if (::IsWindowVisible(hwnd))
	{
		Hide(hwnd);
	}
	return 0;
}

long WndChooseAccount::OnDestroy(unsigned int wParam, long lParam)
{
	if (can_post_quit)
	{
		::PostQuitMessage(0);
	}
	else
	{
		can_post_quit = true;
	}
	return 0;
}

long WndChooseAccount::OnListViewItemTooltip(unsigned int wParam, long lParam)
{
	LPNMLVGETINFOTIP pGetInfoTip((LPNMLVGETINFOTIP)lParam);
	if (pGetInfoTip != NULL)
	{
		if (!all_ports_checked)
		{
			wcscpy_s(pGetInfoTip->pszText, (rsize_t)pGetInfoTip->cchTextMax, L"is checking order front and quote front");
		}
		else
		{
			char cell_text[8];
			datagrid_account_list.GetCellTextA(pGetInfoTip->iItem, 1, cell_text, 8);
			BrokerID broker_id(cell_text);
			if (tooltip_map.find(cell_text) != tooltip_map.end())
			{
				std::wstring buffer;
				TypeConverter::StringToWString(&buffer, &tooltip_map[broker_id]);
				wcscpy_s(pGetInfoTip->pszText, (rsize_t)pGetInfoTip->cchTextMax, buffer.c_str());
			}
		}
	}
	return 0;
}

long WndChooseAccount::OnListViewItemChanging(unsigned int wParam, long lParam)
{
	LPNMLISTVIEW pNMLV(reinterpret_cast<LPNMLISTVIEW>(lParam));

	// is the user selecting an item?
	if (all_ports_checked)
	{
		if ((pNMLV->uChanged & LVIF_STATE))
		{
			status_bar_choose_account.SetBlockText(0, L"");
			if (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))
			{
				SetAccountAllowLoginStatus(pNMLV->iItem, true);
			}
			else if (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))
			{
				SetAccountAllowLoginStatus(pNMLV->iItem, false);
			}
		}
		return FALSE;
	}
	else
	{
		if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)))
		{
			// yes - never allow a selected item
			status_bar_choose_account.SetBlockText(0, L"cannot select item until all ports checked");
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

void WndChooseAccount::InitComponents()
{
	using namespace Global;
	Wnd::CenterWindow(hwnd);

	std::vector<wchar_t*> column_headers({ L"investor id", L"broker id" });
	datagrid_account_list = DataGrid(&column_headers, 6, 7, 158, 173, DIALOG_UNIT);
	datagrid_account_list.AddCheckBoxToRowHead();
	datagrid_account_list.AutoSize();
	button_login = Button(L"login", 24, 190, 50, 14, DIALOG_UNIT);
	button_login.Disable(button_login.GetHWND());
	std::vector<WidthPercent> width_percents({ 100 });
	status_bar_choose_account = StatusBar(&width_percents);
	if (!TradeModule::order_broker_list.empty())
	{
		int credential_count(0);
		int account_count(0);
		int group_id;

		for (auto it = TradeModule::order_broker_list.begin(); it != TradeModule::order_broker_list.end(); ++it)
		{
			group_id = atoi(it->broker_id.c_str());
			datagrid_account_list.AddGroupUncollapsible(L"", group_id);

			for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
			{
				datagrid_account_list.InsertCellA(account_count, 0, &(it->broker_name + it1->second.investor_id)[0]);

				datagrid_account_list.InsertCellA(account_count, 1, &it->broker_id[0]);

				datagrid_account_list.MoveRowToGroup(group_id, account_count);
				++account_count;
			}
			++credential_count;
		}
	}
}

long WndChooseAccount::OnButtonClickLogin(unsigned int wParam, long lParam)
{
	can_post_quit = false;

	/*******************************************************/
	::SendMessage(hwnd, WM_CLOSE, 0, 0);
	return 0;
}

void WndChooseAccount::SetAccountAllowLoginStatus(int datagrid_row_index, bool row_checkstate)
{
	using namespace Global;
	char broker_id[8], investor_id[18];
	datagrid_account_list.GetCellTextA(datagrid_row_index, 1, broker_id, 8);

	for (auto it = TradeModule::order_broker_list.begin(); it != TradeModule::order_broker_list.end(); ++it)
	{
		if (it->broker_id == broker_id)
		{
			datagrid_account_list.GetCellTextA(datagrid_row_index, 0, investor_id, 18);
			for (auto it1 = it->invest_account_list.begin(); it1 != it->invest_account_list.end(); ++it1)
			{
				if (String::Contains(std::string(investor_id), it1->second.investor_id))
				{
					if (row_checkstate)
					{
						if (!::IsWindowEnabled(button_login.GetHWND()))
						{
							button_login.Enable(button_login.GetHWND());
						}
					}
					else if (!row_checkstate && ::IsWindowEnabled(button_login.GetHWND()))
					{
						int check_state = false;
						for (unsigned int i = 0; i < datagrid_account_list.GetRowCount(); ++i)
						{
							if (datagrid_account_list.GetCheckBoxState(i) && i != datagrid_row_index)
							{
								check_state = true;
								break;
							}
						}
						if (!check_state)
						{
							button_login.Disable(button_login.GetHWND());
						}
					}
				}
			}
			break;
		}
	}
}

void WndChooseAccount::InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type)
{
	lpWindowName = window_caption;
	y = x = CW_USEDEFAULT;
	Height = height;
	Width = width;
	hwndParent = parent_hwnd;

	AddHandler(WM_CREATE, static_cast<MessageHandler>(&WndChooseAccount::OnCreate));
	AddHandler(WM_CLOSE, static_cast<MessageHandler>(&WndChooseAccount::OnClose));
	AddHandler(LVN_GETINFOTIP, static_cast<MessageHandler>(&WndChooseAccount::OnListViewItemTooltip));
	AddHandler(LVN_ITEMCHANGING, static_cast<MessageHandler>(&WndChooseAccount::OnListViewItemChanging));
	AddHandler(WM_DESTROYING, static_cast<MessageHandler>(&WndChooseAccount::OnDestroy));
	AddHandler(BN_CLICKED, &button_login, static_cast<MessageHandler>(&WndChooseAccount::OnButtonClickLogin));

	_CreateWindow(type);
}