#include <Windows.h>
#include <iostream>

#include "SDK/Basic.hpp"
#include "SDK/Engine_classes.hpp"

#include "Hax/Flashlight.h"
#include "Hax/Movement.h"
#include "Hax/Misc.h"
#include "Hax/Ghost.h"

#include "Menu/Menu.h"
#include "Menu/Vars.h"

#include "Memory/Memory.h"

typedef void(__thiscall* ProcessEventType)(SDK::UObject*, SDK::UFunction*, void*);
ProcessEventType ProcessEvent = nullptr;

typedef void* (__fastcall* GetViewPointType)(SDK::ULocalPlayer*, SDK::FMinimalViewInfo*);
GetViewPointType GetViewPoint = nullptr;

int32_t LoadingScreen = -1;
int32_t FlashLight = -1;
int32_t PlayerCharacter = -1;
int32_t BasePlayerCharacter = -1;
int32_t KillPlayer = -1;

void __fastcall ProcessEventDetour(SDK::UObject* caller, SDK::UFunction* function, void* params) {

	if (function->Index == KillPlayer) {
		if (bGodMode) return;
	}

	if (function->Index == LoadingScreen) {
		bNoclip = false;
	}

	ProcessEvent(caller, function, params);

	if (function->Index == FlashLight) {
		Flashlight::FlashlightTick(caller);
	}

	if (function->Index == PlayerCharacter) {
		Flashlight::PlayerCharacterTick(caller);
		Movement::PlayerCharacterTick(caller);
		Misc::PlayerCharacterTick(caller);

		if (FunctionToExecute) {
			FunctionToExecute();
			FunctionToExecute = nullptr;
		}
	}
	
	if (function->Index == BasePlayerCharacter) {
		Movement::PlayerCharacterTick(caller);

		if (FunctionToExecute) {
			FunctionToExecute();
			FunctionToExecute = nullptr;
		}
	}
}

void* __fastcall GetViewPointDetour(SDK::ULocalPlayer* player, SDK::FMinimalViewInfo* params) {
	auto returnValue = GetViewPoint(player, params);
	params->FOV = FOV;
	return returnValue;
}

SDK::UFunction* FindFunc(std::string name, bool partial = false) {

	for (int i = 0; i < SDK::UObject::GObjects->Num(); ++i) {

		SDK::UObject* Object = SDK::UObject::GObjects->GetByIndex(i);

		if (!Object) continue;
		if (!Object->IsA(SDK::UFunction::StaticClass())) continue;

		if (!partial && Object->GetFullName() == name) return (SDK::UFunction*)Object;
		if (partial && Object->GetFullName().find(name) != std::string::npos) return (SDK::UFunction*)Object;
	}

	return nullptr;
}

DWORD MainThread(HMODULE Module) {
	uintptr_t base = (uintptr_t)GetModuleHandleA(0);

	SDK::Offsets::GObjects = Memory::GetGObjects() - base;
	SDK::Offsets::AppendString = Memory::GetAppendString() - base;
	SDK::Offsets::ProcessEvent = Memory::GetProcessEvent() - base;
	SDK::Offsets::ProcessEventIdx = Memory::GetProcessEventIndex();

	auto funcLoadingScreen = FindFunc("Function WBP_LoadingScreen.WBP_LoadingScreen_C.Construct");
	auto funcFlashlight = FindFunc("Function BP_Flashlight.BP_Flashlight_C.ReceiveTick");
	auto funcPlayerCharacter = FindFunc("Function BP_PlayerCharacter.BP_PlayerCharacter_C.ReceiveTick");
	auto funcBasePlayerCharacter = FindFunc("Function BP_BasePlayerCharacter.BP_BasePlayerCharacter_C.ReceiveTick");
	auto funcKillPlayer = FindFunc("Function BTT_ChaseAndKillPlayer.BTT_ChaseAndKillPlayer_C.OnSuccess");

	if (funcLoadingScreen) LoadingScreen = funcLoadingScreen->Index;
	if (funcFlashlight) FlashLight = funcFlashlight->Index;
	if (funcPlayerCharacter) PlayerCharacter = funcPlayerCharacter->Index;
	if (funcBasePlayerCharacter) BasePlayerCharacter = funcBasePlayerCharacter->Index;
	if (funcKillPlayer) KillPlayer = funcKillPlayer->Index;

	Memory::CreateHook(Memory::GetProcessEvent(), (uintptr_t)&ProcessEventDetour, (uintptr_t)&ProcessEvent);
	Memory::CreateHook(Memory::GetGetViewPoint(), (uintptr_t)&GetViewPointDetour, (uintptr_t)&GetViewPoint);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Menu::InitMenu, hModule, 0, 0);
	}

	return TRUE;
}