#include "Stats.h"

#include "../Menu/Vars.h"
#include "../SDK/BPL_Shivers_classes.hpp"
#include "../SDK/BP_PlayerCharacter_classes.hpp"
#include "../SDK/SteamCorePro_classes.hpp"

void Stats::SetPlayerMoney() {
	auto shivers = (SDK::UBPL_Shivers_C*)SDK::UObject::FindObjectFast("BPL_Shivers_C");
	if (!shivers) return;

	auto world = SDK::UWorld::GetWorld();
	if (!world) return;

	auto currentMoney = shivers->GetPlayerMoney(world);

	shivers->ManipulatePlayerMoney(SDK::E_CalculationTypes::NewEnumerator1, currentMoney, world);
	shivers->ManipulatePlayerMoney(SDK::E_CalculationTypes::NewEnumerator0, Money, world);
}

void Stats::SetPlayerLevel() {
	auto shivers = (SDK::UBPL_Shivers_C*)SDK::UObject::FindObjectFast("BPL_Shivers_C");
	if (!shivers) return;

	auto world = SDK::UWorld::GetWorld();
	if (!world) return;

	int32_t currentLevel;
	double currentXp;
	shivers->GetPlayerLevel(world, &currentLevel, &currentXp);

	shivers->ManipulatePlayerLevel(SDK::E_CalculationTypes::NewEnumerator1, currentLevel, world);
	shivers->ManipulatePlayerLevel(SDK::E_CalculationTypes::NewEnumerator0, Level - 1, world);
}

void Stats::UnlockAchievements() {
	auto steam = SDK::USteamProUserStats::GetDefaultObj();
	if (!steam) return;
	
	for (int index = 0; index < 100; index++) {
		auto achievement = steam->GetAchievementName(index);
		if (!achievement.IsValid() || achievement.ToString() == "") continue;

		steam->SetAchievement(achievement);
	}

	steam->StoreStats();
}