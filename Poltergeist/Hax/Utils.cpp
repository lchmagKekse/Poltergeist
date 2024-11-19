#include "Utils.h"

ImVec2 Utils::World2Screen(SDK::FVector position) {
	auto world = SDK::UWorld::GetWorld();
	if (!world) return ImVec2(0, 0);
	if (!world->OwningGameInstance) return ImVec2(0, 0);
	if (!world->OwningGameInstance->LocalPlayers[0]) return ImVec2(0, 0);
	if (!world->OwningGameInstance->LocalPlayers[0]->PlayerController) return ImVec2(0, 0);

	SDK::FVector2D pos;
	SDK::UGameplayStatics::ProjectWorldToScreen(world->OwningGameInstance->LocalPlayers[0]->PlayerController, position, &pos, true);

	return ImVec2(pos.X, pos.Y);
}