#pragma once
#include"preinclude.h"

typedef std::string ControlName;

class UIBase;
class DataGrid;
class DataBind
{
private:
	std::unordered_map<ControlName, UIBase*> bound_controls;
	static DataGrid* bound_datagrid;//only one datagrid is allowed to be bound
protected:
	char buffer[50];
public:
	DataBind();
	virtual ~DataBind();
	void BindControl(unsigned int control_count, ...);
	void BindControl(UIBase* uibase_ptr);
	void RefreshControl(char* control_name, const char* value);//refresh non-datagrid controls
	void RefreshDataGrid(int row_index, char* col_name, const char* value);
	void DeleteDataGridRow(int row_index);
	int AddDataGridRow();//return value is the index of the inserted row
};
