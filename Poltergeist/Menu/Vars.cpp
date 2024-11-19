#include "Vars.h"


bool bFOV = false;
int FOV = 105;

bool bAlwaysFlashlight = false;
ImVec4 FlashlightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
float FlashlightMultiplier = 1.0f;
float FlashLightAngle = 1.0f;

float SpeedMultiplier = 1.0f;
bool bJumping = false;
bool bInfiniteStamina = false;

bool bInfiniteSanity = false;
bool bNoSanity = false;

bool bGodMode = false;

int Money = 1000;
int Level = 10;

bool bNoclip = false;
bool bToggleNoclip = false;

bool ESPGhost = false;
bool ESPGhostType = false;
bool ESPRooms = false;
bool ESPFavRoom = false;
bool ESPTools = false;

bool bFullbright = false;

bool bGhost = false;

std::function<void()> FunctionToExecute = nullptr;