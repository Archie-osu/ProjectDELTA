#include "Core.hpp"
#include "../API/PD.hpp"
#include <Windows.h>
#include <TlHelp32.h>

#include "../Hooks/hkEndScene/hkEndScene.hpp"
#include "../Hooks/hkPresent/hkPresent.hpp"
#include "../Hooks/hkWndProc/hkWndProc.hpp"

int GetDirectXVersion()
{
    MODULEENTRY32 entry = { sizeof(MODULEENTRY32) };
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());

    Module32FirstW(Snapshot, &entry);

    do
    {
        if (!_wcsicmp(entry.szModule, L"d3d9.dll"))
            return 9;

        else if (!_wcsicmp(entry.szModule, L"d3d10.dll"))
            return 10;
         
        else if (!_wcsicmp(entry.szModule, L"d3d11.dll"))
            return 11;

    } while (Module32NextW(Snapshot, &entry));

    return false;
}

void Core::Start()
{
    DXVersion = GetDirectXVersion();
	gDelta.Begin();

    //Hook stuff
    if (DXVersion == 9)
        Hooks::EndScene::Init();
    else if (DXVersion == 11)
        Hooks::Present::Init();
    else
        gDelta.RaiseError("Unsupported DirectX version: %d", DXVersion);

    Hooks::WndProc::Init();
}

void Core::Kill()
{
	gDelta.Stop();
}