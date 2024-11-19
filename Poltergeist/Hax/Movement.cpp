#include "Movement.h"

SDK::FVector Move(SDK::FVector position, SDK::FRotator rotation, float forward, float right) {

	float pitchRad = rotation.Pitch * 3.14159f / 180.0f;
	float yawRad = rotation.Yaw * 3.14159f / 180.0f;

	position.X += forward * std::cos(yawRad) * std::cos(pitchRad);
	position.Y += forward * std::sin(yawRad) * std::cos(pitchRad);
	position.Z += forward * std::sin(pitchRad);

	position.X += right * std::cos(yawRad + 1.5708f);
	position.Y += right * std::sin(yawRad + 1.5708f);

	return position;
}

void NoClip(SDK::ABP_BasePlayerCharacter_C* character) {
	if (!character) return;

	auto movement = character->CharacterMovement;
	if (!movement) return;

	if (bToggleNoclip) {
		character->SetActorEnableCollision(!bNoclip);
		movement->MovementMode = SDK::EMovementMode::MOVE_Walking;
		bToggleNoclip = false;
	}

	auto camera = character->Camera;
	if (!camera) return;

	auto rotation = camera->K2_GetComponentRotation();

	movement->GravityScale = bNoclip ? 0.0f : 1.0f;

	if (!bNoclip) return;

	movement->MovementMode = SDK::EMovementMode::MOVE_Walking;
	movement->Velocity.X = 0.0f;
	movement->Velocity.Y = 0.0f;
	movement->Velocity.Z = 0.0f;

	auto location = character->K2_GetActorLocation();
	auto speed = 3.0f;

	if (GetKeyState(VK_SHIFT) & 0x8000) {
		speed *= 2.0f;
	}

	if (GetKeyState('W') & 0x8000) {
		location = Move(location, rotation, speed, 0.0f);
	}

	if (GetKeyState('S') & 0x8000) {
		location = Move(location, rotation, -speed, 0.0f);
	}

	if (GetKeyState('D') & 0x8000) {
		location = Move(location, rotation, 0.0f, speed);
	}

	if (GetKeyState('A') & 0x8000) {
		location = Move(location, rotation, 0.0f, -speed);
	}

	if (GetKeyState(VK_SPACE) & 0x8000) {
		location.Z += speed;
	}

	if (GetKeyState(VK_CONTROL) & 0x8000) {
		location.Z -= speed;
	}

	character->K2_SetActorLocation(location, false, nullptr, false);
}

void Movement::PlayerCharacterTick(SDK::UObject* caller) {
	if (!caller) return;
	auto playerCharacter = (SDK::ABP_BasePlayerCharacter_C*)caller;

	auto movement = playerCharacter->CharacterMovement;
	if (!movement) return;

	if (bJumping && GetAsyncKeyState(VK_SPACE)) {
		movement->JumpZVelocity = 500.0f;
		playerCharacter->Jump();
	}

	for (auto component : playerCharacter->BlueprintCreatedComponents) {
		if (!component) continue;
		if (!component->IsA(SDK::UBP_SprintComponent_C::StaticClass())) continue;

		auto sprint = (SDK::UBP_SprintComponent_C*)component;

		movement->MaxCustomMovementSpeed = 600 * SpeedMultiplier;
		movement->MaxWalkSpeed = sprint->IsSprinting ? 300 * SpeedMultiplier : 200 * SpeedMultiplier;
		movement->MaxWalkSpeedCrouched = 300 * SpeedMultiplier;

		if (bInfiniteStamina) {
			sprint->Stamina = sprint->MaximumStamina;
		}
	}

	NoClip(playerCharacter);
}