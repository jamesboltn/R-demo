#pragma once
#include"EditWithLog.h"
#include"DateTime.h"
#include"FileSystem.h"

GuruLib::EditWithLog::EditWithLog(int x_pos, int y_pos, int _width, int _height, UnitType type)
{
	extended_style_flag |= WS_EX_CLIENTEDGE;
	window_class_name = L"EDIT";
	style_flag = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL;

	y = y_pos;
	x = x_pos;
	height = _height;
	width = _width;
	hMenu = HMENU(resource_id);

	_CreateWindow(type);

	if (hwnd != NULL)
	{
		Edit_SetReadOnly(hwnd, TRUE);
	}
}
void GuruLib::EditWithLog::WriteLog(const char* msg, const char* folder_path)
{
	DateTime dt;
	if (fp == NULL)
	{
		if (folder_path == NULL)
		{
			if (log_path == "")
			{
				log_path = "./Log/" + dt.Current().ToDateString() + ".log";
			}
			else
			{
				log_path += dt.Current().ToDateString() + ".log";
			}
		}
		else
		{
			log_path = folder_path + dt.Current().ToDateString() + ".log";
		}
		fopen_s(&fp, log_path.c_str(), "a");
	}

	std::string time_stamp(dt.Current().ToTimeString());
	std::string record(time_stamp + " " + msg);
	AppendLineA(record.c_str(), false);
	FileSystem::File::WriteLog(msg, fp, &time_stamp);
}
void GuruLib::EditWithLog::WriteLogLocal(const char* msg, const char* folder_path)
{
	DateTime dt;
	if (fp == NULL)
	{
		if (folder_path == NULL)
		{
			if (log_path == "")
			{
				log_path = "./Log/" + dt.Current().ToDateString() + ".log";
			}
			else
			{
				log_path += dt.Current().ToDateString() + ".log";
			}
		}
		else
		{
			log_path = folder_path + dt.Current().ToDateString() + ".log";
		}
		fopen_s(&fp, log_path.c_str(), "a");
	}
	std::string time_stamp = dt.Current().ToTimeString();
	FileSystem::File::WriteLog(msg, fp, &time_stamp);
}
GuruLib::EditWithLog::EditWithLog(HWND _hwnd)
{
	hwnd = _hwnd;
}