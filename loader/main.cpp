#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <iostream>

#define ReCa reinterpret_cast

uintptr_t GetModule(DWORD procId, const char* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnapshot, &modEntry))
		{
			do
			{
				if (!_stricmp(modEntry.szModule, modName))
				{
					CloseHandle(hSnapshot);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnapshot, &modEntry));
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

//Thank you Rake and iPower from GuidedHacking!
uintptr_t GetProcAddressEx(HANDLE hProcess, DWORD pid, const char* module, const char* function)
{
	if (!module || !function || !pid || !hProcess)
		return 0;

	uintptr_t moduleBase = GetModule(pid, module);

	if (!moduleBase)
		return 0;

	IMAGE_DOS_HEADER Image_Dos_Header = { 0 };

	if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(moduleBase), &Image_Dos_Header, sizeof(IMAGE_DOS_HEADER), nullptr))
		return 0;

	if (Image_Dos_Header.e_magic != IMAGE_DOS_SIGNATURE)
		return 0;

	IMAGE_NT_HEADERS Image_Nt_Headers = { 0 };

	if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(moduleBase + Image_Dos_Header.e_lfanew), &Image_Nt_Headers, sizeof(IMAGE_NT_HEADERS), nullptr))
		return 0;

	if (Image_Nt_Headers.Signature != IMAGE_NT_SIGNATURE)
		return 0;

	IMAGE_EXPORT_DIRECTORY Image_Export_Directory = { 0 };
	uintptr_t img_exp_dir_rva = 0;

	if (!(img_exp_dir_rva = Image_Nt_Headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress))
		return 0;

	if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(moduleBase + img_exp_dir_rva), &Image_Export_Directory, sizeof(IMAGE_EXPORT_DIRECTORY), nullptr))
		return 0;

	uintptr_t EAT = moduleBase + Image_Export_Directory.AddressOfFunctions;
	uintptr_t ENT = moduleBase + Image_Export_Directory.AddressOfNames;
	uintptr_t EOT = moduleBase + Image_Export_Directory.AddressOfNameOrdinals;

	WORD ordinal = 0;
	SIZE_T len_buf = strlen(function) + 1;
	char* temp_buf = new char[len_buf];

	for (size_t i = 0; i < Image_Export_Directory.NumberOfNames; i++)
	{
		uintptr_t tempRvaString = 0;

		if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(ENT + (i * sizeof(uintptr_t))), &tempRvaString, sizeof(uintptr_t), nullptr))
			return 0;

		if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(moduleBase + tempRvaString), temp_buf, len_buf, nullptr))
			return 0;

		if (!lstrcmpi(function, temp_buf))
		{
			if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(EOT + (i * sizeof(WORD))), &ordinal, sizeof(WORD), nullptr))
				return 0;

			uintptr_t temp_rva_func = 0;

			if (!ReadProcessMemory(hProcess, ReCa<LPCVOID>(EAT + (ordinal * sizeof(uintptr_t))), &temp_rva_func, sizeof(uintptr_t), nullptr))
				return 0;

			delete[] temp_buf;
			return moduleBase + temp_rva_func;
		}
	}
	delete[] temp_buf;
	return 0;
}

struct DeltaArgs_t
{
	char DllPath[MAX_PATH];
	int DXVer; //11 = DirectX11, 9 = DirectX9, you get it.
};

HANDLE GetProcess()
{
	PROCESSENTRY32 peEntry{ sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0xBADC0DE);

	Process32First(hSnapshot, &peEntry);

	do
	{
		if (!_stricmp(peEntry.szExeFile, "DELTARUNE.exe") 
			|| !_stricmp(peEntry.szExeFile, "UNDERTALE.exe") 
			|| !_stricmp(peEntry.szExeFile, "TS!Underswap.exe")
		)
		{
			CloseHandle(hSnapshot);
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, peEntry.th32ProcessID);
		}
	} while (Process32Next(hSnapshot, &peEntry));

	CloseHandle(hSnapshot);
	return 0;
}

//Returns the DirectX version as an integer
int GetDXApi(HANDLE hProc)
{
	MODULEENTRY32 mEntry{ sizeof(MODULEENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(hProc));

	Module32First(hSnapshot, &mEntry);

	do
	{
		if (!_stricmp(mEntry.szModule, "d3d9.dll"))
		{
			CloseHandle(hSnapshot);
			return 9;
		}

		else if (!_stricmp(mEntry.szModule, "d3d10.dll"))
		{
			CloseHandle(hSnapshot);
			return 10;
		}

		else if (!_stricmp(mEntry.szModule, "d3d11.dll"))
		{
			CloseHandle(hSnapshot);
			return 11;
		}

	} while (Module32Next(hSnapshot, &mEntry));

	printf("[Warn] Failed to get the DirectX version, falling back to DX11.\n");

	return 11;
}

int main()
{
	SetConsoleTitleA("Project DELTA Loader");

	HANDLE hProcess = 0;
	DeltaArgs_t Args;
	std::string CurrentPath = std::filesystem::current_path().string();
	strcpy_s<MAX_PATH>(Args.DllPath, std::string(CurrentPath + "\\ProjectDelta.dll").c_str());

	std::cout << "Waiting for any supported game..\n";

	while (!(hProcess = GetProcess()))
		Sleep(50);

	if (hProcess != INVALID_HANDLE_VALUE)
	{
		Args.DXVer = GetDXApi(hProcess);

		void* pAddr = VirtualAllocEx(hProcess, nullptr, sizeof(DeltaArgs_t) + 4, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (pAddr == 0)
		{
			printf("[Fatal Error] pAddr was zero.\nPlease report this bug.\n");
			std::cin.get();
			return 1;
		}

		//This shit is the reason normal injectors don't work - WHY DOESN'T WINAPI HAVE A FUNCTION FOR THIS?!
		if (WriteProcessMemory(hProcess, pAddr, &Args, sizeof(DeltaArgs_t), 0))
		{
			HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, ReCa<LPTHREAD_START_ROUTINE>(LoadLibraryA), pAddr, 0, 0);
			Sleep(300);

			auto Fn = GetProcAddressEx(hProcess, GetProcessId(hProcess), "ProjectDELTA.dll", "PD_Begin");

			hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)Fn, pAddr, 0, 0);
			
			if (hThread)
				CloseHandle(hThread);
		}
	}

	CloseHandle(hProcess);
	std::cout << "Done. Auto-closing in 3 seconds.\n";
	Sleep(3000);
	return 0;
}