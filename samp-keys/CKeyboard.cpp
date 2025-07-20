#include "pch.h"
#include "CKeyboard.h"

std::vector<CKey*> CKeyboard::vKeyboardListener;

void CKeyboard::ProcessKeyboard(UINT keyId)
{
	Sync::SendPressedKey(keyId);
}

void CKeyboard::InitKeys()
{
	for (UINT i = 0; i < 254; i++) {
		UINT keyCode = MapVirtualKeyW(i, MAPVK_VK_TO_VSC);
		if (!keyCode) return;

		CKey* keyPtr = new CKey(keyCode, CKeyboard::ProcessKeyboard);
		vKeyboardListener.push_back(keyPtr);
	}
}
