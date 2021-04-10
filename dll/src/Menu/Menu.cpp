#include "Menu.hpp"
#include "../ImGui/imgui.h"
#include "../API/API.hpp"
#include "../Invoker/Invoker.hpp"

using fn = bool(__cdecl*)(int);
using fnwrap = bool(__cdecl*)(Game::RValue);

inline fn room_goto_direct;

inline fnwrap room_goto_wrapper;

void Menu::SetStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(4, 4);
    style.CellPadding = ImVec2(4, 2);
    style.ItemSpacing = ImVec2(10, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21;
    style.ScrollbarSize = 14;
    style.GrabMinSize = 8;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 0;
    style.TabBorderSize = 0;

    style.WindowRounding = 4;
    style.ChildRounding = 1;
    style.FrameRounding = 8;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 8;
    style.GrabRounding = 12;
    style.LogSliderDeadzone = 4;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.07f, 0.27f, 0.75f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.67f, 0.00f, 0.75f, 0.46f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.74f, 0.00f, 0.73f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.17f, 0.60f, 0.29f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.17f, 0.60f, 0.67f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.09f, 0.73f, 0.55f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.10f, 0.36f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.11f, 0.07f, 0.56f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.07f, 0.23f, 0.60f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.37f, 0.81f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.37f, 0.81f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.60f, 0.13f, 1.00f, 0.61f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.00f, 0.86f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.23f, 1.00f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.12f, 0.34f, 0.58f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.09f, 0.18f, 0.48f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.10f, 0.31f, 0.56f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.79f, 0.34f, 0.82f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.31f, 0.19f, 0.51f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.37f, 0.20f, 0.91f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.17f, 0.82f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Menu::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
#ifdef _DEBUG
        ImGui::Text("Project DELTA by Archie - Debug Version");
#else
        ImGui::Text("Project DELTA by Archie - Version 1.01");
#endif
        ImGui::EndMainMenuBar();
    }

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
                Menu::nTab = 0;
            if (ImGui::MenuItem("World"))
                Menu::nTab = 1;

            ImGui::EndMenuBar();
        }

        if (nTab == 0)
        {

            if (ImGui::BeginChild("ch_Kris", ImVec2(320, 100), true, 0) && Globals::pCrewData.valid())
            {
                ImGui::Text("Kris");
                ImGui::SliderDouble("HP", &Globals::pCrewData.get<Game::CCrewData>().Kris_CurHP.Value<double>(), 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", &Globals::pCrewData.get<Game::CCrewData>().Kris_MaxHP.Value<double>(), 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("ch_Ralsei", ImVec2(320, 100), true, 0) && Globals::pCrewData.valid())
            {
                ImGui::Text("Ralsei");
                ImGui::SliderDouble("HP", &Globals::pCrewData.get<Game::CCrewData>().Ralsei_CurHP.Value<double>(), 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", &Globals::pCrewData.get<Game::CCrewData>().Ralsei_MaxHP.Value<double>(), 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("ch_Susie", ImVec2(320, 100), true, 0) && Globals::pCrewData.valid())
            {
                ImGui::Text("Susie");
                ImGui::SliderDouble("HP", &Globals::pCrewData.get<Game::CCrewData>().Susie_CurHP.Value<double>(), 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", &Globals::pCrewData.get<Game::CCrewData>().Susie_MaxHP.Value<double>(), 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();
        }

        else if (nTab == 1)
        {
            ImGui::CheckboxDouble("Debug Mode", &Globals::pGlobalVars.get<Game::CGlobals>().DebugMode.Value<double>());
            if (ImGui::Button("Force No Interact", ImVec2(150, 30)))
            {
                Globals::pGlobalVars.get<Game::CGlobals>().Interact.Value<double>() = 0;
            }

            if (ImGui::BeginChild("ch_Invoker", ImVec2(440, 140), true))
            {
                //TODO: Reverse & walk the function map - DELTARUNE stores functions to be called from the game loop's 'call ecx'.

                static const char* szFunctions[] = { "room_goto", "room_goto_next", "room_goto_previous", "room_restart" };

                ImGui::Text("WARNING: Incorrect use of the invoker can crash the game!");
                ImGui::Combo("Function", &nInvFunc, szFunctions, IM_ARRAYSIZE(szFunctions));
                if (nInvFunc == 0)
                    ImGui::InputDouble("Room Number", &Menu::nRoom, 1.0, 1.0, "%.0f", ImGuiInputTextFlags_CharsDecimal);

                if (ImGui::Button("Invoke!"))
                {
                    PUCHAR function = Globals::pRoomGoTo.getraw(); 
                    Game::RValue rRoom;
                    switch (nInvFunc)
                    {
                    case 0: /* room_goto */
                        rRoom.SetValue(static_cast<double>(nRoom));
                        Invoker::invoke<bool, Game::RValue*>(function, &rRoom);
                        break;
                    case 1: /* room_goto_next */
                        Invoker::invoke<bool>(function + 0x20);
                        break;
                    case 2: /* room_goto_previous */
                        Invoker::invoke<bool>(function + 0x30);
                        break;
                    case 3:
                        Invoker::invoke<bool>(function + 0x100);
                        break;
                    default:
                        break;
                    }
                }
            }
            ImGui::EndChild();
            //room_goto_direct = (int(__cdecl*)(int))Invoker::FindPattern("\x8B\x44\x24\x04\x50\xA3\x00\x00\x00\x00", "xxxxxx????").getraw();
            //room_goto_previous =  0x524EF0
            //room_goto_next =      0x524EE0
            //room_goto =           0x524EC0
            //room_restart =        0x524FC0
        }
        ImGui::End();
    }
}
