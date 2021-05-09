#include "../UI.hpp"
#include <ImGui/imgui_cpp.hpp>
#include "../../SDK/Void.hpp"
#include "../../SDK/Invoker/Invoker.hpp"

void UI::Sus()
{
	RValue glHP = Void.Invoker->GetGlobal("playerhp");
	RValue glMaxHP = Void.Invoker->GetGlobal("playermaxhp");
	RValue glGold = Void.Invoker->GetGlobal("playergold");
	RValue glLV = Void.Invoker->GetGlobal("playerlv");
	RValue glAT = Void.Invoker->GetGlobal("playerat"); 
	RValue glDF = Void.Invoker->GetGlobal("playerdf");

	ImGui::SliderDouble("HP", &glHP.DoubleValue, 0, 99, "%.0f", 1.0f);
	ImGui::SliderDouble("Max HP", &glMaxHP.DoubleValue, 0, 99, "%.0f", 1.0f);
	ImGui::SliderDouble("LV", &glLV.DoubleValue, 0, 20, "%.0f", 1.0f);
	ImGui::SliderDouble("AT", &glAT.DoubleValue, 0, 99, "%.0f", 1.0f);
	ImGui::SliderDouble("DF", &glDF.DoubleValue, 0, 99, "%.0f", 1.0f);
	ImGui::SliderDouble("Gold", &glGold.DoubleValue, 0, 1000, "%.0f", 1.0f);

	Void.Invoker->SetGlobal("playerhp", glHP);
	Void.Invoker->SetGlobal("playermaxhp", glMaxHP);
	Void.Invoker->SetGlobal("playergold", glGold);
	Void.Invoker->SetGlobal("playerlv", glLV);
	Void.Invoker->SetGlobal("playerat", glAT);
	Void.Invoker->SetGlobal("playerdf", glDF);
}