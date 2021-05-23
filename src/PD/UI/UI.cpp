#include "UI.hpp"
#include <ImGui/imgui.h>
#include <string>

#include "../SDK/Void.hpp"
#include "../SDK/Structs/Structs.hpp"
#include "../SDK/Invoker/Invoker.hpp"
#include "../SDK/Lua Engine/Lua Engine.hpp"

#include <Memories/CatHeaven.hpp>

bool stricontains(const std::string& String, const std::string& ToFind)
{
    std::string LowerString;
    std::string LowerToFind;

    for (auto ch : String)
        LowerString.push_back(std::tolower(ch));

    for (auto ch : ToFind)
        LowerToFind.push_back(std::tolower(ch));

    return (LowerString.find(LowerToFind) != std::string::npos);
}

void UI::Render(std::vector<void*>)
{
    if (!bOpen)
        return;

    UI::ApplyStyle();

    ShowGameWarning();

	std::string BaseTitle = "Project DELTA v3 - ";
	auto GameForm = (GameForm_t*)Void.GetGameData();

    if (GameForm)
        BaseTitle.append(GameForm->ReadString(GameForm->Gen8.DisplayNameOffset));

    DrawMainMenuBar();

    ImGui::SetNextWindowSizeConstraints(ImVec2(256, 144), ImVec2(512, 400)); //16:9 kinda

	if (ImGui::Begin(BaseTitle.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
        if (stricontains(BaseTitle, "SURVEY_PROGRAM"))
            Deltarune();

        else if (stricontains(BaseTitle, "UNDERTALE"))
            Undertale();

        else if (stricontains(BaseTitle, "TS!Underswap"))
            Sus();

        else
            Default();

        if (bDrawLuaConsole)
            DrawLuaConsole();

        if (bDrawDebugMenu)
            DrawDebug();
	}
    ImGui::End();
}

void UI::Default()
{
    ImGui::Text("Automatic game recognition failed.\nIf you wish, you may use the Lua API to interact with the game.");
}

void UI::ApplyStyle()
{
    if (bSetStyle)
        return;

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

    bSetStyle = true;
}

void UI::ShowGameWarning()
{
    static bool Warned = false;

    if (!Warned)
        ImGui::OpenPopup("PD - Warning");

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("PD - Warning", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Using Project DELTA online may get you banned.\nUse at your own risk!\n\n");
        ImGui::Separator();

        if (ImGui::Button("I accept the risk", ImVec2(120, 0))) { Warned = true; ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Close game", ImVec2(120, 0))) { exit(0); }
        ImGui::EndPopup();
    }
}

void UI::DrawMainMenuBar()
{
    static bool bShowDemo = false;

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Tool"))
    {
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::Checkbox("Disable Menu Movement", &UI::bNoMovementInMenu);
            ImGui::Checkbox("Draw Debug Menu", &UI::bDrawDebugMenu);
            ImGui::Checkbox("Draw Lua Console", &UI::bDrawLuaConsole);
            ImGui::EndMenu();
        };

        if (ImGui::BeginMenu("Debug"))
        {
            
            ImGui::Text("data.win base: %p", Void.GetGameData());
            ImGui::Text("device: %p", Void.GetGameDevice());
            ImGui::Checkbox("Use experimental sig", &UI::bUseExperimentalSig);
            ImGui::Checkbox("Show Demo Window", &bShowDemo);

            ImGui::EndMenu();
        }

        if (bUseExperimentalSig && !UI::bNoMovementInMenu && UI::bDrawDebugMenu && !UI::bDrawLuaConsole)
        {
            if (ImGui::BeginMenu("Cat Heaven"))
            {
                CatHeaven::CatHeaven();
                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }

    ImGui::Text("Project DELTA v3.0.4 - Build date: %s @ %s", __DATE__, __TIME__);

    ImGui::EndMainMenuBar();

    if (bShowDemo)
        ImGui::ShowDemoWindow(nullptr);
}

void UI::DrawDebug()
{
    if (ImGui::Begin("PD v3 - data viewer", nullptr, ImGuiWindowFlags_NoSavedSettings))
    {
        GameForm_t* pGameData = nullptr;
        pGameData = ReCa<GameForm_t*>(Void.GetGameData());

        if (pGameData)
        {
            char FORMString[5] = { 0 }; memcpy(FORMString, pGameData->FORM, 4);
            ImGui::Text("Is FORM: %s", strcmp(FORMString, "FORM") == 0 ? "Yes" : "No");
            ImGui::Text("File length: 0x%X", pGameData->FileLength);
            ImGui::Separator();
            ImGui::Text("GEN8 Length: 0x%X", pGameData->Gen8.Length);
            ImGui::Checkbox("DisableDebug", &pGameData->Gen8.DisableDebug);
            ImGui::Text("GameName: %s", pGameData->ReadString(pGameData->Gen8.GameNameOffset));
            ImGui::Text("DisplayName: %s", pGameData->ReadString(pGameData->Gen8.DisplayNameOffset));
            ImGui::Text("FileName: %s", pGameData->ReadString(pGameData->Gen8.FileNameOffset));
        }
    }
    ImGui::End();
}

void UI::DrawLuaConsole()
{
    ImGui::SetNextWindowSize(ImVec2(560, 320));

    if (ImGui::Begin("Lua Console", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::PushFont(cast<ImFont*>(Void.pCodeFont));

        Void.LuaEngine->puTextEditor.Render("ConIn", ImVec2(540, 240), true);

        ImGui::PopFont();

        if (ImGui::Button("Execute", ImVec2(120, 30))) {
            auto Error = Void.LuaEngine->RunScript(Void.LuaEngine->puTextEditor.GetText());
            if (!Error.empty())
                Void.Warning(Error.c_str());
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset API", ImVec2(120, 30)))
        {
            Void.LuaEngine->Init(); //Reset state
        }
    }
    ImGui::End();
}