#include "../UI.hpp"
#include <ImGui/imgui_cpp.hpp>
#include "../../SDK/Void.hpp"
#include "../../SDK/Invoker/Invoker.hpp"

void UI::Undertale()
{
	RValue glHP = Void.Invoker->GetGlobal("hp");
	RValue glMaxHP = Void.Invoker->GetGlobal("maxhp");
	RValue glLV = Void.Invoker->GetGlobal("lv");
	RValue glXP = Void.Invoker->GetGlobal("xp");
	RValue glGold = Void.Invoker->GetGlobal("gold");
	RValue glPlot = Void.Invoker->GetGlobal("plot");

	ImGui::SliderDouble("HP", &glHP.DoubleValue, 0, 120, "%.0f", 1.0f);
	ImGui::SliderDouble("Max HP", &glMaxHP.DoubleValue, 1, 99, "%.0f", 1.0f);
	ImGui::SliderDouble("LV", &glLV.DoubleValue, 1, 20, "%.0f", 1.0f);
	ImGui::InputDouble("EXP", &glXP.DoubleValue, 0, 0, "%.0f");
	ImGui::InputDouble("GOLD", &glGold.DoubleValue, 0, 0, "%.0f");
	ImGui::Text("Plot: %i", StCa<int>(glPlot.DoubleValue));

	Void.Invoker->SetGlobal("hp", glHP);
	Void.Invoker->SetGlobal("maxhp", glMaxHP);
	Void.Invoker->SetGlobal("lv", glLV);
	Void.Invoker->SetGlobal("xp", glXP);
	Void.Invoker->SetGlobal("gold", glGold);
	//Don't write plot, since it's read only anyway.
}