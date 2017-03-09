#pragma once
#include"preinclude.h"
#include"Module.h"

class SQLite;
class TradeModule:
	public Module
{
private:
	TradeModule &CreateFolders();
	TradeModule &CreateDataBaseTables();
	TradeModule &GetInvestAccountListFromDataBase();
	TradeModule &GetOrderFrontListFromDataBase();
protected:

public:
	void Load();
	void Unload() {}
};
