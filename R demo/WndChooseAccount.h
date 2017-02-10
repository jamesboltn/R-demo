#pragma once
#include"Window.h"
#include"StatusBar.h"
#include"DataGrid.h"
#include"Button.h"

typedef std::string BrokerID;
typedef std::string ToolTip;
typedef std::map<BrokerID, ToolTip> ToolTipMap;

class WndMain;
class WndChooseAccount :
	public Window
{
private:
	bool can_post_quit;
	long OnCreate(unsigned int wParam, long lParam);
	long OnClose(unsigned int wParam, long lParam);
	long OnDestroy(unsigned int wParam, long lParam);
	long OnButtonClickLogin(unsigned int wParam, long lParam);
	long OnListViewItemTooltip(unsigned int wParam, long lParam);
	long OnListViewItemChanging(unsigned int wParam, long lParam);
	//datagrid_row is zero based, return TRUE means all order fronts of the broker are unconnectable
	void SetAccountAllowLoginStatus(int datagrid_row_index, bool row_checkstate);
protected:
	void InitComponents();
	void InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type);
public:
	StatusBar status_bar_choose_account;
	DataGrid datagrid_account_list;
	Button button_login;
	bool all_ports_checked;
	ToolTipMap tooltip_map;

	WndChooseAccount(wchar_t* window_caption, HWND parent_hwnd = NULL, int height = CW_USEDEFAULT, int width = CW_USEDEFAULT, UnitType type = PIXEL);
	virtual ~WndChooseAccount();
};
