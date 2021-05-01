#include "../UI.hpp"
#include <ImGui/imgui_cpp.hpp>
#include "../../SDK/Void.hpp"
#include "../../SDK/Invoker/Invoker.hpp"

void UI::Deltarune()
{
	static double RVX, RVY;

	ImGui::InputDouble("X", &RVX);
	ImGui::InputDouble("Y", &RVY);

	if (ImGui::Button("Apply", ImVec2(60, 30)))
	{
		Void.Invoker->Call("window_set_size", { RVX, RVY });
	}

	ImGui::ShowDemoWindow();
}