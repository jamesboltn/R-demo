#pragma once
#include"preinclude.h"
#include"Utilities.h"

int MachineInfo::GetScreenHeight()
{
	return ::GetSystemMetrics(SM_CYFULLSCREEN);
}

int MachineInfo::GetScreenWidth()
{
	return ::GetSystemMetrics(SM_CXFULLSCREEN);
}