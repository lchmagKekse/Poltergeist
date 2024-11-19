#pragma once
#include "../ImGui/imgui.h"
#include <string>
#include <vector>
#include <functional>

extern bool bFOV;
extern int FOV;

extern bool bAlwaysFlashlight;
extern ImVec4 FlashlightColor;
extern float FlashlightMultiplier;
extern float FlashLightAngle;

extern float SpeedMultiplier;
extern bool bJumping;
extern bool bInfiniteStamina;

extern bool bGodMode;

extern bool bInfiniteSanity;
extern bool bNoSanity;

extern int Money;
extern int Level;

extern bool bNoclip;
extern bool bToggleNoclip;

extern bool ESPGhost;
extern bool ESPGhostType;
extern bool ESPRooms;
extern bool ESPFavRoom;
extern bool ESPTools;

extern bool bFullbright;

extern bool bGhost;

extern std::function<void()> FunctionToExecute;