#pragma once
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

int GuruLib::ControlBase::_resource_id = 1023;
GuruLib::ControlBase::ControlBase():
	resource_id(++_resource_id)
{
}
GuruLib::ControlBase::~ControlBase()
{
}
void GuruLib::ControlBase::IncrementResourceID()
{
	resource_id = ++_resource_id;
}