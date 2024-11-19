#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>

namespace Memory {
	uintptr_t FindPattern(uintptr_t base, std::string pattern);
	uintptr_t GetGObjects();
	uintptr_t GetProcessEvent();
	uintptr_t GetAppendString();
	uintptr_t GetGetViewPoint();
	int32_t GetProcessEventIndex();
	void CreateHook(uintptr_t address, uintptr_t function, uintptr_t original);
	uintptr_t GetPresent();
}