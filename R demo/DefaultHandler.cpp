#pragma once
#include"preinclude.h"
#include"DefaultHandler.h"

long DefaultHandler::OnDataGridCellBeginEdit(unsigned int wParam, long lParam)
{
	return FALSE;
}

long DefaultHandler::OnDataGridCellEndEdit(unsigned int wParam, long lParam)
{
	LPNMLVDISPINFOW pdi((LPNMLVDISPINFOW)lParam);

	if (pdi->item.pszText)
	{
		ListView_SetItemText(pdi->hdr.hwndFrom, pdi->item.iItem, 0, pdi->item.pszText);// put new text
		return TRUE;
	}
	return FALSE;
}

long DefaultHandler::OnTabSelectionChanging(unsigned int wParam, long lParam)
{
	return FALSE;
}

long DefaultHandler::OnTreeViewSelectionChanging(unsigned int wParam, long lParam)
{
	return FALSE;
}