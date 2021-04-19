#include "UI.hpp"
#include "../ImGui/imgui.h"
#include "../Core/Core.hpp"

void UI::RenderUnderswap()
{
    ImGui::SetNextWindowSize(ImVec2(flWindowX, flWindowY));

    if (ImGui::Begin("Project DELTA", NULL,
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    ))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Player"))
                nTab = 0;
            if (ImGui::MenuItem("World"))
                nTab = 1;

            ImGui::EndMenuBar();
        }

        if (nTab == 0)
        {
            ImGui::SliderDouble("HP", &Core::GlobalMap.at("playerhp").dValue, 0, 120, "%.0f", 1.0f);
            ImGui::SliderDouble("Max HP", &Core::GlobalMap.at("playermaxhp").dValue, 1, 99, "%.0f", 1.0f);
            ImGui::SliderDouble("LV", &Core::GlobalMap.at("playerlv").dValue, 1, 20, "%.0f", 1.0f);
            ImGui::InputDouble("EXP", &Core::GlobalMap.at("playerxp").dValue, 0, 0, "%.0f");
            ImGui::InputDouble("GOLD", &Core::GlobalMap.at("playergold").dValue, 0, 0, "%.0f");
        }
        else if (nTab == 1)
        {
            if (ImGui::Button("Force Movement", ImVec2(150, 30)))
            {
                Core::GlobalMap.at("canmove") = 1.0;
            }

            RenderInvoker();
        }
        ImGui::End();
    }
}