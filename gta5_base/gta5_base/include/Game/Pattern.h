#pragma once
#include <Includes.h>

class Pattern {
public:
	uintptr_t g_swapchain;
	void Initialize();
};

inline Pattern* g_pattern = new Pattern; 