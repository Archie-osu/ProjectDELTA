#include "Lua Console.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Lua Engine/Lua Engine.hpp"

void CLuaConsole::ExecuteCommand(std::string Command)
{
    if (Command.starts_with("dbg-echo ") && Command.length() > 10)
    {
        Echo(std::string(Command.begin() + 9, Command.end()));
    }
    else
    {
        std::string ErrorMsg = Void.LuaEngine->RunScript(Command);
        if (!ErrorMsg.empty())
            Echo(ErrorMsg);
    }
}

void CLuaConsole::Echo(std::string text)
{
    this->Text.append(text); this->Text.push_back('\n');
}

void CLuaConsole::Render()
{
    ImGui::SetNextWindowSize(ImVec2(420, 420));

    if (ImGui::Begin("Lua Console", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
    {
        static TextEditor Editor;
        Void.LuaEngine->SetupLanguage(Editor);

        ImGui::Text("Console output");
        if (ImGui::BeginChildFrame(0xFFFC, ImVec2(400, 120)))
            ImGui::TextWrapped(this->Text.c_str());

        ImGui::EndChildFrame();

        ImGui::Text("Command input");

        Editor.Render("ConIn", ImVec2(400, 160), true);

        ImGui::NewLine();

        if (ImGui::Button("Run Script", ImVec2(120, 30)))
            ExecuteCommand(Editor.GetText());

        ImGui::SameLine();

        if (ImGui::Button("Clear", ImVec2(120, 30)))
            this->Text.clear();

        ImGui::SameLine();

        if (ImGui::Button("Reset API", ImVec2(120, 30)))
        {
            Void.LuaEngine->GetState() = sol::state(); //Reset state
            Void.LuaCallbackManager->Purge();
            Void.LuaEngine->Init();
        }
    }
    ImGui::End();
}