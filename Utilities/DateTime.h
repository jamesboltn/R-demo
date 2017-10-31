#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	class DateTime
	{
	private:
		std::time_t tm_t = std::time(0);
		struct std::tm* tm;
	public:
		LIB DateTime();
		LIB DateTime& Current();
		LIB DateTime& AddDays(int days);
		LIB int ToYearInt();
		LIB std::string ToYearString();
		LIB int ToMonthInt();
		LIB std::string ToMonthString();
		LIB int ToDateInt();
		LIB std::string ToDateString();
		LIB std::string GetYesterDateString();
		LIB std::string ToDayString();
		LIB std::string ToTimeString();
		LIB std::string ToDateTimeString();
		LIB std::string ToDateTimeStringFormat(const char* format) const;//For a complete set of formats, please visist http://www.cplusplus.com/reference/ctime/strftime/
		LIB std::string GetSpecificDateTimeString(std::time_t* ptr_timet);
		LIB std::time_t GetUnixElapseFromString(std::string* time_str);
	};
}
