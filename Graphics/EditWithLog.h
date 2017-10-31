#pragma once
#include"Edit.h"

namespace GuruLib
{
	class EditWithLog :
		public Edit
	{
	private:
		FILE* fp = NULL;
	public:
		std::string log_path;
		EditWithLog() {}
		virtual ~EditWithLog() {};
		EditWithLog(int x_pos, int y_pos, int width, int height, UnitType type = PIXEL);
		explicit EditWithLog(HWND _hwnd);//accepts HWND of created EditWithLog

										 //write msg to edit and local disk successively
		void WriteLog(const char* msg, const char* folder_path = NULL);
		//write msg to local disk only
		void WriteLogLocal(const char* msg, const char* folder_path = NULL);
	};
}

