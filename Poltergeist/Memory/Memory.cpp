#include "Memory.h"
#include "../SDK/CoreUObject_classes.hpp"

typedef uintptr_t(__fastcall* CreateHookType)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
CreateHookType OriginalCreateHook = nullptr;

void Memory::CreateHook(uintptr_t address, uintptr_t function, uintptr_t original) {
	if (!OriginalCreateHook) {
		auto base = (uintptr_t)GetModuleHandleA("GameOverlayRenderer64.dll");
		auto createHookAddr = FindPattern(base, "48 89 5C 24 ?? 57 48 83 EC ?? 33 C0 48 89");
		OriginalCreateHook = reinterpret_cast<CreateHookType>(createHookAddr);
	}

	OriginalCreateHook(address, function, original, 1);
}

void PatternToBytes(const std::string& pattern, std::vector<std::pair<uint8_t, bool>>& bytes) {
    size_t len = pattern.length();
    for (size_t i = 0; i < len; i += 3) {
        std::string byteStr = pattern.substr(i, 2);
        if (byteStr == "??") {
            bytes.emplace_back(0, true);
        }
        else {
            uint8_t byte = static_cast<uint8_t>(std::strtol(byteStr.c_str(), nullptr, 16));
            bytes.emplace_back(byte, false);
        }
    }
}

uintptr_t Memory::FindPattern(uintptr_t base, std::string pattern) {
    std::vector<std::pair<uint8_t, bool>> bytes;
    PatternToBytes(pattern, bytes);

    uint8_t* basePtr = reinterpret_cast<uint8_t*>(base);
    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t currentAddr = base;

    while (VirtualQuery(reinterpret_cast<LPCVOID>(currentAddr), &mbi, sizeof(mbi)) != 0) {
        if (mbi.State == MEM_COMMIT && (mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READWRITE | PAGE_READONLY))) {
            uint8_t* regionStart = reinterpret_cast<uint8_t*>(mbi.BaseAddress);
            size_t regionSize = mbi.RegionSize;

            for (size_t i = 0; i < regionSize - bytes.size(); ++i) {
                bool found = true;
                for (size_t j = 0; j < bytes.size(); ++j) {
                    if (!bytes[j].second && regionStart[i + j] != bytes[j].first) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return reinterpret_cast<uintptr_t>(regionStart + i);
                }
            }
        }
        currentAddr += mbi.RegionSize;
    }
    return 0;
}

uintptr_t Memory::GetGObjects() {
	auto base = (uintptr_t)GetModuleHandleA(0);

	auto someFunctionThatReferencesGObjects = FindPattern(base, "40 53 55 56 48 83 EC ?? 48 8B 11");
	auto instruction = someFunctionThatReferencesGObjects + 0x57;
	auto relativeOffset = (int32_t*)(instruction + 3);

	return (uintptr_t)(instruction + *relativeOffset + 7);
}

uintptr_t Memory::GetProcessEvent() {
	auto base = (uintptr_t)GetModuleHandleA(0);
	return FindPattern(base, "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 48 8D 6C 24 ?? 48 89 9D ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C5 48 89 85 ?? ?? ?? ?? 4D");
}

uintptr_t Memory::GetAppendString() {
	auto base = (uintptr_t)GetModuleHandleA(0);
	return FindPattern(base, "48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 80 3D ?? ?? ?? ?? ?? 48 8B F2");
}

uintptr_t Memory::GetGetViewPoint() {
	auto base = (uintptr_t)GetModuleHandleA(0);
	return FindPattern(base, "48 8B C4 48 89 58 ?? 48 89 68 ?? 56 57 41 57 48 81 EC ?? ?? ?? ?? 0F 29 70 D8 48");
}

uintptr_t Memory::GetPresent() {
	auto base = (uintptr_t)GetModuleHandleA("GameOverlayRenderer64.dll");
	return FindPattern(base, "48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 41 56 41 57 48 83 EC ?? 41");
}

int32_t Memory::GetProcessEventIndex() {
	auto uObject = SDK::UObject::FindObject("Class CoreUObject.Object");
	if (!uObject) return 0;

	void** VTable = *reinterpret_cast<void***>(uObject->VTable);

	for (int32_t index = 0; index < 1000; index++) {
		if ((uintptr_t)VTable[index] != (uintptr_t)SDK::Offsets::ProcessEvent) continue;
		return index;
	}

	return 0;
}