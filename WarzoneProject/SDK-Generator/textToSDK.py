import os
import re

o_replace = ['sVersion', 'iCamBase', 'iCamPos', 'iGameMode', 'iLocalIndex', 'iIndexPos', 'iNameArray', 'iNAPadding', 'iRefDef', 'iWeaponDefinition', 'iVisible', 'iBoneBase', 'iBoneSize', 'iCommand', 'iSwapChain', 'iRecoil', 'iPlayerSize', 'iPlayerValid', 'iPlayerPos', 'iPlayerTeam', 'iPlayerStance', 'iPlayerIndex', 'iPlayerDead1', 'iPlayerDead2']
o_replace1 = ['Version', 'camera_base = ', 'camera_pos = ', 'game_mode = ', 'local_index = ', 'local_index_pos = ', 'name_array = ', 'name_array_pos = ', 'refdef = ', 'weapon_definitions = ', 'visible = ', 'base_pos = ', 'index_struct_size = ', 'command_queue = ', 'swap_chain = ', 'recoil = ', 'size = ', 'valid = ', 'constexpr auto pos = ', 'team = ', 'stance = ', 'weapon_index = ', 'dead_1 = ', 'dead_2 = ']
o_replace2 = ['c', ';',';',';',';',';',';',';',';',';',';', ';',';',';',';',';',';',';',';',';',';',';',';',';']

sdk_replace = ['ClientInfoData', 'CLBase', 'GetBone', 'BoneIndex']
sdk_replace1 = ['extern "C" auto decrypt_client_info(uint64_t imageBase, uint64_t sdk::peb) -> uint64_t  {      ', 'extern "C" auto decrypt_client_base(uint64_t sdk::client_info, uint64_t imageBase, uint64_t sdk::peb) -> uint64_t  {      ', 'extern "C" auto decrypt_bone_base(uint64_t imageBase, uint64_t sdk::peb) -> uint64_t  {      ', 'extern "C" auto get_bone_index(uint32_t index, uint64_t imageBase) -> uint64_t  {      ']
sdk_replace2 = ['return RBX;  }', '          return RAX;      }      }  }', '          return RDX;      }      }  }', '      return RSI;  }']

sdkRAW_replace = ['readMemory', 'peb', 'clientInfo']
sdkRAW_replace1 = ['driver::read', 'sdk::peb', 'sdk::client_info']

def inBetween(str, start, end):
    start_index = str.find(start) + len(start)
    end_index = str.find(end)
    return str[start_index:end_index]

def formatOffsets(template, dump):
    for i in range(0, len(o_replace)):
        for item in dump:
            if re.search(o_replace1[i], item):
                template = template.replace(o_replace[i], inBetween(item, o_replace1[i], o_replace2[i]))
    print("Offsets.h created!")
    return template

def createSDK(dump):
    SDKDump = dump

    for item in SDKDump:
        if re.search('//', item):
            #SDKDump.remove(item)
            SDKDump[SDKDump.index(item)] = item.replace("//" + item.split('//')[1], '')

    for i in range(0, len(sdkRAW_replace)):
        for item in SDKDump:
            if re.search(sdkRAW_replace[i], item):
                SDKDump[SDKDump.index(item)] = item.replace(sdkRAW_replace[i], sdkRAW_replace1[i])   

    SDKDump = ' '.join(SDKDump)

    print("raw SDK Dumped!")
    return SDKDump

def formatSDK(template, dump):
    dump = dump.strip()
    
    #DEBUG
    #test = open("C:\\Users\\dynam\\Game Reversal\\COD-Warzone\\SDK\\WarzoneSDK-Dump\\debugDump.txt", "w") #Create File
    #test.write(dump)
    #test.close()
    
    buffer = template.replace(sdk_replace[0], inBetween(dump, sdk_replace1[0], sdk_replace2[0]) + 'return RBX;')
    buffer2 = buffer.replace(sdk_replace[1], inBetween(dump, sdk_replace1[1], sdk_replace2[1]) + 'return RAX; } ')
    buffer3 = buffer2.replace(sdk_replace[2], inBetween(dump, sdk_replace1[2], sdk_replace2[2]) + 'return RDX;} }')
    buffer4 = buffer3.replace(sdk_replace[3], inBetween(dump, sdk_replace1[3], sdk_replace2[3]) + 'return RSI;')

    print("Decryption.cpp created!")
    return buffer4

def main():
    template = """#pragma once

// Version sVersion

namespace offsets
{
    constexpr auto camera_base = iCamBase;
    constexpr auto camera_pos = iCamPos;
    constexpr auto game_mode = iGameMode;
    constexpr auto local_index = iLocalIndex;
    constexpr auto local_index_pos = iIndexPos;
    constexpr auto name_array = iNameArray;
    constexpr auto name_array_padding = iNAPadding;
    constexpr auto name_array_size = 0xD0;
    constexpr auto ref_def_ptr = iRefDef;
    constexpr auto score_base = 0x0;
    constexpr auto weapon_definition = iWeaponDefinition;
    constexpr auto visible = iVisible;
    
    namespace bones
    {
        constexpr auto bone_base = iBoneBase;
        constexpr auto distribute = 0x0;
        constexpr auto size = iBoneSize;
        constexpr auto visible = 0x0;
    }

    namespace directx {
        constexpr auto command_queue = iCommand;
        constexpr auto swap_chain = iSwapChain;
    }

    namespace other
    {
        constexpr auto recoil = iRecoil;
    }

    namespace player {
        constexpr auto size = iPlayerSize;
        constexpr auto valid = iPlayerValid;
        constexpr auto pos = iPlayerPos;
        constexpr auto team = iPlayerTeam;
        constexpr auto stance = iPlayerStance;
        constexpr auto weapon_index = iPlayerIndex;
        constexpr auto dead_1 = iPlayerDead1;
        constexpr auto dead_2 = iPlayerDead2;
    }
}"""

    SDK_template = """#include "sdk.h"
#include "..\driver\driver.h"
#include "offsets.h"

namespace decryption {
	uintptr_t get_client_info() {
		uintptr_t imageBase = sdk::module_base;
        ClientInfoData
    }

	uintptr_t get_client_info_base() {
		uintptr_t imageBase = sdk::module_base;

        CLBase

		default:
			return 0;
		}
	}

	uint64_t get_bone() {
		uintptr_t imageBase = sdk::module_base;

        GetBone
	}

	uint32_t get_bone_index(uint32_t index) {
		uintptr_t imageBase = sdk::module_base;

        BoneIndex
	}

	struct ref_def_key {
		int ref0;
		int ref1;
		int ref2;
	};

	uintptr_t get_ref_def() {
		ref_def_key crypt = driver::read<ref_def_key>(sdk::module_base + offsets::ref_def_ptr);
		uint64_t baseAddr = sdk::module_base;

		DWORD lower = crypt.ref0 ^ (crypt.ref2 ^ (uint64_t)(baseAddr + offsets::ref_def_ptr)) * ((crypt.ref2 ^ (uint64_t)(baseAddr + offsets::ref_def_ptr)) + 2);
		DWORD upper = crypt.ref1 ^ (crypt.ref2 ^ (uint64_t)(baseAddr + offsets::ref_def_ptr + 0x4)) * ((crypt.ref2 ^ (uint64_t)(baseAddr + offsets::ref_def_ptr + 0x4)) + 2);

		return (uint64_t)upper << 32 | lower; 
	}
}
    """
    scriptPath = str(os.path.dirname(__file__))

    SDKPath = scriptPath + "\\WarzoneSDK-Dump\\"

    if not os.path.exists(SDKPath):
        os.makedirs(SDKPath)

    input("Paste draven's dump into a file called draven.txt in the same directory as this script")
    
    #Read Original Dump by Draven
    dravenTxt = open(scriptPath + "\\draven.txt", "r") #Get Raw Dump Data
    rawDump = dravenTxt.readlines()
    dravenTxt.close()

    #Create Offsets.h File
    offsets = open(SDKPath + "Offsets.h", "w")

    offsets.write(formatOffsets(template, rawDump))

    offsets.close()

    #Create SDK Dump File, replaced namespaces, classes, etc.
    sdk = open(SDKPath + "rawSDK.txt", "w")

    sdk.write(createSDK(rawDump))

    sdk.close()


    #Convert rawSDK into a one line string
    file = open(SDKPath + "rawSDK.txt", "r")

    SDKdata = file.read().replace('\n', ' ') #Retrieve SDK Data with replaced namespaces/renamed vars

    file.close()

    #Remove Temporary rawSDK.txt
    if os.path.exists(SDKPath + "rawSDK.txt"):
        os.remove(SDKPath + "rawSDK.txt")

    #Create Decryption.cpp
    decrypt = open(SDKPath + "decryption.cpp", "w")

    decrypt.write(formatSDK(SDK_template, SDKdata))

    decrypt.close()

if __name__ == '__main__':
    main()