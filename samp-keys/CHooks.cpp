#include "pch.h"
#include "CHooks.h"

typedef void(__thiscall* CGame__Loop_def)();
CGame__Loop_def cGameLoop = nullptr;

typedef void(__cdecl* KeyPressHandler_def)(unsigned int nKey);
KeyPressHandler_def keyPressHandler = nullptr;

void __fastcall CGame__LoopHook() {
	Sync::registerPackets();
	return cGameLoop();
}

void KeyPressHandler__Hook(unsigned int nKey, void* edx) {
	for (const CKey* key : CKeyboard::vKeyboardListener) {
		if (key->m_dKeyId == nKey) {
			key->m_fCallback(nKey);
			break;
		}
	}
	
	return keyPressHandler(nKey);
}

bool CHooks::InitGameLoopHook()
{
	if (MH_CreateHook((LPVOID)g_array_addrs[0], &CGame__LoopHook,
		reinterpret_cast<LPVOID*>(&cGameLoop)) != MH_OK) {
		return false;
	}

	if (MH_EnableHook((LPVOID)g_array_addrs[0]) != MH_OK) {
		return false;
	}

	return true;
}

bool CHooks::InitializeMinHook()
{
	if (MH_Initialize() != MH_OK) {
		return false;
	}

	return true;
}

bool CHooks::InitKeyPressHandler()
{
	HMODULE hSamp = GetModuleHandleA("samp.dll");
	if (!hSamp)
	{
		return false;
	}

	void* pTarget = (void*)((uintptr_t)hSamp + g_array_addrs[1]);

	if (MH_CreateHook(pTarget, &KeyPressHandler__Hook, reinterpret_cast<LPVOID*>(&keyPressHandler)) != MH_OK)
	{
		return false;
	}

	if (MH_EnableHook(pTarget) != MH_OK)
	{
		return false;
	}

	return true;
}
