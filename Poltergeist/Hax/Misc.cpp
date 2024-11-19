#include "Misc.h"

#include "../Menu/Vars.h"
#include "../SDK/BP_PlayerCharacter_classes.hpp"

void Misc::PlayerCharacterTick(SDK::UObject* caller) {
	if (!caller) return;
	auto playerCharacter = (SDK::ABP_PlayerCharacter_C*)caller;

	if (bInfiniteSanity) {
		playerCharacter->PlayerSanity = 100;
	}

	if (bNoSanity) {
		playerCharacter->PlayerSanity = 0;
	}
}

void Misc::SetFullBright() {
	auto engine = SDK::UEngine::GetEngine();
	if (!engine) return;
	if (!engine->GameViewport) return;

	auto viewmode = (SDK::EViewModeIndex*)((uintptr_t)engine->GameViewport + 0xB0);
	*viewmode = bFullbright ? SDK::EViewModeIndex::VMI_Unlit : SDK::EViewModeIndex::VMI_Lit;
}