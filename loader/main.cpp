#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <iostream>


HANDLE GetDeltaruneProcess()
{
	PROCESSENTRY32 peEntry{ sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0xBADC0DE);

	Process32First(hSnapshot, &peEntry);

	do
	{
		if (_stricmp(peEntry.szExeFile, "DELTARUNE.exe") == 0)
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
	MessageBoxA(0,
		"An auto-updating system has NOT been implemented yet.\n\n"
		"You NEED to have the LATEST loader to load the latest version of Project DELTA.\n\n"
		"Also, do NOT download loaders from any other place than my GitHub (github.com/archie-osu/ProjectDelta)\n\n"
		"If you downloaded this from an unofficial source, I am NOT responsible for any damage that might happen.\n\n",
		"Warning", MB_ICONWARNING | MB_OK);

	std::cout << "WARNING: An auto-updating system has NOT yet been implemented." << std::endl;
	std::cout << "You NEED to have the LATEST loader to load the latest version of Project DELTA!" << std::endl << std::endl;

	std::cout << "Waiting for DELTARUNE.exe (if you renamed it, please rename it back).\n";

	HANDLE hProcess = 0;

	while (!(hProcess = GetDeltaruneProcess()))
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

	std::cout << "DELTARUNE and DLL found, injecting.\n";

	char DllName[MAX_PATH];
	GetFullPathNameA("ProjectDELTA.dll", MAX_PATH, DllName, NULL);

	LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

	// Allocate space in the process for our DLL name
	LPVOID RemoteString = VirtualAllocEx(hProcess, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated
	WriteProcessMemory(hProcess, RemoteString, DllName, strlen(DllName), NULL);

	// Load our DLL by calling loadlibrary in the other process and passing our dll name
	CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibrary, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(hProcess);

	std::cout << "Done. Auto-closing in 3 seconds.\n";

	Sleep(3000);

	return 0;
}