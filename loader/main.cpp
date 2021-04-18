#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <iostream>


HANDLE GetProcess()
{
	PROCESSENTRY32 peEntry{ sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0xBADC0DE);

	Process32First(hSnapshot, &peEntry);

	do
	{
		if (!_stricmp(peEntry.szExeFile, "DELTARUNE.exe") || !_stricmp(peEntry.szExeFile, "UNDERTALE.exe") || !_stricmp(peEntry.szExeFile, "TS!Underswap.exe"))
		{
			CloseHandle(hSnapshot);
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, peEntry.th32ProcessID);
		}
	} while (Process32Next(hSnapshot, &peEntry));

	CloseHandle(hSnapshot);
	return 0;
}

int main()
{
	SetConsoleTitleA("Project DELTA v2 Loader");

	std::cout << "WARNING: An auto-updating system has NOT yet been implemented." << std::endl;
	std::cout << "You NEED to have the LATEST loader to load the latest version of Project DELTA!" << std::endl << std::endl;

	std::cout << "Waiting for DELTARUNE.exe (if you renamed it, please rename it back).\n";

	HANDLE hProcess = 0;

	while (!(hProcess = GetProcess()))
	{
		Sleep(50);
	}

	Sleep(5000);

	if (!std::filesystem::exists(std::filesystem::current_path().string() + "\\ProjectDELTA.dll"))
	{
		std::cout << "Unable to inject: cannot find ProjectDELTA.dll (is it in this directory?)" << std::endl;
		std::cout << "Press any key to exit." << std::endl;
		std::cin.get();
		return 0;
	}

	std::cout << "Game and DLL found, injecting.\n";

	char DllName[MAX_PATH];
	GetFullPathNameA("ProjectDELTA.dll", MAX_PATH, DllName, NULL);

	LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

	LPVOID RemoteString = VirtualAllocEx(hProcess, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(hProcess, RemoteString, DllName, strlen(DllName), NULL);

	CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibrary, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(hProcess);

	std::cout << "Done. Auto-closing in 3 seconds.\n";

	Sleep(3000);

	return 0;
}