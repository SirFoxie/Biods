#pragma once
#include "imgui.h"

#include "../flock.hpp"

#include "ImGuiHelper.hpp"
#include "module.hpp"

class FlockMenu : public MenuModule {
public:
	FlockMenu() = delete;
	FlockMenu(Flock& _flock);
	~FlockMenu() = default;

	void update();

	void renderImGui();
	void onEnable() override;
	void onDisable() override;
private:
	Flock& flock;
	int selectedSubTab;
	bool isPlacing;
	bool isFollowMouse;
	int newFlockSize;
};

void FlockMenu::onEnable() {
	renderImGui();
}

void FlockMenu::onDisable() {
	return;
}

FlockMenu::FlockMenu(Flock& _flock) :
	MenuModule("Flock", "Configure Flock"), flock(_flock)
{
	this->selectedSubTab = 0;
	this->isPlacing = false;
	this->isFollowMouse = false;
	this->newFlockSize = flock.getBoids().size();
	return;
}

void FlockMenu::update() {
	if (isFollowMouse) {
		for (int i = 0; i < flock.size(); i++) {
			flock.addTarget({ 100.0, 100.0 }, i);
		}
	}
	if (flock.size() != newFlockSize) {
		flock.getBoids().resize(newFlockSize);
	}
}

void FlockMenu::renderImGui() {
	std::vector<std::string> tabs = { "Actions","Visuals" };
	ImGuiHelper::drawTabHorizontally("flock-actions-tab", ImVec2(ImGui::GetContentRegionAvail().x, 50), tabs, selectedSubTab);

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("flock-actions", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);
	ImGui::PopStyleColor();

	switch (selectedSubTab) {
	case 0: {
		ImGui::BeginChild("Population", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2), true);

		ImGui::PushItemWidth(this->elementsize());
		ImGui::SliderInt("Population", &newFlockSize, 0, 500);
		ImGui::SliderFloat("Alignment", &(flock.weights().alignment), 0.0, 2.0);
		ImGui::SliderFloat("Cohesion", &(flock.weights().cohesion), 0.0, 2.0);
		ImGui::SliderFloat("Seperation", &(flock.weights().seperation), 0.0, 2.0);
		ImGui::EndChild();

		ImGui::Spacing();

		ImGui::BeginChild("Behaviours", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);

		ImGui::PushItemWidth(this->elementsize());
		ImGui::Checkbox("Follow Mouse", &isFollowMouse);
		ImGui::EndChild();
		break;
	}

	case 1: {
		break;
	}
	default: {

		break;
	}
	}


	ImGui::EndChild();

	this->update();
	return;
}