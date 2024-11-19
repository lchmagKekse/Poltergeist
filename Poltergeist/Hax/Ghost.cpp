#include "Ghost.h"
#include "Utils.h"
#include "../SDK/BP_Entity_classes.hpp"

void Ghost::GhostShowSelf() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	entity->GhostShowSelf();
}

void Ghost::FinishHunt() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	entity->RequestFinishHunt();
}

void Ghost::Hunt() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	entity->HuntBehaviour();
}

void Ghost::Interaction() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	entity->InteractionBehaviour();
}

void Ghost::GhostEvent() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	entity->GhostEventBehaviour();
}

void Ghost::CursedHunt() {
	auto entity = Utils::GetActor<SDK::ABP_Entity_C>();
	if (!entity) return;

	entity->CursedHuntBehaviour();
}