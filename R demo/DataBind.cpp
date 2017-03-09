#pragma once
#include"DataBind.h"
#include"UIBase.h"
#include"ControlBase.h"
#include"Button.h"
#include"CheckBox.h"
#include"ComboBox.h"
#include"DataGrid.h"
#include"DateTimePicker.h"
#include"Edit.h"
#include"GroupBox.h"
#include"ListBox.h"
#include"RadioButton.h"
#include"ScrollBar.h"
#include"Static.h"

DataGrid* DataBind::bound_datagrid = NULL;

void DataBind::BindControl(unsigned int control_count, ...)
{
	va_list list1, list2;
	va_start(list1, control_count);
	va_copy(list2,list1);

	UIBase* uibase_ptr;
	ControlBase* controlbase_ptr;
	for (unsigned int arg = 0; arg < control_count; ++arg)
	{
		controlbase_ptr = va_arg(list1, ControlBase*);
		if (controlbase_ptr->unique_name == "")
		{
			continue;
		}

		uibase_ptr = va_arg(list2, UIBase*);
		if (typeid(*uibase_ptr) == typeid(Button))//it is a Button control
		{
				bound_controls[controlbase_ptr->unique_name]= new Button(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(CheckBox))//it is a CheckBox control
		{
			bound_controls[controlbase_ptr->unique_name] = new CheckBox(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(ComboBox))//it is a ComboBox control
		{
			bound_controls[controlbase_ptr->unique_name] =new ComboBox(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(DataGrid))//it is a DataGrid control
		{
			if (bound_datagrid != NULL)
			{
				continue;
			}
			bound_datagrid = new DataGrid(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(DateTimePicker))//it is a DateTimePicker control
		{
			bound_controls[controlbase_ptr->unique_name] = new DateTimePicker(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(Edit))//it is an Edit control
		{
			bound_controls[controlbase_ptr->unique_name] = new Edit(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(EditWithLog))//it is an EditWithLog control
		{
			bound_controls[controlbase_ptr->unique_name] = new EditWithLog(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(GroupBox))//it is a GroupBox control
		{
			bound_controls[controlbase_ptr->unique_name] = new GroupBox(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(ListBox))//it is a ListBox control
		{
			bound_controls[controlbase_ptr->unique_name] = new ListBox(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(RadioButton))//it is a RadioButton control
		{
			bound_controls[controlbase_ptr->unique_name] = new RadioButton(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(ScrollBar))//it is a ScrollBar control
		{
			bound_controls[controlbase_ptr->unique_name] = new ScrollBar(uibase_ptr->GetHWND());
		}
		else if (typeid(*uibase_ptr) == typeid(Static))//it is a Static control
		{
			bound_controls[controlbase_ptr->unique_name] = new Static(uibase_ptr->GetHWND());
		}
	}
	// Cleanup the va_list when we're done.
	va_end(list1);
	va_end(list2);
}

void DataBind::BindControl(UIBase* uibase_ptr)
{
	if (typeid(*uibase_ptr) == typeid(Button))//it is a Button control
	{
		if (Button* button_ptr = dynamic_cast<Button*>(uibase_ptr))
		{
			if (button_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[button_ptr->unique_name] = new Button(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(CheckBox))//it is a CheckBox control
	{
		if (CheckBox* checkbox_ptr = dynamic_cast<CheckBox*>(uibase_ptr))
		{
			if (checkbox_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[checkbox_ptr->unique_name] = new CheckBox(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(ComboBox))//it is a ComboBox control
	{
		if (ComboBox* combobox_ptr = dynamic_cast<ComboBox*>(uibase_ptr))
		{
			if (combobox_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[combobox_ptr->unique_name] = new ComboBox(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(DataGrid))//it is a DataGrid control
	{
		if (bound_datagrid != NULL)
		{
			return;//only one datagrid is allowed to be bound
		}

		if (DataGrid* datagrid_ptr = dynamic_cast<DataGrid*>(uibase_ptr))
		{
			bound_datagrid = new DataGrid(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(DateTimePicker))//it is a DateTimePicker control
	{
		if (DateTimePicker* dtpicker_ptr = dynamic_cast<DateTimePicker*>(uibase_ptr))
		{
			if (dtpicker_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[dtpicker_ptr->unique_name] = new DateTimePicker(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(Edit))//it is an Edit control
	{
		if (Edit* edit_ptr = dynamic_cast<Edit*>(uibase_ptr))
		{
			if (edit_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[edit_ptr->unique_name] = new Edit(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(EditWithLog))//it is an EditWithLog control
	{
		if (EditWithLog* edit_ptr = dynamic_cast<EditWithLog*>(uibase_ptr))
		{
			if (edit_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[edit_ptr->unique_name] = new EditWithLog(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(GroupBox))//it is a GroupBox control
	{
		if (GroupBox* groupbox_ptr = dynamic_cast<GroupBox*>(uibase_ptr))
		{
			if (groupbox_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[groupbox_ptr->unique_name] = new GroupBox(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(ListBox))//it is a ListBox control
	{
		if (ListBox* listbox_ptr = dynamic_cast<ListBox*>(uibase_ptr))
		{
			if (listbox_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[listbox_ptr->unique_name] = new ListBox(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(RadioButton))//it is a RadioButton control
	{
		if (RadioButton* radiobutton_ptr = dynamic_cast<RadioButton*>(uibase_ptr))
		{
			if (radiobutton_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[radiobutton_ptr->unique_name] = new RadioButton(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(ScrollBar))//it is a ScrollBar control
	{
		if (ScrollBar* scrollbar_ptr = dynamic_cast<ScrollBar*>(uibase_ptr))
		{
			if (scrollbar_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[scrollbar_ptr->unique_name] = new ScrollBar(uibase_ptr->GetHWND());
		}
	}
	else if (typeid(*uibase_ptr) == typeid(Static))//it is a Static control
	{
		if (Static* static_ptr = dynamic_cast<Static*>(uibase_ptr))
		{
			if (static_ptr->unique_name == "")
			{
				return;
			}
			bound_controls[static_ptr->unique_name] = new Static(uibase_ptr->GetHWND());
		}
	}
}

DataBind::~DataBind()
{
	for (auto it = bound_controls.begin(); it != bound_controls.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	if (bound_datagrid != NULL)
	{
		delete bound_datagrid;
		bound_datagrid = NULL;
	}
}

DataBind::DataBind()
{

}

void DataBind::RefreshControl(char* control_name, const char* value)
{
	auto it = bound_controls.find(control_name);
	if (it != bound_controls.end())
	{
		
		if (typeid(*(it->second)) == typeid(Static))//it is a Static control
		{
			if (Static* static_ptr = dynamic_cast<Static*>(it->second))
			{
				static_ptr->SetTextA(value);
			}
		}
		if (typeid(*(it->second)) == typeid(EditWithLog))//it is an EditWithLog control
		{
			if (EditWithLog* edit_ptr = dynamic_cast<EditWithLog*>(it->second))
			{
				edit_ptr->WriteLog(value);
			}
		}
	}
}

void DataBind::RefreshDataGrid(int row_index, char* col_name, const char* value)
{
	if (bound_datagrid != NULL)
	{
		bound_datagrid->InsertCellA(row_index, std::string(col_name), value);
	}
}

int DataBind::AddDataGridRow()
{
	if (bound_datagrid != NULL)
	{
		return bound_datagrid->AddRow();
	}
	else
	{
		return -1;
	}
}

void DataBind::DeleteDataGridRow(int row_index)
{
	if (bound_datagrid != NULL)
	{
		bound_datagrid->DeleteRow(row_index);
	}
}