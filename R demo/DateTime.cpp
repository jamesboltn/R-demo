#pragma once
#include"preinclude.h"
#include"Utilities.h"

std::string DateTime::GetCurrentTimeString()
{
	time_t now(::time(0));
	char hour_str[10];
	char min_str[10];
	char sec_str[10];
	struct tm tm;
	localtime_s(&tm, &now);

	if ((tm.tm_hour) < 10)
	{
		sprintf_s(hour_str, ARRAYCOUNT(hour_str), "0%d", tm.tm_hour);
	}
	else
	{
		sprintf_s(hour_str, ARRAYCOUNT(hour_str), "%d", tm.tm_hour);
	}

	if ((tm.tm_min) < 10)
	{
		sprintf_s(min_str, ARRAYCOUNT(min_str), "0%d", tm.tm_min);
	}
	else
	{
		sprintf_s(min_str, ARRAYCOUNT(min_str), "%d", tm.tm_min);
	}

	if ((tm.tm_sec) < 10)
	{
		sprintf_s(sec_str, ARRAYCOUNT(sec_str), "0%d", tm.tm_sec);
	}
	else
	{
		sprintf_s(sec_str, ARRAYCOUNT(sec_str), "%d", tm.tm_sec);
	}
	return std::string(hour_str) + ":" + min_str + ":" + sec_str;
}

std::string DateTime::GetCurrentDateTimeString()
{
	time_t now(::time(0));
	char year_str[5];
	char month_str[3];
	char day_str[10];
	char hour_str[3];
	char min_str[3];
	char sec_str[3];
	struct tm tm;
	localtime_s(&tm, &now);
	::sprintf_s(year_str, ARRAYCOUNT(year_str), "%d", tm.tm_year + 1900);
	if ((tm.tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "0%d", tm.tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "%d", tm.tm_mon + 1);
	}

	if ((tm.tm_mday) < 10)
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "0%d", tm.tm_mday);
	}
	else
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "%d", tm.tm_mday);
	}

	if ((tm.tm_hour) < 10)
	{
		sprintf_s(hour_str, ARRAYCOUNT(hour_str), "0%d", tm.tm_hour);
	}
	else
	{
		sprintf_s(hour_str, ARRAYCOUNT(hour_str), "%d", tm.tm_hour);
	}

	if ((tm.tm_min) < 10)
	{
		sprintf_s(min_str, ARRAYCOUNT(min_str), "0%d", tm.tm_min);
	}
	else
	{
		sprintf_s(min_str, ARRAYCOUNT(min_str), "%d", tm.tm_min);
	}

	if ((tm.tm_sec) < 10)
	{
		sprintf_s(sec_str, ARRAYCOUNT(sec_str), "0%d", tm.tm_sec);
	}
	else
	{
		sprintf_s(sec_str, ARRAYCOUNT(sec_str), "%d", tm.tm_sec);
	}
	return std::string(year_str) + "-" + month_str + "-" + day_str + " " + hour_str + ":" + min_str + ":" + sec_str;
}

std::string DateTime::GetYesterDateString()
{
	time_t yesterday = time(NULL) - (60 * 60 * 24);
	struct tm tm;
	localtime_s(&tm, &yesterday);
	char year_str[10];
	char month_str[10];
	char day_str[10];
	sprintf_s(year_str, ARRAYCOUNT(year_str), "%d", tm.tm_year + 1900);
	if ((tm.tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "0%d", tm.tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "%d", tm.tm_mon + 1);
	}

	if ((tm.tm_mday) < 10)
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "0%d", tm.tm_mday);
	}
	else
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "%d", tm.tm_mday);
	}

	return std::string(year_str) + "-" + std::string(month_str) + "-" + std::string(day_str);
}

std::string DateTime::GetCurrentDateString()
{
	time_t now = time(NULL);
	char year_str[10];
	char month_str[10];
	char day_str[10];
	struct tm tm;
	localtime_s(&tm, &now);
	sprintf_s(year_str, ARRAYCOUNT(year_str), "%d", tm.tm_year + 1900);
	if ((tm.tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "0%d", tm.tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "%d", tm.tm_mon + 1);
	}

	if ((tm.tm_mday) < 10)
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "0%d", tm.tm_mday);
	}
	else
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "%d", tm.tm_mday);
	}

	return std::string(year_str) + "-" + std::string(month_str) + "-" + std::string(day_str);
}

std::string DateTime::GetSpecificDateTimeString(time_t* ptr_timet)
{
	char year_str[10];
	char month_str[10];
	char day_str[10];
	char hour_str[10];
	char min_str[10];
	char sec_str[10];
	struct tm tm;
	localtime_s(&tm, ptr_timet);
	sprintf_s(year_str, ARRAYCOUNT(year_str), "%d", tm.tm_year + 1900);
	if ((tm.tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "0%d", tm.tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "%d", tm.tm_mon + 1);
	}

	if ((tm.tm_mday) < 10)
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "0%d", tm.tm_mday);
	}
	else
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "%d", tm.tm_mday);
	}

	if ((tm.tm_hour) < 10)
	{
		sprintf_s(hour_str, ARRAYCOUNT(hour_str), "0%d", tm.tm_hour);
	}
	else
	{
		sprintf_s(hour_str, ARRAYCOUNT(hour_str), "%d", tm.tm_hour);
	}

	if ((tm.tm_min) < 10)
	{
		sprintf_s(min_str, ARRAYCOUNT(min_str), "0%d", tm.tm_min);
	}
	else
	{
		sprintf_s(min_str, ARRAYCOUNT(min_str), "%d", tm.tm_min);
	}

	if ((tm.tm_sec) < 10)
	{
		sprintf_s(sec_str, ARRAYCOUNT(sec_str), "0%d", tm.tm_sec);
	}
	else
	{
		sprintf_s(sec_str, ARRAYCOUNT(sec_str), "%d", tm.tm_sec);
	}
	return std::string(year_str) + "-" + std::string(month_str) + "-" + std::string(day_str) + " " + std::string(hour_str) + ":" + std::string(min_str) + ":" + std::string(sec_str);
}

time_t DateTime::GetUnixElapseFromString(std::string* time_str)
{
	std::vector<std::string>date_time;
	std::vector<std::string>year_mon_day;
	std::vector<std::string>hour_min_sec;
	String::Split(time_str, ' ', &date_time);
	String::Split(&date_time[0], '-', &year_mon_day);
	String::Split(&date_time[1], ':', &hour_min_sec);

	struct tm ttm = { 0 };
	ttm.tm_year = atoi(year_mon_day[0].c_str()) - 1900;
	ttm.tm_mon = atoi(year_mon_day[1].c_str()) - 1;
	ttm.tm_mday = atoi(year_mon_day[2].c_str());
	ttm.tm_hour = atoi(hour_min_sec[0].c_str());
	ttm.tm_min = atoi(hour_min_sec[1].c_str());
	ttm.tm_sec = atoi(hour_min_sec[2].c_str());

	return mktime(&ttm);
}

int DateTime::GetCurrentMonthInt()
{
	time_t now(::time(0));
	struct tm tm;
	localtime_s(&tm, &now);
	return tm.tm_mon + 1;
}

int DateTime::GetCurrentYearInt()
{
	time_t now(::time(0));
	struct tm tm;
	localtime_s(&tm, &now);
	return tm.tm_year + 1900;
}

std::string DateTime::GetCurrentYearString()
{
	time_t now = time(NULL);
	char year_str[10];;
	struct tm tm;
	localtime_s(&tm, &now);
	sprintf_s(year_str, ARRAYCOUNT(year_str), "%d", tm.tm_year + 1900);
	return std::string(year_str);
}

int DateTime::GetCurrentDateInt()
{
	time_t now(::time(0));
	struct tm tm;
	localtime_s(&tm, &now);
	return (tm.tm_year + 1900) * 10000 + (tm.tm_mon + 1) * 100 + tm.tm_mday;
}

std::string DateTime::GetCurrentDayString()
{
	time_t now = time(NULL);
	char day_str[10];
	struct tm tm;
	localtime_s(&tm, &now);
	
	if ((tm.tm_mday) < 10)
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "0%d", tm.tm_mday);
	}
	else
	{
		sprintf_s(day_str, ARRAYCOUNT(day_str), "%d", tm.tm_mday);
	}

	return std::string(day_str);
}

std::string DateTime::GetCurrentMonthString()
{
	time_t now = time(NULL);
	char month_str[10];
	struct tm tm;
	localtime_s(&tm, &now);

	if ((tm.tm_mon + 1) < 10)
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "0%d", tm.tm_mon + 1);
	}
	else
	{
		sprintf_s(month_str, ARRAYCOUNT(month_str), "%d", tm.tm_mon + 1);
	}

	return std::string(month_str);
}