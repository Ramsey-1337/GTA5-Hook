#pragma once
#include <Includes.h>

class Memory {
public:
	uintptr_t find_pattern_ex(uintptr_t baseaddy, const char* pattern, size_t index);
	uintptr_t relative_addr(uintptr_t addr, int length);
	uintptr_t find_pattern(const char* szModuleName, const char* Pattern, int Index, size_t index);
	uintptr_t get_module(LPCSTR sz_modulename);
	DWORD_PTR get_proc_base(DWORD procID);
};

inline Memory* g_memory = new Memory;