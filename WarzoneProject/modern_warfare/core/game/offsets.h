#pragma once

// Version  1.43.2.10430694

namespace offsets
{
    constexpr auto camera_base = 0x157D08E0;
    constexpr auto camera_pos = 0x1D8;
    constexpr auto game_mode = 0x17C2A488;
    constexpr auto local_index = 0x3B058;
    constexpr auto local_index_pos = 0x1FC;
    constexpr auto name_array = 0x18011D48;
    constexpr auto name_array_padding = 0x4C70;
    constexpr auto name_array_size = 0xD0;
    constexpr auto ref_def_ptr = 0x18006508;
    constexpr auto score_base = 0x0;
    constexpr auto weapon_definition = 0x158A67A0;
    constexpr auto visible = 0x6300860;

    namespace bones
    {
        constexpr auto bone_base = 0x15F84;
        constexpr auto distribute = 0x0;
        constexpr auto size = 0x150;
        constexpr auto visible = 0x0;
    }

    namespace directx {
        constexpr auto command_queue = 0x19A8AD28;
        constexpr auto swap_chain = 0x19A900A0;
    }

    namespace other
    {
        constexpr auto recoil = 0x810BC;
    }

    namespace player {
        constexpr auto size = 0x3AB8;
        constexpr auto valid = 0x294;
        constexpr auto pos = 0x308;
        constexpr auto team = 0x2F0;
        constexpr auto stance = 0x2CF8;
        constexpr auto weapon_index = 0x3784;
        constexpr auto dead_1 = 0x28CC;
        constexpr auto dead_2 = 0x408;
    }
}