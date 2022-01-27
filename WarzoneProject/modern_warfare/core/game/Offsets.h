#pragma once

// Version  1.43.2.10430694

namespace offsets
{
    constexpr auto camera_base = 0x1B8BB090;
    constexpr auto camera_pos = 0x1D8;
    constexpr auto game_mode = 0x1EAE578;
    constexpr auto local_index = 0x6928;
    constexpr auto local_index_pos = 0x880;
    constexpr auto name_array = 0x1EF76E90;
    constexpr auto name_array_padding = 0x4C70;
    constexpr auto name_array_size = 0xD0;
    constexpr auto ref_def_ptr = 0x1EF6B100;
    constexpr auto score_base = 0x0;
    constexpr auto weapon_definition = 0x1B965E20;
    constexpr auto visible = 0x6793B30;
    
    namespace bones
    {
        constexpr auto bone_base = 0x6C2C;
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
        constexpr auto recoil = 0x19B88;
    }

    namespace player {
        constexpr auto size = 0x60C0;
        constexpr auto valid = 0x7C;
        constexpr auto pos = 0x308;
        constexpr auto team = 0x84;
        constexpr auto stance = 0xCB8;
        constexpr auto weapon_index = 0x3784;
        constexpr auto dead_1 = 0xAA8;
        constexpr auto dead_2 = 0x668;
    }
}