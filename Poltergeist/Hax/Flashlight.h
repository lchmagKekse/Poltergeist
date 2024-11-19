#pragma once
#include "../Menu/Vars.h"
#include "../SDK/BP_Flashlight_classes.hpp"
#include "../SDK/BP_PlayerCharacter_classes.hpp"

namespace Flashlight {
	void FlashlightTick(SDK::UObject* caller);
	void PlayerCharacterTick(SDK::UObject* caller);
}