#pragma once

#include "../ImGui/imgui.h"
#include "../SDK/Basic.hpp"
#include "../SDK/Engine_classes.hpp"

namespace Utils {
	ImVec2 World2Screen(SDK::FVector position);

	template<typename T> std::vector<T*> GetActors() {
		std::vector<T*> actors;

		auto world = SDK::UWorld::GetWorld();
		if (!world) return actors;

		auto actorClass = T::StaticClass();

		for (auto level : world->Levels) {
			for (auto uObject : level->Actors) {
				if (!uObject) continue;
				if (!uObject->IsA(actorClass)) continue;
				actors.push_back((T*)uObject);
			}
		}

		return actors;
	}

	template<typename T> T* GetActor() {
		auto world = SDK::UWorld::GetWorld();
		if (!world) return nullptr;

		auto actorClass = T::StaticClass();

		for (auto level : world->Levels) {
			for (auto uObject : level->Actors) {
				if (!uObject) continue;
				if (!uObject->IsA(actorClass)) continue;
				return (T*)uObject;
			}
		}

		return nullptr;
	}
}