#include "pch.h"
#include "CKeyboard.h"

std::vector<CKey*> CKeyboard::vKeyboardListener;

void CKeyboard::ProcessKeyboard(UINT keyId)
{
	//printf("key pressed: %d", keyId);

	AVSSync::SendPressedKey(keyId);
}

void CKeyboard::InitKeys()
{
	for (UINT vk = 0; vk < 254; vk++) {
		if (vk == VK_LBUTTON) continue;
		if (vk == VK_RBUTTON) continue;
		if (vk == VK_MBUTTON) continue;
		if (vk == VK_XBUTTON1) continue;
		if (vk == VK_XBUTTON2) continue;

		UINT keyCode = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);
		if (!keyCode) continue;
		
		CKey* keyPtr = new CKey(keyCode, CKeyboard::ProcessKeyboard);
		vKeyboardListener.push_back(keyPtr);
	}
}
