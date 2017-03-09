#pragma once
#include"ControlBase.h"

int ControlBase::_resource_id = 1023;
ControlBase::ControlBase():
	resource_id(++_resource_id)
{
}

void ControlBase::IncrementResourceID()
{
	resource_id = ++_resource_id;
}