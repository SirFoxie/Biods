#pragma once

#include "imgui.h"
#include "raylib.h"

#include "editor.hpp"

class toolbox{
public:
	toolbox() = delete;
	toolbox(Editor& _edtr);
	~toolbox() = default;

	void updateAndDrawMenu(void);

private:
	ImGuiStyle* style;
	Editor& edtr;
	void DrawMapEditor();
};

toolbox::toolbox(Editor& _edtr) : 
	edtr(_edtr) {}

void toolbox::updateAndDrawMenu(void){

	int menuW = 200;
	int menuH = 300;

	ImGui::SetNextWindowPos(ImVec2(0, GetScreenWidth()/2 - menuH));
	ImGui::SetNextWindowSize(ImVec2(menuW, menuH));

	static ImGuiWindowFlags windowFlags{
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar
	};

	if (ImGui::Begin("ToolBox", NULL, windowFlags)) {
		// Draw Submenus
		DrawMapEditor();
	}

	ImGui::End();
}

void toolbox::DrawMapEditor() {
	ImGui::BeginChild("MapEditor", ImVec2(150, 200), true);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
	std::string toolNames[] = { "Mark Area", "Clear Area" };
	for (int i = 0; i < sizeof(toolNames) / sizeof(toolNames[0]); i++) {
		std::string it = toolNames[i];
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
		if (ImGui::Button(it.c_str(), ImVec2(140, 40))) edtr.setTool(i);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
}