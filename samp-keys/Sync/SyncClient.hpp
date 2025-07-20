#pragma once

#include <cstdint>
#include <windows.h>

#include "../BitStream.h"
#include "raknet/RakClient.h"

namespace Sync {
    typedef uint16_t ID;

    inline bool init = false;
    inline int INPUT_KEYS_PACKET = 201;
    inline int PRESSED_PACKET = 200;

    inline uintptr_t getSampModule() {
        static uintptr_t sampDll = 0;
        if (!sampDll)
            sampDll = reinterpret_cast<uintptr_t>(GetModuleHandleA("samp.dll"));
        return sampDll;
    }

    inline uintptr_t getSampEntryPointOffset() {
        static uintptr_t entrypointOffset = 0x0;
        if (entrypointOffset == 0x0) {
            auto base = getSampModule();
            if (base != 0x0) {
                auto ntheader = reinterpret_cast<IMAGE_NT_HEADERS*>(base + reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
                entrypointOffset = ntheader->OptionalHeader.AddressOfEntryPoint;
            }
        }
        return entrypointOffset;
    }

    inline uintptr_t getSampNetGame() {
        auto samp_module = getSampModule();
        if (samp_module == 0) return 0;
        if (getSampEntryPointOffset() != 0xCC4D0) return 0;
        return *reinterpret_cast<uintptr_t*>(samp_module + 0x26E8DC);
    }

    inline RakClientInterface* getRakClientIntf() {
        auto samp_module = getSampModule();
        auto samp_netgame = getSampNetGame();
        if (samp_netgame == 0) return nullptr;
        return reinterpret_cast<RakClientInterface * (__thiscall*)(uintptr_t)>
            (samp_module + 0x1A40)(samp_netgame);
    }

    inline void SendPressedKey(UINT key) {
        if (!getRakClientIntf()) return;
        
        BitStream bs;

        bs.Write<unsigned int>(key);

        getRakClientIntf()->RPC(&PRESSED_PACKET, &bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_SEQUENCED, 0, false);

        return;
    }

    inline void OnIncomingInputKeys(RPCParameters* params) {
        BitStream bs(params->input, BITS_TO_BYTES(params->numberOfBitsOfData), false);

        UINT keyId = 0;
        bs.Read<UINT>(keyId);

        //CKeyboard::InitKey(VK_TAB);
    }

    inline void registerPackets() {
        if (!init && getSampModule() && getRakClientIntf() != nullptr) {
            getRakClientIntf()->RegisterAsRemoteProcedureCall(&INPUT_KEYS_PACKET, &OnIncomingInputKeys);
            init = true;
        }
    }
}
