#pragma once
#include"DataGrid.h"
#include"Utilities.h"

GuruLib::DataGrid::DataGrid(const std::vector<wchar_t*> &column_headers, int x_pos, int y_pos, int _width, int _height, GuruLib::UnitType type, const char* _unique_name)
{
	//we're going to assume the following members are declared in Window as protected
	//lpWindowName = L"";
	window_class_name = WC_LISTVIEW;
	style_flag = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | LVS_ALIGNTOP;
	y = y_pos;
	x = x_pos;
	height = _height;
	width = _width;
	hMenu = HMENU(resource_id);
	_CreateWindow(type);

	if (_unique_name != NULL)
	{
		unique_name = _unique_name;
	}

	SendMessage(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_LABELTIP);

	//add column headers
	int column_count = 0;
	std::string column_header;
	for (auto it = column_headers.begin(); it != column_headers.end(); ++it)
	{
		// Initialize the LVCOLUMN structure.
		// The mask specifies that the format, width, text,
		// and subitem members of the structure are valid.
		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = column_count;
		lvc.pszText = *it;
		lvc.cx = 100;               // Width of column in pixels.
		lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.

								// Insert the columns into the list view.
		ListView_InsertColumn(hwnd, column_count, &lvc);

		//fill col_name_index_map
		TypeConverter::WCharStrToString(*it, &column_header);
		col_name_index_map.insert({ column_header, column_count++ });
	}

	
}
void GuruLib::DataGrid::SetViewStyle(long new_view_style)
{
	// Retrieve the current window style. 
	long dw_style = GetWindowLong(hwnd, GWL_STYLE);

	// Set the window style only if the view bits changed.
	if ((dw_style & LVS_TYPEMASK) != new_view_style)
	{
		SetWindowLong(hwnd,
			GWL_STYLE,
			(dw_style & ~LVS_TYPEMASK) | new_view_style);
		// Logical OR'ing of dwView with the result of 
		// a bitwise AND between dwStyle and 
		// the Unary complenent of LVS_TYPEMASK.
	}
}
void GuruLib::DataGrid::InsertCell(int row_index, int col_index, const wchar_t* text)
{
	LVITEM listview_item;
	// Initialize LVITEM members that are common to all items.
	listview_item.pszText = const_cast<wchar_t*>(text); // Sends an LVN_GETDISPINFO message.
	listview_item.mask = LVIF_TEXT | LVIF_STATE;
	listview_item.stateMask = 0;
	listview_item.state = 0;
	listview_item.iItem = row_index;
	listview_item.iSubItem = col_index;

	//add new item to list
	if (!col_index)
	{
		ListView_InsertItem(hwnd, &listview_item);
	}
	else
	{
		// add subitem to existing item
		ListView_SetItem(hwnd, &listview_item);
	}
}
void GuruLib::DataGrid::InsertCellA(int row_index, int col_index, const char* text)
{
	LVITEMA listview_item;
	// Initialize LVITEM members that are common to all items.
	listview_item.pszText = const_cast<char*>(text); // Sends an LVN_GETDISPINFO message.
	listview_item.mask = LVIF_TEXT | LVIF_STATE;
	listview_item.stateMask = 0;
	listview_item.state = 0;
	listview_item.iItem = row_index;
	listview_item.iSubItem = col_index;

	//add new item to list
	if (!col_index)
	{
		::SendMessageA(hwnd, LVM_INSERTITEMA, 0, (LPARAM)(const LV_ITEMA *)(&listview_item));
		//ListView_InsertItem(hwnd, &listview_item);
	}
	else
	{
		// add subitem to existing item
		//ListView_SetItem(hwnd, &listview_item);
		::SendMessageA(hwnd, LVM_SETITEMA, 0, (LPARAM)(const LV_ITEMA *)(&listview_item));
	}
}
void GuruLib::DataGrid::InsertCellA(int row_index, const std::string &col_name, const char* text)
{
	if (col_name_index_map.find(col_name) != col_name_index_map.end())
	{
		LVITEMA listview_item;
		// Initialize LVITEM members that are common to all items.
		listview_item.pszText = const_cast<char*>(text); // Sends an LVN_GETDISPINFO message.
		listview_item.mask = LVIF_TEXT | LVIF_STATE;
		listview_item.stateMask = 0;
		listview_item.state = 0;
		listview_item.iItem = row_index;
		int col_index = col_name_index_map[col_name];
		listview_item.iSubItem = col_index;
		//add new item to list
		if (!col_index)
		{
			::SendMessageA(hwnd, LVM_INSERTITEMA, 0, (LPARAM)(const LV_ITEMA *)(&listview_item));
		}
		else
		{
			// add subitem to existing item
			::SendMessageA(hwnd, LVM_SETITEMA, 0, (LPARAM)(const LV_ITEMA *)(&listview_item));
		}
	}
}
int GuruLib::DataGrid::GetColumnCount()
{
	return Header_GetItemCount(ListView_GetHeader(hwnd));
}
int GuruLib::DataGrid::GetRowCount()
{
	return ListView_GetItemCount(hwnd);
}
void GuruLib::DataGrid::DeleteAll()
{
	ListView_DeleteAllItems(hwnd);
}
void GuruLib::DataGrid::DeleteRow(int row_index)
{
	ListView_DeleteItem(hwnd, row_index);
}
int GuruLib::DataGrid::AddRow()
{
	LVITEMA listview_item;
	// Initialize LVITEM members that are common to all items.
	listview_item.pszText = ""; // Sends an LVN_GETDISPINFO message.
	listview_item.mask = LVIF_TEXT | LVIF_STATE;
	listview_item.stateMask = 0;
	listview_item.state = 0;
	listview_item.iItem = GetRowCount();
	listview_item.iSubItem = 0;
	return ::SendMessageA(hwnd, LVM_INSERTITEMA, 0, (LPARAM)(const LV_ITEMA *)(&listview_item));
}
int GuruLib::DataGrid::GetSelectedRowIndex()
{
	return ::SendMessage(hwnd, LVM_GETNEXTITEM, WPARAM(-1), LVNI_FOCUSED);
}
void GuruLib::DataGrid::GetCellText(int row_index, int col_index, wchar_t* buffer, int length)
{
	LVITEM LvItem;
	ZeroMemory(&LvItem, sizeof(LvItem));
	LvItem.mask = LVIF_TEXT;
	LvItem.iSubItem = col_index;
	LvItem.pszText = buffer;
	LvItem.cchTextMax = length + 1;
	LvItem.iItem = row_index;

	SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)row_index, (LPARAM)&LvItem);
}
void GuruLib::DataGrid::GetCellTextA(int row_index, int col_index, char* buffer, int length)
{
	LVITEMA LvItem;
	ZeroMemory(&LvItem, sizeof(LvItem));
	LvItem.mask = LVIF_TEXT;
	LvItem.iSubItem = col_index;
	LvItem.pszText = buffer;
	LvItem.cchTextMax = length + 1;
	LvItem.iItem = row_index;

	::SendMessageA(hwnd, LVM_GETITEMTEXTA, (WPARAM)row_index, (LPARAM)&LvItem);
}
void GuruLib::DataGrid::AddCheckBoxToRowHead()
{
	::SendMessage(hwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP |
		LVS_EX_LABELTIP);
}
void GuruLib::DataGrid::AutoSize()
{
	for (unsigned int i = 0; i < GetColumnCount(); ++i)
	{
		// Set column widths
		ListView_SetColumnWidth(hwnd, i, LVSCW_AUTOSIZE_USEHEADER);
	}
}
void GuruLib::DataGrid::SetRowHeight(int height)
{
	static HIMAGELIST hIconList;
	ImageList_Destroy(hIconList);
	hIconList = ImageList_Create(1, height, ILC_COLOR, 0, 1);
	ListView_SetImageList(hwnd, hIconList, LVSIL_SMALL);
}
void GuruLib::DataGrid::SetCheckBoxState(unsigned int index, bool state)
{
#pragma warning(push)
#pragma warning(disable:4365)
	ListView_SetCheckState(hwnd, index, state);
#pragma warning(pop)
}
bool GuruLib::DataGrid::GetCheckBoxState(int index)
{
	if (ListView_GetCheckState(hwnd, index))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void GuruLib::DataGrid::AddGroupsCollapsible(const std::vector<wchar_t*> &group_headers, AlignOrientation align)
{
	ListView_EnableGroupView(hwnd, TRUE);

	LVGROUP group;

	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_HEADER | LVGF_GROUPID | LVGF_STATE;
	group.state = LVGS_COLLAPSIBLE;
	group.stateMask = LVGS_COLLAPSIBLE;

	if (align != LEFT_SIDE)
	{
		group.mask |= LVGF_ALIGN;

		if (align == CENTER)
		{
			group.uAlign = LVGA_HEADER_CENTER;
		}
		else if (align == RIGHT_SIDE)
		{
			group.uAlign = LVGA_HEADER_RIGHT;
		}
	}

	int index(0);
	for (auto it = group_headers.begin(); it != group_headers.end(); ++it)
	{
		group.pszHeader = *it;
		group.iGroupId = index;
		ListView_InsertGroup(hwnd, -1, &group);
		++index;
	}
}
void GuruLib::DataGrid::AddGroupCollapsible(wchar_t* group_header, int index, AlignOrientation align)
{
	ListView_EnableGroupView(hwnd, TRUE);

	LVGROUP group;

	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_HEADER | LVGF_GROUPID | LVGF_STATE;
	group.state = LVGS_COLLAPSIBLE;
	group.stateMask = LVGS_COLLAPSIBLE;

	if (align != LEFT_SIDE)
	{
		group.mask |= LVGF_ALIGN;

		if (align == CENTER)
		{
			group.uAlign = LVGA_HEADER_CENTER;
		}
		else if (align == RIGHT_SIDE)
		{
			group.uAlign = LVGA_HEADER_RIGHT;
		}
	}

	group.pszHeader = group_header;
	group.iGroupId = index;
	ListView_InsertGroup(hwnd, -1, &group);
}
void GuruLib::DataGrid::AddGroupsUncollapsible(std::vector<wchar_t*> group_headers, AlignOrientation align)
{
	ListView_EnableGroupView(hwnd, TRUE);

	LVGROUP group;

	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_HEADER | LVGF_GROUPID;

	if (align != LEFT_SIDE)
	{
		group.mask |= LVGF_ALIGN;
		if (align == CENTER)
		{
			group.uAlign = LVGA_HEADER_CENTER;
		}
		else if (align == RIGHT_SIDE)
		{
			group.uAlign = LVGA_HEADER_RIGHT;
		}
	}

	int index(0);
	for (auto it = group_headers.begin(); it != group_headers.end(); ++it)
	{
		group.pszHeader = *it;
		group.iGroupId = index;
		ListView_InsertGroup(hwnd, -1, &group);
		++index;
	}
}
void GuruLib::DataGrid::AddGroupUncollapsible(wchar_t* group_header, int index, AlignOrientation align)
{
	ListView_EnableGroupView(hwnd, TRUE);

	LVGROUP group;

	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_HEADER | LVGF_GROUPID;

	if (align != LEFT_SIDE)
	{
		group.mask |= LVGF_ALIGN;
		if (align == CENTER)
		{
			group.uAlign = LVGA_HEADER_CENTER;
		}
		else if (align == RIGHT_SIDE)
		{
			group.uAlign = LVGA_HEADER_RIGHT;
		}
	}

	group.pszHeader = group_header;
	group.iGroupId = index;
	ListView_InsertGroup(hwnd, -1, &group);
}
void GuruLib::DataGrid::MoveRowToGroup(int group_index, int row_index)
{
	LVITEMA Item = { 0 };

	Item.mask = LVIF_GROUPID;
	Item.iItem = row_index;
	Item.iGroupId = group_index;

	ListView_SetItem(hwnd, &Item);
}
GuruLib::DataGrid::DataGrid(HWND _hwnd)
{
	hwnd = _hwnd;
}