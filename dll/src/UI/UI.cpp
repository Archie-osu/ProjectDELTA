#include "UI.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../SDK/Memory/Memory.hpp"
#include "../Core/Core.hpp"

void UI::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
#ifdef _DEBUG
        ImGui::Text("Project DELTA v2 | Debug Version");
#else
        ImGui::Text("Project DELTA v2 by Archie");
#endif
        ImGui::EndMainMenuBar();
    }

    switch (Core::CurrentGame)
    {
    case Core::GameType::Deltarune:
        RenderDeltarune();
        break;
    case Core::GameType::Undertale:
    case Core::GameType::Underswap:
        break;
    }
}

void UI::RenderDeltarune()
{
    CDeltaruneData* pGameData = reinterpret_cast<CDeltaruneData*>(Core::pGame.pPlayerData);
    CDeltaruneGlobals* pGlobals = reinterpret_cast<CDeltaruneGlobals*>(Core::pGame.pGlobals);

	ImGui::SetNextWindowSize(ImVec2(480, 360));

    if (ImGui::Begin("Project DELTA", NULL,
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    ))
    {

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Crew"))
                nTab = 0;
            if (ImGui::MenuItem("World"))
                nTab = 1;

            ImGui::EndMenuBar();
        }

        if (nTab == 0 && pGameData != nullptr)
        {
            if (ImGui::BeginChild("ch_Kris", ImVec2(320, 90), true, 0))
            {
                ImGui::Text("Kris");
                ImGui::SliderDouble("HP", reinterpret_cast<double*>(&pGameData->Kris_CurHP), 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", reinterpret_cast<double*>(&pGameData->Kris_MaxHP), 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("ch_Ralsei", ImVec2(320, 90), true, 0))
            {
                ImGui::Text("Ralsei");
                ImGui::SliderDouble("HP", reinterpret_cast<double*>(&pGameData->Ralsei_CurHP), 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", reinterpret_cast<double*>(&pGameData->Ralsei_MaxHP), 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("ch_Susie", ImVec2(320, 90), true, 0))
            {
                ImGui::Text("Susie");
                ImGui::SliderDouble("HP", reinterpret_cast<double*>(&pGameData->Susie_CurHP), 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", reinterpret_cast<double*>(&pGameData->Susie_MaxHP), 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();
        }
        else if (nTab == 1 && pGlobals != nullptr)
        {
            ImGui::CheckboxDouble("Debug Mode", reinterpret_cast<double*>(&pGlobals->DebugMode));
            if (ImGui::Button("Force No Interact", ImVec2(150, 30)))
            {
                pGlobals->Interact = 0.0;
            }
        }
        ImGui::End();
    }
}

void UI::Init(PDIRECT3DDEVICE9 pDX9Device)
{
	if (!bInit)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Memory::GetCurrentWindow());
		ImGui_ImplDX9_Init(pDX9Device);

		ImGuiIO& curIO = ImGui::GetIO();
		ImFont* font = curIO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 18.0f);

		bInit = true;
	}
}

void UI::Init(ID3D11Device* pDX11Device, ID3D11DeviceContext* pContext)
{
	if (!bInit)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Memory::GetCurrentWindow());
		ImGui_ImplDX11_Init(pDX11Device, pContext);

		ImGuiIO& curIO = ImGui::GetIO();
		ImFont* font = curIO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 18.0f);

		bInit = true;
	}
}
