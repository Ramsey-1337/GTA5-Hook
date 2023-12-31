#include <Memory.h>

uintptr_t Memory::get_module(LPCSTR sz_modulename) {
	uintptr_t handle = (uintptr_t)GetModuleHandle(sz_modulename);
	if (handle != NULL) {
		return handle;
	}
	else {
		return 0;
	}
}

uintptr_t Memory::find_pattern_ex(uintptr_t baseaddy, const char* pattern, size_t index)
{
	static auto patternToByte = [](const char* pattern)
	{
		auto       bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else
				bytes.push_back(strtoul((const char*)current, &current, 16));
		}
		return bytes;
	};

	const auto dosHeader = (PIMAGE_DOS_HEADER)baseaddy;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)baseaddy + dosHeader->e_lfanew);

	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(pattern);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(baseaddy);

	const auto s = patternBytes.size();
	const auto d = patternBytes.data();

	size_t nFoundResults = 0;

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;

		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			if (index != 0)
			{
				if (nFoundResults < index)
				{
					nFoundResults++;
					found = false;
				}
				else
					return reinterpret_cast<uint64_t>(&scanBytes[i]);
			}
			else
				return reinterpret_cast<uint64_t>(&scanBytes[i]);
		}
	}

	return NULL;
}
uintptr_t Memory::relative_addr(uintptr_t addr, int length)
{
	DWORD Offset = *(DWORD*)(addr + (length - 4));
	return addr + length + Offset;
}

uintptr_t Memory::find_pattern(const char* szModuleName, const char* Pattern, int relative_addr_length, size_t index)
{
	auto ret = find_pattern_ex((uintptr_t)GetModuleHandleA(szModuleName), Pattern, index);

	if (relative_addr_length != 0)
		ret = relative_addr(ret, relative_addr_length);

	return ret;
}

DWORD_PTR Memory::get_proc_base(DWORD procID) {
	DWORD_PTR baseAddress = 0;
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	HMODULE* moduleArray;
	LPBYTE moduleArrayBytes;
	DWORD bytesRequired;
	if (processHandle)
	{
		if (K32EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);
				if (moduleArrayBytes)
				{
					unsigned int moduleCount;
					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE*)moduleArrayBytes;
					if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}
					LocalFree(moduleArrayBytes);
				}
			}
		}
		CloseHandle(processHandle);
	}
	return baseAddress;
}