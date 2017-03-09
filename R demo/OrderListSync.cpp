#pragma once
#include"OrderListSync.h"

bool operator==(const OrderCredential &oc1, const OrderCredential &oc2)
{
	return oc1.front_id == oc2.front_id &&
		oc1.session_id == oc2.session_id && oc1.order_ref == oc2.order_ref;
}
void OrderDetailSync::UpdateContract()
{
	if (oc != NULL)
	{
		RefreshDataGrid(row_index, "order_contract", oc->contract.c_str());
	}
}
void OrderDetailSync::UpdateDirection()
{
	if (direction == SELL)
	{
		RefreshDataGrid(row_index, "direction", "sell");
	}
	else if (direction == BUY)
	{
		RefreshDataGrid(row_index, "direction", "buy");
	}
}
void OrderDetailSync::UpdateOpenFlag()
{
	if (open_flag == OPEN)
	{
		RefreshDataGrid(row_index, "open_flag", "open");
	}
	else if (open_flag == CLOSE)
	{
		RefreshDataGrid(row_index, "open_flag", "close");
	} 
	else if (open_flag == FORCE_CLOSE)
	{
		RefreshDataGrid(row_index, "open_flag", "force_close");
	}
	else if (open_flag == FORCE_OFF)
	{
		RefreshDataGrid(row_index, "open_flag", "force_off");
	}
	else if (open_flag == LOCAL_FORCE_CLOSE)
	{
		RefreshDataGrid(row_index, "open_flag", "local_force_close");
	}
}
void OrderDetailSync::UpdateStatus()
{
	RefreshDataGrid(row_index, "status", status.c_str());
}
void OrderDetailSync::UpdateVolume()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", volume);
	RefreshDataGrid(row_index, "volume", buffer);
}
void OrderDetailSync::UpdatePrice()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", price);
	RefreshDataGrid(row_index, "price", buffer);
}
void OrderDetailSync::UpdateInsertTime()
{
	RefreshDataGrid(row_index, "insert_time", insert_time.c_str());
}
void OrderDetailSync::UpdateTradeVolume()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", trade_volume);
	RefreshDataGrid(row_index, "trade_volume", buffer);
}
void OrderDetailSync::UpdateTradeVolume(int _trade_volume)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", trade_volume = _trade_volume);
	RefreshDataGrid(row_index, "trade_volume", buffer);
}
void OrderDetailSync::UpdateExchange()
{
	if (oc != NULL)
	{
		RefreshDataGrid(row_index, "exchange", oc->exchange.c_str());
	}	
}
void OrderDetailSync::UpdateFrontID()
{
	if (oc != NULL)
	{
		sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", oc->front_id);
		RefreshDataGrid(row_index, "front_id", buffer);
	}
}
void OrderDetailSync::UpdateSessionID()
{
	if (oc != NULL)
	{
		sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", oc->session_id);
		RefreshDataGrid(row_index, "session_id", buffer);
	}
}
void OrderDetailSync::UpdateOrderRef()
{
	if (oc != NULL)
	{
		RefreshDataGrid(row_index, "order_ref", oc->order_ref.c_str());
	}	
}
void OrderDetailSync::UpdateStatus(const char* _status)
{
	status = _status;
	RefreshDataGrid(row_index, "status", _status);
}
int OrderDetailSync::AddDataGridRow()
{
	return row_index = DataBind::AddDataGridRow();
}
void OrderDetailSync::UpdateVolume(int _volume)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", volume = _volume);
	RefreshDataGrid(row_index, "volume", buffer);
}
void OrderDetailSync::UpdatePrice(double _price)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", price = _price);
	RefreshDataGrid(row_index, "price", buffer);
}
void OrderDetailSync::UpdateInsertTime(const std::string &_insert_time)
{
	insert_time = _insert_time;
	RefreshDataGrid(row_index, "insert_time", _insert_time.c_str());
}
void OrderDetailSync::UpdateStatus(const std::string &_status)
{
	status = _status;
	RefreshDataGrid(row_index, "status", status.c_str());
}









