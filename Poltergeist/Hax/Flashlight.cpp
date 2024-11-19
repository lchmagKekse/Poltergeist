#include "Flashlight.h"

bool PreviousAlwaysFlashlight;

SDK::FColor ImCol2FColor(ImVec4 color) {
	return SDK::FColor{ (uint8_t)(color.z * 255), (uint8_t)(color.y * 255), (uint8_t)(color.x * 255), (uint8_t)(color.w * 255) };
}

void SetColorAndCone(SDK::USpotLightComponent* light) {
	if (!light) return;

	light->LightColor = ImCol2FColor(FlashlightColor);
	light->BloomTint = ImCol2FColor(FlashlightColor);

	light->SetOuterConeAngle(FlashLightAngle * 26);
	light->SetInnerConeAngle(FlashLightAngle * 7);
}

void Flashlight::FlashlightTick(SDK::UObject* caller) {
	if (!caller) return;
	auto flashlight = (SDK::ABP_Flashlight_C*)caller;

	flashlight->Multiplier = FlashlightMultiplier;
	SetColorAndCone(flashlight->SpotLight);
}

void Flashlight::PlayerCharacterTick(SDK::UObject* caller) {
	if (!caller) return;
	auto playerCharacter = (SDK::ABP_PlayerCharacter_C*)caller;

	playerCharacter->FlashlightIntensityMultiplier = FlashlightMultiplier;
	SetColorAndCone(playerCharacter->Flashlight);

	if (bAlwaysFlashlight) {
		playerCharacter->IsFlashlightActivated = true;
	}

	if (!bAlwaysFlashlight && PreviousAlwaysFlashlight) {
		playerCharacter->IsFlashlightActivated = false;
		playerCharacter->OnRep_IsFlashlightActivated();
	}

	PreviousAlwaysFlashlight = bAlwaysFlashlight;
}