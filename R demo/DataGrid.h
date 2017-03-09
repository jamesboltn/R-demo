#pragma once
#include"preinclude.h"
#include"UIBase.h"
#include"ControlBase.h"

typedef std::string ColumnName;
typedef int ColumnIndex;

class DataGrid :
	public UIBase,
	public ControlBase
{
private:
	std::unordered_map<ColumnName, ColumnIndex> col_name_index_map;
public:
	DataGrid(std::vector<wchar_t*>* column_headers, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, char* _unique_name = NULL);
	DataGrid(HWND _hwnd);//accepts HWND of created DataGrid
	DataGrid() {}
	virtual ~DataGrid() {}
	//virtual ~DataGrid() {}
	void SetViewStyle(long new_view_style);

	//due to win32 listview limitations,you must add row by row. you must first insert column 0 cell, then can you insert following column cells on the same row
	void InsertCell(int row_index, int col_index, const wchar_t* text);
	void InsertCellA(int row_index, int col_index, const char* text);
	void InsertCellA(int row_index, const std::string &col_name, const char* text);

	int GetColumnCount();
	int GetRowCount();

	void DeleteAll();
	int AddRow();//return value is the index of the inserted row 
	void DeleteRow(int row_index);

	//return -1 means no rows in datagrid or no row is selected, otherwise return the selected row index
	int GetSelectedRowIndex();

	void GetCellText(int row_index, int col_index, wchar_t* buffer, int length);

	void GetCellTextA(int row_index, int col_index, char* buffer, int length);

	void AutoSize();

	void SetRowHeight(int height);

	void SetCheckBoxState(unsigned int index, bool state);

	bool GetCheckBoxState(int index);

	//note you should not combine use of AddGroups and AddGroup
	void AddGroupsUncollapsible(std::vector<wchar_t*> group_headers, Align align = LEFT);

	//note you should not combine use of AddGroups and AddGroup
	void AddGroupUncollapsible(wchar_t* group_header, int index, Align align = LEFT);

	//note you should not combine use of AddGroups and AddGroup
	void AddGroupsCollapsible(std::vector<wchar_t*> group_headers, Align align = LEFT);

	//note you should not combine use of AddGroups and AddGroup
	void AddGroupCollapsible(wchar_t* group_header, int index, Align align = LEFT);

	void MoveRowToGroup(int group_index, int row_index);

	/*during lifetime of a datagrid object, you can pick and execute one of the following  functions only once*/
	//this function should better be used right after datagrid object get constructed or in the construction	
	void AddCheckBoxToRowHead();
	/***************************/
};
