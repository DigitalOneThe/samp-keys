#pragma once

#define SAMP_VERSION_R1			(1)
#define SAMP_VERSION_R3			(2)
#define SAMP_VERSION_R5			(3)

struct SampFunctionAddrs {
	int version;
	uintptr_t gameLoopAddr;
	uintptr_t keyPressHandlerAddr;
};

const std::array<SampFunctionAddrs, 3> g_array_addrs = { {
	{ SAMP_VERSION_R1, 0x53BEE0, 0x5D850 },
	{ SAMP_VERSION_R3, 0x53BEE0, 0x60BF0 }, // game loop
	{ SAMP_VERSION_R5, 0x53BEE0, 0x61360 } // KeyPressHandler (SAMP)
} };

class CHooks
{
public:
	static bool InitGameLoopHook();
	static bool InitializeMinHook();
	static bool InitKeyPressHandler();
};

