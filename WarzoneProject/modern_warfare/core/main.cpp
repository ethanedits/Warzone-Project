#include <iostream>
#include "driver/driver.h"
#include "game/offsets.h"
#include "overlay/hijack/hijack.h"
#include "overlay/renderer/renderer.h"
#include <string>

#define ERROR(msg) std::cout << msg << std::endl; std::cin.get(); exit(EXIT_FAILURE); 
#define ASSERT(cond, msg) if (!cond) { ERROR(msg) }

uintptr_t ref_def_ptr = NULL;

int xFOV = 200; //Aimbot horizontal FOV
int yFOV = 200; //Aimbot vertical FOV
int aSmoothAmount = 2; // Aimbot smoothness
int aimKey = VK_MENU;

int entX = 0;
int entY = 0;

int closestX = 0;
int closestY = 0;

int aX = 0;
int aY = 0;

namespace config {
	//Visual
	bool boxESP_enabled = true;
	bool snaplines_enabled = true;
	bool boneESP_enabled = true;
	bool nameESP_enabled = true;
	bool healthESP_enabled = true;
	bool distanceESP_enabled = true;
	bool aimbotTarget_enabled = false;
	bool crosshair_enabled = true;
	bool fovCircle_enabled = true;

	//Game
	bool aimbot_enabled = true;

	//Menu
	bool openOnInsert = true;

	//Tabs
	bool tabVisual_open = false;
	bool tabGame_open = false;
	bool tabSettings_open = false;
}

bool menuIsOpen = false;

void UpdateAddresses()
{
	sdk::client_info = decryption::get_client_info();

	sdk::client_info_base = decryption::get_client_info_base();

	ref_def_ptr = decryption::get_ref_def();

	sdk::ref_def = driver::read<sdk::ref_def_t>(ref_def_ptr);

	sdk::names = sdk::GetNameList();

	#ifdef DEBUG
	std::cout << "	[~] UPDATED: client_info:      " << std::hex << sdk::client_info << std::endl;
	std::cout << "	[~] UPDATED: client_info_base: " << std::hex << sdk::client_info_base << std::endl;
	std::cout << "	[~] UPDATED: name_list: " << std::hex << sdk::names << std::endl;
	#endif
}

int main() {

	#pragma region Initialization
	sdk::process_id = driver::get_process_id("ModernWarfare.exe");
	ASSERT(sdk::process_id, "[-] Could not find Process ID");

	sdk::module_base = driver::get_module_base_address("ModernWarfare.exe");
	ASSERT(sdk::module_base, "[-] Could not find Module Base Address");

	sdk::peb = driver::get_peb();
	ASSERT(sdk::peb, "[-] Could not find PEB");
		
	sdk::set_game_hwnd();
	ASSERT(sdk::hwnd, "[-] Could not find Game Window");

	ASSERT(hijack::init(), "[-] Could not hijack NVIDIA Overlay (Is it open?)");
	ASSERT(renderer::init(), "[-] Could initialize renderer");

	sdk::client_info = decryption::get_client_info();
	ASSERT(sdk::client_info, "[-] Could not get client (You are not in game or cheat is outdated)");

	sdk::client_info_base = decryption::get_client_info_base();
	ASSERT(sdk::client_info_base, "[-] Could not get client (You are not in game or cheat is outdated)");

	ref_def_ptr = decryption::get_ref_def();
	ASSERT(ref_def_ptr, "[-] Could not get ref def pointer (OUTDATED!)");

	std::cout << "Mainframe by EthanEDITS Loaded" << std::endl;
	std::cout << " " << std::endl;
	std::cout << "[INSERT] for menu, [END] to close." << std::endl;

#ifdef DEBUG
	std::cout << "[-] Debug Log" << std::endl;
	std::cout << "	[~] pid:  " << std::hex << sdk::process_id << std::endl;
	std::cout << "	[~] base: " << std::hex << sdk::module_base << std::endl;
	std::cout << "	[~] peb:  " << std::hex << sdk::peb << std::endl;
	std::cout << "	[~] hwnd: " << std::hex << sdk::hwnd << std::endl;
	std::cout << "	[~] client_info:      " << std::hex << sdk::client_info << std::endl;
	std::cout << "	[~] client_info_base: " << std::hex << sdk::client_info_base << std::endl;
#endif

	sdk::ref_def = driver::read<sdk::ref_def_t>(ref_def_ptr);

	sdk::names = sdk::GetNameList();

	#pragma endregion

	bool newRoundCheck = false;

	while (!GetAsyncKeyState(VK_END)) {
		renderer::scene::start();

		bool inGame = sdk::in_game(); 
		//std::cout << "Game Status: " << inGame << std::endl;

		//NEED TO TEST
		//if (inGame && !newRoundCheck)
			UpdateAddresses(); //Doing with no inGame checks is Bad for performance, should only be called every time a new round is created
			//newRoundCheck = true;

		//if (!inGame)
			//newRoundCheck = false;

		sdk::player_t local(sdk::client_info_base + (sdk::local_index() * offsets::player::size));
		auto local_pos = local.get_pos();
		auto local_team = local.team_id();

		int closestX = 50000;
		int closestY = 50000;

		for (int i = 0; i < 150; i++) {
			sdk::player_t player(sdk::client_info_base + (i * offsets::player::size));
			if (!player.is_valid() || player.dead()) {
				continue;
			}

			bool isEnemy = true; //Is the entity your teammate or not

			if (player.team_id() == local_team)
				isEnemy = false;

			//Entity Vectors and WorldToScreen

			Vector3 pos = player.get_pos();
			Vector3 headPos = pos; headPos.z = pos.z + 66.f;

			if (local_pos == pos) continue;

			Vector2 PlayerOrigin = sdk::WorldToScreen(pos); if (PlayerOrigin == Vector2(0, 0)) continue;
			Vector2 PlayerHead = sdk::WorldToScreen(headPos); if (PlayerHead == Vector2(0, 0)) continue; //Does not use bones!

			Vector2 HealthPos = sdk::WorldToScreen(Vector3(headPos.x, headPos.y, headPos.z + 30.f)); if (HealthPos == Vector2(0, 0)) continue;
			Vector2 NamePos = sdk::WorldToScreen(Vector3(pos.x, pos.y, pos.z - 30.f)); if (NamePos == Vector2(0, 0)) continue;

			auto dist = sdk::units_to_m(local_pos.distance_to(pos));

			if (dist < 225) {
				float height = PlayerOrigin.y - PlayerHead.y;
				float width = height / 4;

				sdk::NameEntry nameEntry = player.get_name_entry(i);

			//Box ESP
			if (config::boxESP_enabled) {
				if (isEnemy) {
					renderer::scene::boxTransparentFill(PlayerOrigin.x - height / 4, PlayerOrigin.y, PlayerOrigin.x + height / 4, PlayerOrigin.y - height, 2, renderer::colors::red_color); //Box Fill

					renderer::scene::boxStyle1(PlayerOrigin.x - height / 4, PlayerOrigin.y, PlayerOrigin.x + height / 4, PlayerOrigin.y - height, 2, renderer::colors::red_color); //Box
				}
				else
				{
					renderer::scene::boxTransparentFill(PlayerOrigin.x - height / 4, PlayerOrigin.y, PlayerOrigin.x + height / 4, PlayerOrigin.y - height, 2, renderer::colors::green_color); //Box Fill

					renderer::scene::boxStyle1(PlayerOrigin.x - height / 4, PlayerOrigin.y, PlayerOrigin.x + height / 4, PlayerOrigin.y - height, 2, renderer::colors::green_color); //Box
				}
			}

			//Bone ESP

				const auto bone_base = decryption::get_bone();
				Vector3 bone_base_pos = player.get_bone_base_pos();
				const auto bone_index = decryption::get_bone_index(i); if (bone_index == decryption::get_bone_index(sdk::local_index())) continue;
				uintptr_t bone_ptr = player.get_bone_ptr(bone_base, bone_index);

				if (!bone_ptr) continue;

				if (dist < 100)
				{
					if (config::boneESP_enabled) {
						for (std::pair<int, int> p2p : player.skeleton)
						{
							Vector3 bone1_3d = player.get_bone_position(bone_ptr, bone_base_pos, p2p.first);
							Vector3 bone2_3d = player.get_bone_position(bone_ptr, bone_base_pos, p2p.second);

							Vector2 bone1 = sdk::WorldToScreen(bone1_3d);
							Vector2 bone2 = sdk::WorldToScreen(bone2_3d);
							//If the bones are less than 3 meters apart
							if (sdk::units_to_m(bone1_3d.distance_to(bone2_3d)) <= 3.f && bone1 != Vector2(0, 0) && bone2 != Vector2(0, 0))
								//If bones are not within 2 meters of the player
								if (!sdk::units_to_m(local_pos.distance_to(bone1_3d)) <= 2.f && !sdk::units_to_m(local_pos.distance_to(bone2_3d)) <= 2.f)
									renderer::scene::line(bone1.x, bone1.y, bone2.x, bone2.y, 1.f, renderer::colors::white_color);
						}
					}


					//DEBUG BONE LIST
					/*for (int boneI = 10; boneI < 30; boneI++)
					{
						Vector2 w2s_bone1 = sdk::WorldToScreen(player.get_bone_position(bone_ptr, bone_base_pos, boneI));
						Vector2 w2s_bone2 = w2s_bone1; w2s_bone2.y += 1.f;

						wchar_t intBuf[256];

						swprintf_s(intBuf, L"%d", boneI);

						renderer::scene::text(w2s_bone1, intBuf, renderer::colors::white_color, renderer::fonts::tahoma_font);
					}*/

			//Healthbar

	
				}

			//Aimbot

				Vector3 aimBone = player.get_bone_position(bone_ptr, bone_base_pos, 7); //Head
				Vector2 w2s_aim = sdk::WorldToScreen(aimBone);

				int entX = w2s_aim.x;
				int entY = w2s_aim.y;

				// Is it an enemy
				if (isEnemy) {
					// Aimbot fov
					if (abs(sdk::ScreenWidth / 2 - entX) < abs(sdk::ScreenWidth / 2 - closestX) && abs(sdk::ScreenWidth / 2 - entX) < xFOV && abs(sdk::ScreenHeight / 2 - entY) < abs(sdk::ScreenHeight / 2 - closestY) && abs(sdk::ScreenHeight / 2 - entY) < yFOV)
					{
						// Aimbot find closest target
						closestX = entX;
						closestY = entY;
					}
				}

				if (closestX != 50000 && closestY != 50000)
				{
					if (config::aimbotTarget_enabled) {
						//Debug-Draw circle on enemy being targeted
						renderer::scene::circle(Vector2(closestX, closestY), renderer::colors::darkred_color, 5.f);
					}

					// If aimbot key pressed
					if (GetAsyncKeyState(aimKey))
					{
						aX = (closestX - sdk::ScreenWidth / 2) / aSmoothAmount;
						aY = (closestY - sdk::ScreenHeight / 2) / aSmoothAmount;

						// If mouse cursor shown
						CURSORINFO ci = { sizeof(CURSORINFO) };
						if (GetCursorInfo(&ci))
						{
							if (ci.flags == 0)
								mouse_event(MOUSEEVENTF_MOVE, aX, aY, 0, 0); // enable aimbot when mouse cursor is hidden
						}
					}
				}

			//Distance Text
				if (config::distanceESP_enabled) {
					wchar_t buf[6];
					swprintf(buf, sizeof(buf), L"[%.1f] ", dist);
					if (isEnemy)
						renderer::scene::textDropShadow(PlayerOrigin, buf, renderer::colors::red_color, renderer::fonts::futura_font);
					else
						renderer::scene::textDropShadow(PlayerOrigin, buf, renderer::colors::green_color, renderer::fonts::futura_font);
				}
			//Health Text
				if (config::healthESP_enabled) {
					wchar_t Healthbuf[6];
					swprintf(Healthbuf, sizeof(Healthbuf), L"Health %d", nameEntry.health);
					if (isEnemy)
						renderer::scene::textDropShadow(HealthPos, Healthbuf, renderer::colors::red_color, renderer::fonts::futura_font);
					else
						renderer::scene::textDropShadow(HealthPos, Healthbuf, renderer::colors::green_color, renderer::fonts::futura_font);
				}
			//Name Text
				if (config::nameESP_enabled) {
					if (dist < 100) {
						if (isEnemy)
							renderer::scene::textDropShadowStr(NamePos, renderer::colors::red_color, renderer::fonts::futura_font, nameEntry.name);
						else
							renderer::scene::textDropShadowStr(NamePos, renderer::colors::green_color, renderer::fonts::futura_font, nameEntry.name);
					}
				}
			//Snapline

				if (config::snaplines_enabled) {
					if (isEnemy)
						renderer::scene::line(sdk::ScreenWidth / 2, sdk::ScreenHeight, PlayerOrigin.x, PlayerOrigin.y, 1.f, renderer::colors::red_color);
					else
						renderer::scene::line(sdk::ScreenWidth / 2, sdk::ScreenHeight, PlayerOrigin.x, PlayerOrigin.y, 1.f, renderer::colors::green_color);
				}

			}
		}

		//Crosshair
		if (config::crosshair_enabled)
			renderer::scene::circle(Vector2(sdk::ScreenWidth / 2, sdk::ScreenHeight / 2), renderer::colors::red_color, 5.f);

		//Fov Circle
		if (config::fovCircle_enabled)
			renderer::scene::circle(Vector2(sdk::ScreenWidth / 2, sdk::ScreenHeight / 2), renderer::colors::white_color, xFOV);

		//Aim FOV Control
		if (GetAsyncKeyState(VK_OEM_PLUS))
			xFOV += 1;
			yFOV += 1;

		if (GetAsyncKeyState(VK_OEM_MINUS))
			xFOV -= 1;
			yFOV -= 1;

		//Toggle Menu
		if (config::openOnInsert) {
			if (GetAsyncKeyState(VK_INSERT))
			{
				menuIsOpen = !menuIsOpen;
				Sleep(100);
			}
		}
		else
		{
		//Hold Menu
			if (GetAsyncKeyState(VK_TAB))
				menuIsOpen = true;
			else if (!GetAsyncKeyState(VK_INSERT))
				menuIsOpen = false;
		}

		//Rendering Menu
		if (menuIsOpen) {

			Vector2 menuPos = renderer::scene::menu::position;

			//Window
			renderer::scene::menu::Plane(menuPos.x, menuPos.y, 800, 12, renderer::colors::orange_color);
			renderer::scene::menu::Plane(menuPos.x, menuPos.y + 12, 85, 588, renderer::colors::black_color);
			renderer::scene::menu::Plane(menuPos.x + 85, menuPos.y + 12, 715, 588, renderer::colors::light_gray_color);
			renderer::scene::menu::Plane(menuPos.x + 104, menuPos.y + 26, 677, 560, renderer::colors::dark_gray_color2);

			//Tabs
			
			//Visual Tab
			renderer::scene::menu::Button Vbutton;
			Vbutton.Draw(menuPos.x, menuPos.y + 26, 85, 20, renderer::colors::black_color);
			if (Vbutton.isClicked)
			{
				config::tabVisual_open = true;
				config::tabGame_open = false;
				config::tabSettings_open = false;
			}
			renderer::scene::menu::Text(Vector2(menuPos.x + 20, menuPos.y + 26.5), L"Visual", renderer::colors::light_gray_color4, renderer::fonts::header_tahoma_font);

			//Game Tab
			renderer::scene::menu::Button Gbutton;
			Gbutton.Draw(menuPos.x, menuPos.y + 56, 85, 20, renderer::colors::black_color);
			if (Gbutton.isClicked)
			{
				config::tabVisual_open = false;
				config::tabGame_open = true;
				config::tabSettings_open = false;
			}
			renderer::scene::menu::Text(Vector2(menuPos.x + 20, menuPos.y + 56.5), L"Game", renderer::colors::light_gray_color4, renderer::fonts::header_tahoma_font);

			//Settings Tab
			renderer::scene::menu::Button Sbutton;
			Sbutton.Draw(menuPos.x, menuPos.y + 86, 85, 20, renderer::colors::black_color);
			if (Sbutton.isClicked)
			{
				config::tabVisual_open = false;
				config::tabGame_open = false;
				config::tabSettings_open = true;
			}
			renderer::scene::menu::Text(Vector2(menuPos.x + 20, menuPos.y + 86.5), L"Settings", renderer::colors::light_gray_color4, renderer::fonts::header_tahoma_font);

			//Visual
			if (config::tabVisual_open) {
				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 41), L"Box ESP", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 64, 30, 30, config::boxESP_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 101), L"Snaplines", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 124, 30, 30, config::snaplines_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 161), L"Bone ESP", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 184, 30, 30, config::boneESP_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 221), L"Name ESP", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 244, 30, 30, config::nameESP_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 281), L"Health ESP", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 304, 30, 30, config::healthESP_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 341), L"Distance ESP", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 364, 30, 30, config::distanceESP_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 401), L"Aimbot Target Circle", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 424, 30, 30, config::aimbotTarget_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 461), L"Crosshair Overlay", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 484, 30, 30, config::crosshair_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 521), L"FOV Circle Overlay", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 544, 30, 30, config::fovCircle_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

			}

			//Game
			if (config::tabGame_open) {
				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 41), L"Aimbot", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 64, 30, 30, config::aimbot_enabled, renderer::colors::dark_gray_color, renderer::colors::orange_color);

				//Aimbot Smoothness Changer
				renderer::scene::menu::ValuePicker(menuPos.x + 124, menuPos.y + 124, 50, 30, aSmoothAmount, renderer::colors::dark_gray_color, renderer::colors::white_color, renderer::colors::white_color);
			}

			//Settings
			if (config::tabSettings_open) {
				renderer::scene::menu::Text(Vector2(menuPos.x + 126, menuPos.y + 41), L"Insert to Toggle Menu or Hold Tab", renderer::colors::orange_color, renderer::fonts::header_tahoma_font);
				renderer::scene::menu::Checkbox(menuPos.x + 124, menuPos.y + 64, 30, 30, config::openOnInsert, renderer::colors::dark_gray_color, renderer::colors::orange_color);
			}



			//Image Rendering Later
			//renderer::scene::menu::Image(11, 65, 59, 31, renderer::internal::resource1);

		}

		renderer::scene::end();

		sdk::ref_def = driver::read<sdk::ref_def_t>(decryption::get_ref_def());
	}
	renderer::shutdown();

	return 0;
}
