#pragma once
#include"DateTime.h"
#include"Utilities.h"
#include"StringAssist.h"

GuruLib::DateTime::DateTime()
{
	tm = std::localtime(&tm_t);
}

std::string GuruLib::DateTime::ToTimeString()
{
	char hour_str[10];
	char min_str[10];
	char sec_str[10];

	if ((tm->tm_hour) < 10)
	{
		sprintf_s(hour_str, GuruLib::ArraySize(hour_str), "0%d", tm->tm_hour);
	}
	else
	{
		sprintf_s(hour_str, GuruLib::ArraySize(hour_str), "%d", tm->tm_hour);
	}

	if ((tm->tm_min) < 10)
	{
		sprintf_s(min_str, GuruLib::ArraySize(min_str), "0%d", tm->tm_min);
	}
	else
	{
		sprintf_s(min_str, GuruLib::ArraySize(min_str), "%d", tm->tm_min);
	}

	if ((tm->tm_sec) < 10)
	{
		sprintf_s(sec_str, GuruLib::ArraySize(sec_str), "0%d", tm->tm_sec);
	}
	else
	{
		sprintf_s(sec_str, GuruLib::ArraySize(sec_str), "%d", tm->tm_sec);
	}
	return std::string(hour_str) + ":" + min_str + ":" + sec_str;
}

std::string GuruLib::DateTime::ToDateTimeString()
{
	char year_str[5];
	char month_str[3];
	char day_str[10];
	char hour_str[3];
	char min_str[3];
	char sec_str[3];
	::sprintf_s(year_str, GuruLib::ArraySize(year_str), "%d", tm->tm_year + 1900);
	if ((tm->tm_mon + 1) < 10)
	{
		sprintf_s(month_str, GuruLib::ArraySize(month_str), "0%d", tm->tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, GuruLib::ArraySize(month_str), "%d", tm->tm_mon + 1);
	}

	if ((tm->tm_mday) < 10)
	{
		sprintf_s(day_str, GuruLib::ArraySize(day_str), "0%d", tm->tm_mday);
	}
	else
	{
		sprintf_s(day_str, GuruLib::ArraySize(day_str), "%d", tm->tm_mday);
	}

	if ((tm->tm_hour) < 10)
	{
		sprintf_s(hour_str, GuruLib::ArraySize(hour_str), "0%d", tm->tm_hour);
	}
	else
	{
		sprintf_s(hour_str, GuruLib::ArraySize(hour_str), "%d", tm->tm_hour);
	}

	if ((tm->tm_min) < 10)
	{
		sprintf_s(min_str, GuruLib::ArraySize(min_str), "0%d", tm->tm_min);
	}
	else
	{
		sprintf_s(min_str, GuruLib::ArraySize(min_str), "%d", tm->tm_min);
	}

	if ((tm->tm_sec) < 10)
	{
		sprintf_s(sec_str, GuruLib::ArraySize(sec_str), "0%d", tm->tm_sec);
	}
	else
	{
		sprintf_s(sec_str, GuruLib::ArraySize(sec_str), "%d", tm->tm_sec);
	}
	return std::string(year_str) + "-" + month_str + "-" + day_str + " " + hour_str + ":" + min_str + ":" + sec_str;
}

std::string GuruLib::DateTime::ToDateTimeStringFormat(const char* format) const
{
	char buffer[255];
	std::strftime(buffer, GuruLib::ArraySize(buffer), format, tm);
	//stime = tAll;
	return buffer;
}

std::string GuruLib::DateTime::GetYesterDateString()
{
	tm_t = std::time(NULL) - (60 * 60 * 24);
	tm = std::localtime(&tm_t);
	char year_str[10];
	char month_str[10];
	char day_str[10];
	sprintf_s(year_str, ArraySize(year_str), "%d", tm->tm_year + 1900);
	if ((tm->tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ArraySize(month_str), "0%d", tm->tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ArraySize(month_str), "%d", tm->tm_mon + 1);
	}

	if ((tm->tm_mday) < 10)
	{
		sprintf_s(day_str, ArraySize(day_str), "0%d", tm->tm_mday);
	}
	else
	{
		sprintf_s(day_str, ArraySize(day_str), "%d", tm->tm_mday);
	}

	return std::string(year_str) + "-" + std::string(month_str) + "-" + std::string(day_str);
}

std::string GuruLib::DateTime::ToDateString()
{
	char year_str[10];
	char month_str[10];
	char day_str[10];
	sprintf_s(year_str, ArraySize(year_str), "%d", tm->tm_year + 1900);
	if ((tm->tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ArraySize(month_str), "0%d", tm->tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ArraySize(month_str), "%d", tm->tm_mon + 1);
	}

	if ((tm->tm_mday) < 10)
	{
		sprintf_s(day_str, ArraySize(day_str), "0%d", tm->tm_mday);
	}
	else
	{
		sprintf_s(day_str, ArraySize(day_str), "%d", tm->tm_mday);
	}

	return std::string(year_str) + "-" + std::string(month_str) + "-" + std::string(day_str);
}

std::string GuruLib::DateTime::GetSpecificDateTimeString(std::time_t* ptr_timet)
{
	char year_str[10];
	char month_str[10];
	char day_str[10];
	char hour_str[10];
	char min_str[10];
	char sec_str[10];
	tm = std::localtime(ptr_timet);
	sprintf_s(year_str, GuruLib::ArraySize(year_str), "%d", tm->tm_year + 1900);
	if ((tm->tm_mon + 1) < 10)
	{
		sprintf_s(month_str, GuruLib::ArraySize(month_str), "0%d", tm->tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, GuruLib::ArraySize(month_str), "%d", tm->tm_mon + 1);
	}

	if ((tm->tm_mday) < 10)
	{
		sprintf_s(day_str, GuruLib::ArraySize(day_str), "0%d", tm->tm_mday);
	}
	else
	{
		sprintf_s(day_str, GuruLib::ArraySize(day_str), "%d", tm->tm_mday);
	}

	if ((tm->tm_hour) < 10)
	{
		sprintf_s(hour_str, GuruLib::ArraySize(hour_str), "0%d", tm->tm_hour);
	}
	else
	{
		sprintf_s(hour_str, GuruLib::ArraySize(hour_str), "%d", tm->tm_hour);
	}

	if ((tm->tm_min) < 10)
	{
		sprintf_s(min_str, GuruLib::ArraySize(min_str), "0%d", tm->tm_min);
	}
	else
	{
		sprintf_s(min_str, GuruLib::ArraySize(min_str), "%d", tm->tm_min);
	}

	if ((tm->tm_sec) < 10)
	{
		sprintf_s(sec_str, GuruLib::ArraySize(sec_str), "0%d", tm->tm_sec);
	}
	else
	{
		sprintf_s(sec_str, GuruLib::ArraySize(sec_str), "%d", tm->tm_sec);
	}
	return std::string(year_str) + "-" + std::string(month_str) + "-" + std::string(day_str) + " " + std::string(hour_str) + ":" + std::string(min_str) + ":" + std::string(sec_str);
}

time_t GuruLib::DateTime::GetUnixElapseFromString(std::string* time_str)
{
	std::vector<std::string>date_time;
	std::vector<std::string>year_mon_day;
	std::vector<std::string>hour_min_sec;
	GuruLib::StdString::Split(*time_str, ' ', date_time);
	GuruLib::StdString::Split(date_time[0], '-', year_mon_day);
	GuruLib::StdString::Split(date_time[1], ':', hour_min_sec);

	tm->tm_year = atoi(year_mon_day[0].c_str()) - 1900;
	tm->tm_mon = atoi(year_mon_day[1].c_str()) - 1;
	tm->tm_mday = atoi(year_mon_day[2].c_str());
	tm->tm_hour = atoi(hour_min_sec[0].c_str());
	tm->tm_min = atoi(hour_min_sec[1].c_str());
	tm->tm_sec = atoi(hour_min_sec[2].c_str());

	return mktime(tm);
}

int GuruLib::DateTime::ToMonthInt()
{
	return tm->tm_mon + 1;
}

int GuruLib::DateTime::ToYearInt()
{
	return tm->tm_year + 1900;
}

std::string GuruLib::DateTime::ToYearString()
{
	char year_str[10];;
	sprintf_s(year_str, ArraySize(year_str), "%d", tm->tm_year + 1900);
	return std::string(year_str);
}

int GuruLib::DateTime::ToDateInt()
{
	return (tm->tm_year + 1900) * 10000 + (tm->tm_mon + 1) * 100 + tm->tm_mday;
}

std::string GuruLib::DateTime::ToDayString()
{
	char day_str[10];

	if ((tm->tm_mday) < 10)
	{
		sprintf_s(day_str, GuruLib::ArraySize(day_str), "0%d", tm->tm_mday);
	}
	else
	{
		sprintf_s(day_str, GuruLib::ArraySize(day_str), "%d", tm->tm_mday);
	}

	return std::string(day_str);
}

std::string GuruLib::DateTime::ToMonthString()
{
	char month_str[10];

	if ((tm->tm_mon + 1) < 10)
	{
		sprintf_s(month_str, GuruLib::ArraySize(month_str), "0%d", tm->tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, GuruLib::ArraySize(month_str), "%d", tm->tm_mon + 1);
	}

	return std::string(month_str);
}

GuruLib::DateTime& GuruLib::DateTime::Current()
{
	tm_t = std::time(0);
	tm = std::localtime(&tm_t);
	return *this;
}

GuruLib::DateTime& GuruLib::DateTime::AddDays(int days)
{
	tm_t += 24 * 60 * 60 * days;
	tm = std::localtime(&tm_t);
	return *this;
}