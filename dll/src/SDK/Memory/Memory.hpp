#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // >needing to include an entire header just for one type
#include <string> //Greatly simplifies converting from WCHAR to const char*
namespace Memory
{
	std::string GetCurrentProcessName();
	HWND GetCurrentWindow();
}