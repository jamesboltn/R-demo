#pragma once
#define DLL_EXPORT
#include"PreInclude.h"
#include"UIBase.h"
#include"ControlBase.h"

namespace GuruLib
{
	typedef std::string ColumnName;
	typedef int ColumnIndex;

	class DataGrid :
		public UIBase,
		public ControlBase
	{
	private:
		std::unordered_map<ColumnName, ColumnIndex> col_name_index_map;
	public:
		LIB DataGrid(const std::vector<wchar_t*> &column_headers, int x_pos, int y_pos, int width, int height, UnitType type = PIXEL, const char* _unique_name = NULL);
		LIB explicit DataGrid(HWND _hwnd);//accepts HWND of created DataGrid
		LIB DataGrid() {}
		LIB virtual ~DataGrid() {}

		LIB void SetViewStyle(long new_view_style);

		//Due to win32 listview limitations,you must add row by row. you must first insert column 0 cell, then can you insert following column cells on the same row
		LIB void InsertCell(int row_index, int col_index, const wchar_t* text);
		LIB void InsertCellA(int row_index, int col_index, const char* text);
		LIB void InsertCellA(int row_index, const std::string &col_name, const char* text);

		LIB int GetColumnCount();
		LIB int GetRowCount();

		LIB void DeleteAll();
		LIB int AddRow();//return value is the index of the inserted row 
		LIB void DeleteRow(int row_index);

		//return -1 means no rows in datagrid or no row is selected, otherwise return the selected row index
		LIB int GetSelectedRowIndex();

		LIB void GetCellText(int row_index, int col_index, wchar_t* buffer, int length);

		LIB void GetCellTextA(int row_index, int col_index, char* buffer, int length);

		LIB void AutoSize();

		LIB void SetRowHeight(int height);

		LIB void SetCheckBoxState(unsigned int index, bool state);

		LIB bool GetCheckBoxState(int index);

		//note you should not combine use of AddGroups and AddGroup
		LIB void AddGroupsUncollapsible(std::vector<wchar_t*> group_headers, AlignOrientation align = LEFT_SIDE);

		//note you should not combine use of AddGroups and AddGroup
		LIB void AddGroupUncollapsible(wchar_t* group_header, int index, AlignOrientation align = LEFT_SIDE);

		//note you should not combine use of AddGroups and AddGroup
		LIB void AddGroupsCollapsible(const std::vector<wchar_t*> &group_headers, AlignOrientation align = LEFT_SIDE);

		//note you should not combine use of AddGroups and AddGroup
		LIB void AddGroupCollapsible(wchar_t* group_header, int index, AlignOrientation align = LEFT_SIDE);

		LIB void MoveRowToGroup(int group_index, int row_index);

		/*during lifetime of a datagrid object, you can pick and execute one of the following  functions only once*/
		//this function should better be used right after datagrid object get constructed or in the construction	
		LIB void AddCheckBoxToRowHead();
		/***************************/
	};

}
