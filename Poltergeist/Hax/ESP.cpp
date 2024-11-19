#include "ESP.h"
#include "Utils.h"

#include "../Menu/Vars.h"
#include "../ImGui/imgui.h"

#include "../SDK/Basic.hpp"
#include "../SDK/Engine_classes.hpp"
#include "../SDK/BP_Entity_classes.hpp"
#include "../SDK/BP_RoomIdentifier_classes.hpp"
#include "../SDK/BP_FavouriteRoom_classes.hpp"
#include "../SDK/BP_Tool_classes.hpp"

void DrawOverlayText(std::string text, SDK::FVector position, ImColor color) {
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	if (!drawList) return;
	
	auto pos = Utils::World2Screen(position);
	if (pos.x == 0 && pos.y == 0) return;

	drawList->AddText(pos, color, text.c_str());
}

void ESP::DrawESP() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	if (ESPGhost || ESPGhostType) {
		auto text = ESPGhostType ? entity->EntityIdentity.ToString() : entity->EntityName.ToString();
		DrawOverlayText(text, entity->K2_GetActorLocation(), ImColor(1.0f, 0.0f, 0.0f, 1.0f));
	}

	std::string favRoomName = "";
	if (entity->FavouriteRoom && ESPFavRoom) {
		favRoomName = entity->FavouriteRoom->RoomIdentifier.ToString();
		DrawOverlayText(favRoomName, entity->FavouriteRoom->K2_GetActorLocation(), ImColor(0.8f, 0.0f, 1.0f, 1.0f));
	}

	if (ESPRooms) {
		auto rooms = Utils::GetActors<SDK::ABP_RoomIdentifier_C>();
		for (auto room : rooms) {
			if (!room) continue;
			if (room->RoomName.ToString() == favRoomName) continue;

			DrawOverlayText(room->RoomName.ToString(), room->K2_GetActorLocation(), ImColor(0.5f, 0.0f, 1.0f, 1.0f));
		}
	}

	if (ESPTools) {
		auto tools = Utils::GetActors<SDK::ABP_Tool_C>();
		for (auto tool : tools) {
			if (!tool) continue;

			DrawOverlayText(tool->ItemTag.TagName.ToString().substr(6), tool->K2_GetActorLocation(), ImColor(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}