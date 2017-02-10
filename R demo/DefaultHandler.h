#pragma once
 
class DefaultHandler
{
protected:
	//default handlers for tab
	long OnTabSelectionChanging(unsigned int wParam, long lParam);
	//default handlers for datagrid
	long OnDataGridCellBeginEdit(unsigned int wParam, long lParam);
	long OnDataGridCellEndEdit(unsigned int wParam, long lParam);
};
