#pragma once

const std::array<uintptr_t, 2> g_array_addrs = { {
	{ 0x53BEE0 }, // game loop
	{ 0x60BF0 } // KeyPressHandler (SAMP)
} };

class CHooks
{
public:
	static bool InitGameLoopHook();
	static bool InitializeMinHook();
	static bool InitKeyPressHandler();
};

