#include <Pattern.h>

void Pattern::Initialize() {
	g_swapchain = g_memory->find_pattern(0, "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", 7, 0); 
}