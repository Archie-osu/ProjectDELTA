#include "UI.hpp"
#include "../ImGui/imgui.h"
#include "../Core/Core.hpp"

void UI::RenderUndertale()
{
    ImGui::SetNextWindowSize(ImVec2(flWindowX, flWindowY));

    if (ImGui::Begin("Project DELTA", NULL,
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
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
            ImGui::SliderDouble("HP", &Core::GlobalMap.at("hp").dValue, 0, 120, "%.0f", 1.0f);
            ImGui::SliderDouble("Max HP", &Core::GlobalMap.at("maxhp").dValue, 1, 99, "%.0f", 1.0f);
            ImGui::SliderDouble("LV", &Core::GlobalMap.at("lv").dValue, 1, 20, "%.0f", 1.0f);
            ImGui::InputDouble("EXP", &Core::GlobalMap.at("xp").dValue, 0, 0, "%.0f");
            ImGui::InputDouble("GOLD", &Core::GlobalMap.at("gold").dValue, 0, 0, "%.0f");
            ImGui::InputDouble("Plot", &Core::GlobalMap.at("plot").dValue, 0, 0, "%.0f");
        }
        else if (nTab == 1)
        {
            ImGui::CheckboxDouble("Debug Mode", &Core::GlobalMap.at("debug").dValue);
            if (ImGui::Button("Force Movement", ImVec2(150, 30)))
            {
                Core::GlobalMap.at("interact") = 0.0;
            }
            RenderInvoker();
        }
        ImGui::End();
    }
}