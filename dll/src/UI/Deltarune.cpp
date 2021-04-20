#include "UI.hpp"
#include "../ImGui/imgui.h"
#include "../Core/Core.hpp"

void UI::RenderDeltarune()
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
            if (ImGui::MenuItem("Crew"))
                nTab = 0;
            if (ImGui::MenuItem("World"))
                nTab = 1;

            ImGui::EndMenuBar();
        }

        if (nTab == 0)
        {
            if (ImGui::BeginChild("ch_Kris", ImVec2(320, 100), true, 0))
            {
                ImGui::Text("Kris");
                ImGui::SliderDouble("HP", &Core::GlobalMap.at("hp").at(1).dValue, 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", &Core::GlobalMap.at("maxhp").at(1).dValue, 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("ch_Ralsei", ImVec2(320, 100), true, 0))
            {
                ImGui::Text("Ralsei");
                ImGui::SliderDouble("HP", &Core::GlobalMap.at("hp").at(3).dValue, 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", &Core::GlobalMap.at("maxhp").at(3).dValue, 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("ch_Susie", ImVec2(320, 100), true, 0))
            {
                ImGui::Text("Susie");
                ImGui::SliderDouble("HP", &Core::GlobalMap.at("hp").at(2).dValue, 0, 120, "%.0f", 1.0f);
                ImGui::SliderDouble("Max HP", &Core::GlobalMap.at("maxhp").at(2).dValue, 0, 120, "%.0f", 1.0f);
            }
            ImGui::EndChild();

            ImGui::SliderDouble("Money", &Core::GlobalMap.at("gold").dValue, 0, 1000, "%.0f", 1.0f);
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