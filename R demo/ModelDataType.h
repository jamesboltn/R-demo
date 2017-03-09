#pragma once
#include"preinclude.h"
typedef bool(*DatabaseErrorMessageHandler)(const std::string &msg);
typedef bool(*AccountSpecificMsgHandler)(const std::string &msg, const char* page_name);