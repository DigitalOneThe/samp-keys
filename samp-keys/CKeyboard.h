#pragma once
#include <map>

class CKey {
public:
	UINT m_dKeyId;
	void (*m_fCallback)(UINT);

	CKey(UINT keyId, void(*callback)(UINT)) {
		this->m_dKeyId = keyId;
		this->m_fCallback = callback;
	}

	~CKey() {
		this->m_dKeyId = NULL;
		this->m_fCallback = nullptr;
	}
};

class CKeyboard
{
public:
	static std::vector<CKey*> vKeyboardListener;

	static void ProcessKeyboard(UINT keyId);
	static void InitKeys();
};

