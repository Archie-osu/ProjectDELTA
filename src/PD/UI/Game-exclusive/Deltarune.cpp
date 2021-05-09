#include "../UI.hpp"
#include <ImGui/imgui_cpp.hpp>
#include "../../SDK/Void.hpp"
#include "../../SDK/Invoker/Invoker.hpp"

void UI::Deltarune()
{
	RValue glHP = Void.Invoker->GetGlobal("hp");
    RValue glMaxHP = Void.Invoker->GetGlobal("maxhp");
	RValue glGold = Void.Invoker->GetGlobal("gold");

    if (ImGui::BeginChild("ch_Kris", ImVec2(320, 100), true, 0))
    {
        ImGui::Text("Kris");
        ImGui::SliderDouble("HP", &glHP.at(1).DoubleValue, 0, 120, "%.0f", 1.0f);
        ImGui::SliderDouble("Max HP", &glMaxHP.at(1).DoubleValue, 0, 120, "%.0f", 1.0f);
    }
    ImGui::EndChild();

    if (ImGui::BeginChild("ch_Ralsei", ImVec2(320, 100), true, 0))
    {
        ImGui::Text("Ralsei");
        ImGui::SliderDouble("HP", &glHP.at(3).DoubleValue, 0, 120, "%.0f", 1.0f);
        ImGui::SliderDouble("Max HP", &glMaxHP.at(3).DoubleValue, 0, 120, "%.0f", 1.0f);
    }
    ImGui::EndChild();

    if (ImGui::BeginChild("ch_Susie", ImVec2(320, 100), true, 0))
    {
        ImGui::Text("Susie");
        ImGui::SliderDouble("HP", &glHP.at(2).DoubleValue, 0, 120, "%.0f", 1.0f);
        ImGui::SliderDouble("Max HP", &glMaxHP.at(2).DoubleValue, 0, 120, "%.0f", 1.0f);
    }
    ImGui::EndChild();

    ImGui::SliderDouble("Money", &glGold.DoubleValue, 0, 1000, "%.0f", 1.0f);

    Void.Invoker->SetGlobal("maxhp", glMaxHP);
    Void.Invoker->SetGlobal("hp", glHP);
    Void.Invoker->SetGlobal("gold", glGold);
}