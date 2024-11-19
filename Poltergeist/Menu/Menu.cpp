#include "Menu.h"
#include "Roboto.h"
#include "Vars.h"
#include "../Memory/Memory.h"
#include "../Hax/ESP.h"
#include "../Hax/Stats.h"
#include "../Hax/Ghost.h"
#include "../Hax/Misc.h"
#include "../ImGui/imgui_internal.h"

HWND hWnd = NULL;
Present oPresent = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* pRenderTargetView;
WNDPROC oWndProc;
ImFont* roboto;
bool ImGuiInit = false;
bool menu = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (bJumping && wParam == VK_SPACE) {
		return true;
	}

	if (uMsg == WM_KEYUP && wParam == VK_F1) {
		menu = !menu;
	}

	if (uMsg == WM_KEYUP && wParam == VK_ESCAPE) {
		menu = false;
	}

	ImGui::SetMouseCursor(menu ? ImGuiMouseCursor_Arrow : ImGuiMouseCursor_None);
	ImGui::GetIO().WantCaptureMouse = menu;
	ImGui::GetIO().MouseDrawCursor = menu;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	if (menu) {
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT hkPresent(IDXGISwapChain* pSwapchain, UINT SyncInterval, UINT Flags) {
	if (!ImGuiInit) {
		if (SUCCEEDED(pSwapchain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapchain->GetDesc(&sd);
			hWnd = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplDX11_Init(pDevice, pContext);

			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha = 1.0;
			style.WindowPadding = ImVec2(15, 15);
			style.WindowRounding = 4.0f;
			style.FramePadding = ImVec2(5, 5);
			style.FrameRounding = 2.0f;
			style.ItemSpacing = ImVec2(12, 8);
			style.ItemInnerSpacing = ImVec2(8, 6);
			style.IndentSpacing = 10.0f;
			style.ScrollbarSize = 15.0f;
			style.ScrollbarRounding = 9.0f;
			style.GrabMinSize = 5.0f;
			style.GrabRounding = 3.0f;
			style.WindowBorderSize = 0.0f;
			style.PopupBorderSize = 0.0f;
			style.PopupRounding = 4.0f;
			style.AntiAliasedLines = true;
			style.CurveTessellationTol = 1.25f;
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

			ImVec4 Background = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
			ImVec4 Frame = ImVec4(0.082f, 0.082f, 0.082f, 1.0f);
			ImVec4 Hover = ImVec4(0.81f, 0.01f, 0.01f, 1.0f);
			ImVec4 Active = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			ImVec4 Text = ImVec4(0.82f, 0.82f, 0.82f, 1.0f);

			style.Colors[ImGuiCol_Text] = Text;
			style.Colors[ImGuiCol_TextDisabled] = Text;
			style.Colors[ImGuiCol_TextSelectedBg] = Active;
			style.Colors[ImGuiCol_CheckMark] = Active;
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.0f, 0.8f);
			style.Colors[ImGuiCol_WindowBg] = Background;
			style.Colors[ImGuiCol_PopupBg] = Background;
			style.Colors[ImGuiCol_FrameBg] = Frame;
			style.Colors[ImGuiCol_FrameBgHovered] = Hover;
			style.Colors[ImGuiCol_FrameBgActive] = Active;
			style.Colors[ImGuiCol_TitleBg] = Background;
			style.Colors[ImGuiCol_TitleBgCollapsed] = Background;
			style.Colors[ImGuiCol_TitleBgActive] = Background;
			style.Colors[ImGuiCol_MenuBarBg] = Background;
			style.Colors[ImGuiCol_ScrollbarBg] = Background;
			style.Colors[ImGuiCol_ScrollbarGrab] = Frame;
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = Hover;
			style.Colors[ImGuiCol_ScrollbarGrabActive] = Active;
			style.Colors[ImGuiCol_SliderGrab] = Hover;
			style.Colors[ImGuiCol_SliderGrabActive] = Hover;
			style.Colors[ImGuiCol_Button] = Frame;
			style.Colors[ImGuiCol_ButtonHovered] = Hover;
			style.Colors[ImGuiCol_ButtonActive] = Active;
			style.Colors[ImGuiCol_Header] = Frame;
			style.Colors[ImGuiCol_HeaderHovered] = Hover;
			style.Colors[ImGuiCol_HeaderActive] = Active;
			style.Colors[ImGuiCol_Tab] = Frame;
			style.Colors[ImGuiCol_TabHovered] = Hover;
			style.Colors[ImGuiCol_TabActive] = Hover;

			roboto = io.Fonts->AddFontFromMemoryTTF(&Roboto, 14, 14);

			ImGuiInit = true;
		}
		else {
			return oPresent(pSwapchain, SyncInterval, Flags);
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ESP::DrawESP();

	if (menu) {
		ImGui::PushFont(roboto);
		Menu::RenderMenu();
		ImGui::PopFont();
	}

	ImGui::EndFrame();
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapchain, SyncInterval, Flags);
}

void Menu::InitMenu() {
	Memory::CreateHook(Memory::GetPresent(), (uintptr_t)hkPresent, (uintptr_t)&oPresent);
}

void Menu::RenderMenu() {
	ImGui::Begin("Poltergeist v1.0.0", &menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

	if (ImGui::BeginTabBar("Menu", ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_FittingPolicyResizeDown)) {

		if (ImGui::BeginTabItem("   Gameplay   ")) {

			ImGui::SetNextItemWidth(300);
			ImGui::SliderInt("FOV", &FOV, 0, 360);

			ImGui::SetNextItemWidth(300);
			ImGui::SliderFloat("Speed", &SpeedMultiplier, 0.0f, 5.0f, "%.1f");

			ImGui::NewLine();

			ImGui::BeginColumns("Gameplay", 2, ImGuiColumnsFlags_NoBorder);
			ImGui::SetColumnWidth(0, 200);

			ImGui::Checkbox("Enable Jumping", &bJumping);
			ImGui::Checkbox("Infinite Stamina", &bInfiniteStamina);

			if (ImGui::Checkbox("NoClip", &bNoclip)) {
				bToggleNoclip = true;
			}

			if (ImGui::Checkbox("Fullbright", &bFullbright)) {
				FunctionToExecute = Misc::SetFullBright;
			}

			ImGui::NextColumn();

			if (ImGui::Checkbox("Infinite Sanity", &bInfiniteSanity)) {
				if (bInfiniteSanity) bNoSanity = false;
			}

			if (ImGui::Checkbox("No Sanity", &bNoSanity)) {
				if (bNoSanity) bInfiniteSanity = false;
			}

			if (ImGui::Checkbox("God Mode", &bGodMode));

			ImGui::EndColumns();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("   Flashlight   ")) {

			ImGui::Checkbox("Always Flashlight", &bAlwaysFlashlight);
			ImGui::ColorEdit4("Color", (float*)&FlashlightColor, ImGuiColorEditFlags_NoTooltip);

			ImGui::SetNextItemWidth(300);
			ImGui::SliderFloat("Brightness", &FlashlightMultiplier, 0.0f, 50.0f, "%.1f");
			ImGui::SetNextItemWidth(300);
			ImGui::SliderFloat("Size", &FlashLightAngle, 0.0f, 5.0f, "%.1f");
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("   Stats   ")) {

			ImGui::BeginColumns("Stats", 2, ImGuiColumnsFlags_NoBorder);
			ImGui::SetColumnWidth(0, 170);
			ImGui::SetNextItemWidth(140);
			ImGui::InputInt("##Money", &Money, 1, 10);
			ImGui::SetNextItemWidth(140);
			ImGui::InputInt("##Level", &Level, 1, 10);

			ImGui::NextColumn();

			if (ImGui::Button("Set Player Money", ImVec2(170, 0))) {
				FunctionToExecute = Stats::SetPlayerMoney;
			}

			if (ImGui::Button("Set Player Level", ImVec2(170, 0))) {
				FunctionToExecute = Stats::SetPlayerLevel;
			}

			if (ImGui::Button("Unlock Achievements", ImVec2(170, 0))) {
				FunctionToExecute = Stats::UnlockAchievements;
			}

			ImGui::EndColumns();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("   ESP   ")) {

			ImGui::BeginColumns("ESP", 2, ImGuiColumnsFlags_NoBorder);
			ImGui::SetColumnWidth(0, 200);

			ImGui::Checkbox("Ghost", &ESPGhost);
			ImGui::Checkbox("Rooms", &ESPRooms);
			ImGui::Checkbox("Tools", &ESPTools);

			ImGui::NextColumn();

			ImGui::Checkbox("Ghost Type", &ESPGhostType);
			ImGui::Checkbox("Ghost Room", &ESPFavRoom);

			ImGui::EndColumns();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("   Ghost   ")) {

			ImGui::BeginColumns("Ghost", 2, ImGuiColumnsFlags_NoBorder);
			ImGui::SetColumnWidth(0, 180);

			if (ImGui::Button("Hunt", ImVec2(170, 0))) {
				FunctionToExecute = Ghost::Hunt;
			}

			if (ImGui::Button("Cursed Hunt", ImVec2(170, 0))) {
				FunctionToExecute = Ghost::CursedHunt;
			}

			if (ImGui::Button("Finish Hunt", ImVec2(170, 0))) {
				FunctionToExecute = Ghost::FinishHunt;
			}

			ImGui::NextColumn();

			if (ImGui::Button("Show Ghost", ImVec2(170, 0))) {
				FunctionToExecute = Ghost::GhostShowSelf;
			}

			if (ImGui::Button("Interaction", ImVec2(170, 0))) {
				FunctionToExecute = Ghost::Interaction;
			}

			if (ImGui::Button("Ghost Event", ImVec2(170, 0))) {
				FunctionToExecute = Ghost::GhostEvent;
			}

			ImGui::EndColumns();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}