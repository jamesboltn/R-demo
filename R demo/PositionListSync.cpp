#pragma once
#include"PositionListSync.h"

bool operator==(const PositionCredential &pc1, const PositionCredential &pc2)
{
	return pc1.contract == pc2.contract &&
		pc1.direction == pc2.direction && pc1.expire_flag == pc2.expire_flag;
}
PositionCredential &PositionCredential::operator=(const PositionCredential &right)
{
	if (this != &right)
	{
		this->contract = right.contract;
		this->direction = right.direction;
		this->expire_flag = right.expire_flag;
	}
	return *this;
}
PositionListSync::iterator FindPositionFromSellBuy(const std::string &contract, PositionListSync* p)
{
	PositionCredential pc;
	pc.contract = contract;
	pc.direction = SELL;
	pc.expire_flag = NORMAL;
	auto it = p->find(pc);
	if (it == p->end())
	{
		pc.expire_flag = NEW;
		it = p->find(pc);
		if (it == p->end())
		{
			pc.direction = BUY;
			it = p->find(pc);
			if (it == p->end())
			{
				pc.expire_flag = NORMAL;
				return p->find(pc);
			}
			else
			{
				return it;
			}
		}
		else
		{
			return it;
		}
	}
	else
	{
		return it;
	}
}
PositionListSync::iterator FindPositionFromSell(const std::string &contract, PositionListSync* p)
{
	PositionCredential pc;
	pc.contract = contract;
	pc.direction = SELL;
	pc.expire_flag = NORMAL;
	auto it = p->find(pc);
	if (it == p->end())
	{
		pc.expire_flag = NEW;
		return p->find(pc);
	}
	else
	{
		return it;
	}
}
PositionListSync::iterator FindPositionFromBuy(const std::string &contract, PositionListSync* p)
{
	PositionCredential pc;
	pc.contract = contract;
	pc.direction = BUY;
	pc.expire_flag = NORMAL;
	auto it = p->find(pc);
	if (it == p->end())
	{
		pc.expire_flag = NEW;
		return p->find(pc);
	}
	else
	{
		return it;
	}
}
PositionListSync::iterator FindPositionFromNormalNew(PositionCredential* pc, PositionListSync* p)
{
	pc->expire_flag = NORMAL;
	auto it = p->find(*pc);
	if (it == p->end())
	{
		pc->expire_flag = NEW;
		return p->find(*pc);
	}
	else
	{
		return it;
	}
}
void PositionDetailSync::UpdateCurrentPrice(double price)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.3f", price);
	RefreshDataGrid(row_index, "current_price", buffer);
}
void PositionDetailSync::UpdateProfit(double _profit)
{
	profit = _profit;
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", _profit);
	RefreshDataGrid(row_index, "profit", buffer);
}
void PositionDetailSync::UpdateProfit()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", profit);
	RefreshDataGrid(row_index, "profit", buffer);
}
void PositionDetailSync::UpdateAveragePrice()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.3f", average_price);
	RefreshDataGrid(row_index, "average_price", buffer);
}
void PositionDetailSync::UpdateVolume()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", volume);
	RefreshDataGrid(row_index, "volume", buffer);
}
void PositionDetailSync::UpdateVolume(int _volume)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", volume = _volume);
	RefreshDataGrid(row_index, "volume", buffer);

}
void PositionDetailSync::UpdateDirection()
{
	if (pc != NULL)
	{
		if (pc->direction == SELL)
		{
			RefreshDataGrid(row_index, "direction", "sell");
		}
		else if (pc->direction == BUY)
		{
			RefreshDataGrid(row_index, "direction", "buy");
		}
	}
}
void PositionDetailSync::UpdateContract()
{
	if (pc != NULL)
	{
		RefreshDataGrid(row_index, "position_contract", pc->contract.c_str());
	}
}
void PositionDetailSync::UpdateMargin()
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", margin);
	RefreshDataGrid(row_index, "margin", buffer);
}
void PositionDetailSync::UpdateMargin(double _margin)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.2f", _margin);
	RefreshDataGrid(row_index, "margin", buffer);
}
int PositionDetailSync::AddDataGridRow()
{
	return row_index = DataBind::AddDataGridRow();
}

void PositionDetailSync::DeleteDataGridRow()
{
	DataBind::DeleteDataGridRow(row_index);
}
//void PositionDetailSync::UpdateContract(const char* contract)
//{
//	if (pc != NULL)
//	{
//		sprintf_s(buffer, ARRAYCOUNT(buffer), "%s", pc->contract.c_str());
//		RefreshDataGrid(row_index, "position_contract", buffer);
//	}
//}
PositionDetailSync::PositionDetailSync():
	pc(NULL)
{
}
PositionDetailSync::PositionDetailSync(const PositionCredential* _pc):
	pc(_pc)
{
}
void PositionDetailSync::UpdateAveragePrice(double _average_price)
{
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%.3f", average_price = _average_price);
	RefreshDataGrid(row_index, "average_price", buffer);
}
