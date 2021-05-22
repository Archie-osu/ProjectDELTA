#include "Lua Console.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Lua Engine/Lua Engine.hpp"

void CLuaConsole::ExecuteCommand(std::string Command)
{
    std::string ErrorMsg = Void.LuaEngine->RunScript(Command);
    if (!ErrorMsg.empty())
    {
        Void.Warning("Lua Script Execution Error\n\n%s", ErrorMsg.c_str());
    }
}

void CLuaConsole::Render()
{
    ImGui::SetNextWindowSize(ImVec2(560, 320));

    if (ImGui::Begin("Lua Console", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
    {
        static TextEditor Editor; //Make this a member of Void.LuaEngine or something idk

        Void.LuaEngine->SetupLanguage(Editor);

        ImGui::PushFont(cast<ImFont*>(Void.pCodeFont));

        Editor.Render("ConIn", ImVec2(540, 240), true);

        ImGui::PopFont();

        if (ImGui::Button("Execute", ImVec2(120, 30)))
            ExecuteCommand(Editor.GetText());

        ImGui::SameLine();

        if (ImGui::Button("Reset API", ImVec2(120, 30)))
        {
            Void.LuaEngine->GetState() = sol::state(); //Reset state
            Void.LuaCallbackManager->Purge();
            Void.LuaEngine->Init();
            Void.LuaScriptHookManager->Purge();
        }
    }
    ImGui::End();
}