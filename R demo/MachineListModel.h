#pragma once
#include"ModelDataType.h"

class SQLite;
class MachineListModel
{
public:
	static void CreateTable(SQLite* db, DatabaseErrorMessageHandler handler);
};
