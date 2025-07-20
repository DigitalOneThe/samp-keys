// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

DWORD WINAPI Initialize(LPVOID param) {
    while (*reinterpret_cast<unsigned char*>(0xC8D4C0) != 9) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1u));
    }

    if (!CHooks::InitGameLoopHook()) {
        printf("GameLoop not initialized!\n");
    }

    if (!CHooks::InitKeyPressHandler()) {
        printf("KeyPressHandler not initialized!\n");
    }

    CKeyboard::InitKeys();

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        if (!CHooks::InitializeMinHook()) {
            printf("MinHook not initialized!\n");
        }

        CreateThread(0, 0, &Initialize, 0, 0, 0);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

