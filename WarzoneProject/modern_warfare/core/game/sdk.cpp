#include "sdk.h"
#include "..\driver\driver.h"
#include "offsets.h"

namespace sdk {
	HANDLE	  process_id = NULL;
	uintptr_t module_base = NULL;
	uintptr_t peb = NULL;
	HWND      hwnd = NULL;

	uint64_t  names = NULL;

	uintptr_t client_info = NULL;
	uintptr_t client_info_base = NULL;

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	BOOL CALLBACK enum_windows(HWND hwnd, LPARAM param) {
		DWORD process_id;
		GetWindowThreadProcessId(hwnd, &process_id);
		if (process_id == param)
		{
			sdk::hwnd = hwnd;
			return false;
		}
		return true;
	}

	void set_game_hwnd() {
		EnumWindows(enum_windows, (LPARAM)sdk::process_id);
	}

	bool in_game() {
		return driver::read<int>(sdk::module_base + offsets::game_mode) > 1;
	}

	int player_count() {
		return driver::read<uintptr_t>(sdk::module_base + offsets::game_mode);
	}

	int local_index() {
		auto local_index = driver::read<uintptr_t>(sdk::client_info + offsets::local_index);
		return driver::read<int>(local_index + 0x1FC);
	}

	uint64_t GetNameList() {
		auto ptr = driver::read<uint64_t>(sdk::module_base + offsets::name_array);
		return ptr + offsets::name_array_padding;
	}

	NameEntry player_t::get_name_entry(uint32_t index) {
		return driver::read<NameEntry>(sdk::names + (index * offsets::name_array_size));
	}
		
	bool player_t::is_valid() {
		return driver::read<bool>(address + offsets::player::valid);
	}
		
	bool player_t::dead() {
		auto dead1 = driver::read<bool>(address + offsets::player::dead_1);
		auto dead2 = driver::read<bool>(address + offsets::player::dead_2);
		return dead1 || dead2;
	}

	int player_t::team_id() {
		return driver::read<int>(address + offsets::player::team);
	}

	Vector3 player_t::get_pos() {
		auto local_pos = driver::read<uintptr_t>(address + offsets::player::pos);
		return driver::read<Vector3>(local_pos + 0x40);
	}

	Vector3 player_t::get_bone_base_pos()
	{
		return driver::read<Vector3>(sdk::client_info + offsets::bones::bone_base);
	}

	uintptr_t player_t::get_bone_ptr(const uint64_t bone_base, const uint64_t bone_index)
	{
		return driver::read<uintptr_t>(bone_base + (bone_index * offsets::bones::size) + 0xC0);
	}

	Vector3 player_t::get_bone_position(const uintptr_t bone_ptr, Vector3 base_pos, const int bone)
	{
		return base_pos + driver::read<Vector3>(bone_ptr + ((uint64_t)bone * 0x20) + 0x10);
	}

	Vector3 get_camera_position() {
		auto camera = driver::read<uintptr_t>(sdk::module_base + offsets::camera_base);
		if (!camera)
			return {};

		return driver::read<Vector3>(camera + offsets::camera_pos);
	}

	Vector2 world_to_screen(Vector3 world_location, Vector3 matricies[3]) {
		Vector2 out;
		Vector3 camera_pos = get_camera_position();
		Vector2 fov = ref_def.view.tan_half_fov;

		auto local = world_location - camera_pos;
		auto trans = Vector3{
			local.dot(matricies[1]),
			local.dot(matricies[2]),
			local.dot(matricies[0])
		};

		if (trans.z < 0.01f) {
			return Vector2(0, 0);
		}

		out.x = ((float)ref_def.width / 2.0) * (1.0 - (trans.x / fov.x / trans.z));
		out.y = ((float)ref_def.height / 2.0) * (1.0 - (trans.y / fov.y / trans.z));

		if (out.x < 1 || out.y < 1 || (out.x > sdk::ref_def.width) || (out.y > sdk::ref_def.height)) {
			return Vector2(0, 0);
		}

		return out;
	}

	Vector2 WorldToScreen(Vector3 world_position) {
		return world_to_screen(world_position, ref_def.view.axis);
	}

	float units_to_m(float units) {
		return units * 0.0254;
	}

	ref_def_t ref_def;
}