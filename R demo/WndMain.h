#pragma once
#include"Window.h"
#include"StatusBar.h"
#include"Edit.h"
#include"ComboBox.h"
#include"Button.h"
#include"Menu.h"
#include"FutureQuoteFeed.h"
#include"PageManager.h"
#include"ClientSocket.h"

class TreeView;
class WndChooseAccount;
class WndMain :
	public Window, public PageManager
{
private:
	WndChooseAccount* wnd_choose_account = NULL;
	ClientSocket client_socket;

	void DoInit();
	void DatabaseInit();
	void LoadModules(TreeView* tree_modules);

	void PageFutureTrade(char* page_name);

	long OnCreate(unsigned int wParam, long lParam);
	long OnClose(unsigned int wParam, long lParam);
	long OnNCDestroy(unsigned int wParam, long lParam);
	long OnMenuClickRebootOS(unsigned int wParam, long lParam);
	long OnMenuClickChooseAccount(unsigned int wParam, long lParam);
	long OnMenuClickQueryPositionDetail(unsigned int wParam, long lParam);
	long OnTreeClick(unsigned int wParam, long lParam);
	long OnEditContractKillFocus(unsigned int wParam, long lParam);
	long OnButtonClickGo(unsigned int wParam, long lParam);
protected:
	void InitComponents();
	void InitWindow(wchar_t* window_caption, HWND parent_hwnd /*= NULL*/, int height /*= CW_USEDEFAULT*/, int width /*= CW_USEDEFAULT*/, UnitType type);
public:
	static WndMain* local_wnd_main;
	StatusBar status_bar;
	EditWithLog edit_app_log;
	ComboBox combo_direction;
	Edit edit_contract;
	ComboBox combo_open_flag;
	UnsignedFloatEdit edit_price;
	DigitalEdit edit_volume;

	DWORD refresh_ui_thread_id;
	std::string input_contract = "";//the selected contract of combo
	std::string current_page;
	bool is_contract_dic_retrieved = false;//是否已经更新过合约字典
	
	WndMain(wchar_t* window_caption, HWND parent_hwnd = NULL, int height = CW_USEDEFAULT, int width = CW_USEDEFAULT, UnitType type = PIXEL);
	virtual ~WndMain();
	void DoCleanUp();
	void RetrieveContractDictionaryFromBrokerServer();
	void CheckBrokerConnection(OrderBrokerList* order_broker_list, QuoteBrokerList* quote_broker_list);
	static bool ShowDatabaseErrorMessage(const std::string &msg);//if returns true means there is error in database process
	static bool ShowDatabaseErrorMessage(const std::string &msg, const char* page_name);
	static void FutureOrderHandler(const char* msg, const char* page_name, const char* log_path = NULL);//this handler shows error message generated by future order or sets log path of each EditWithLog control to the path supplied with each future order instance 
	static void FutureQuoteHandler(const char* msg);
};


