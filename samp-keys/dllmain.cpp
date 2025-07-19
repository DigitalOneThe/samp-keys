// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

HHOOK g_keyboardHook = nullptr;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    printf("wParam: %d\n", wParam);
    
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = kbStruct->vkCode;

        for (auto pair : CKeyboard::vKeyboardListener) {
            if (vkCode == pair->m_dKeyId) {
                pair->m_fCallback(wParam);
            }
        }
    }

    return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI Initialize(LPVOID param) {
    while (*reinterpret_cast<unsigned char*>(0xC8D4C0) != 9) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1u));
    }

    printf("init success\n");

    //CMessages::AddMessageJumpQ((char*)"хуй", 6000, 0, false);

    HMODULE hDll = GetModuleHandle(L"samp.dll");
    if (!hDll) {
        printf("!hDll\n");
    }

    auto samp_handle = reinterpret_cast<DWORD>(GetModuleHandleA("samp.dll"));
    memwrapper::write_memory<int>(samp_handle + 0xE40E, 20000);

    g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hDll, 0);
    if (!g_keyboardHook) {
        printf("Hook failed! Error: %d\n", GetLastError());
        return 0;
    }

    CKeyboard::InitKeys();
    
    if (!CHooks::InitGameLoopHook()) {
        printf("GameLoop not initialized!\n");
    }

    if (!CHooks::InitKeyPressHandler()) {
        printf("KeyPressHandler not initialized!\n");
    }

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

