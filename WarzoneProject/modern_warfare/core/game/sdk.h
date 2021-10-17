#pragma once
#include <cstdint>
#include <windows.h>
#include "..\utils\vectors.h"
#include <vector>

namespace sdk {
	extern HANDLE	 process_id;
	extern uintptr_t module_base;
	extern uintptr_t peb;
	extern HWND      hwnd;

	extern uint64_t  names;

	extern uintptr_t client_info;
	extern uintptr_t client_info_base;

	extern int ScreenWidth;
	extern int ScreenHeight;

	struct ref_def_view {
		Vector2 tan_half_fov;
		char pad[0xC];
		Vector3 axis[3];
	};

	struct NameEntry {
		uint32_t idx;
		char name[0x24];
		uint8_t unk1[0x64];
		uint32_t health;
	};

	struct ref_def_t {
		int x;
		int y;
		int width;
		int height;
		ref_def_view view;
	};

	extern struct ref_def_t ref_def;

	void set_game_hwnd();

	bool in_game();

	int player_count();

	int local_index();

	uint64_t GetNameList();

	class player_t {
	public:
		player_t(uintptr_t address) {
			this->address = address;
		}

		uintptr_t address{};

		NameEntry get_name_entry(uint32_t index);

		bool is_valid();

		bool dead();

		int team_id();

		Vector3 get_pos();

		Vector3 get_bone_base_pos();

		uintptr_t get_bone_ptr(const uint64_t bone_base, const uint64_t bone_index);

		Vector3 get_bone_position(const uintptr_t bone_ptr, Vector3 base_pos, const int bone);

		std::vector<std::pair<int, int>> skeleton = {
			{8, 7}, {7, 6}, //Head and neck
			{6, 14}, {14, 15}, {15, 16}, //Right Arm and Hand
			{6, 10}, {10, 11}, {11, 12}, //Left Arm and Hand
			{6, 5}, {5, 4}, {4, 3}, {3, 2}, //Spine
			{2, 17}, {17, 18}, {18, 19}, {19, 20}, //Right Leg and Foot
			{2, 21}, {21, 22}, {22, 23}, {23, 24}, //Left Leg and Foot
		};
	};

	Vector2 WorldToScreen(Vector3 world_position);

	float units_to_m(float units);

	bool inRange(auto low, auto high, auto x)
	{
		return (low <= x && x <= high);
	}
}

namespace decryption {
	uintptr_t get_client_info();

	uintptr_t get_client_info_base();

	uint64_t get_bone();

	uint32_t get_bone_index(uint32_t index);

	uintptr_t get_ref_def();
}
