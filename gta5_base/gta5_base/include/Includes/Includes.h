#pragma once
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <thread>
#include <vector>
#include <filesystem>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <Psapi.h>
#include <xmmintrin.h>
#include <DirectXMath.h>
#include <d3d11.h>

#include <Memory.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>

#include <Pattern.h>
#include <Gui.h>

typedef HRESULT(__stdcall* OriginalFnPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

inline OriginalFnPresent opresent{ 0 };
inline HWND hwnd{ 0 };
inline ID3D11Device* pdevice{ 0 };
inline ID3D11DeviceContext* pcontext{ 0 };
inline ID3D11RenderTargetView* rendertarget{ 0 };
inline WNDPROC wnd_proc{ 0 };
inline DWORD_PTR swapchain_backup = 0x0;
inline uintptr_t* swapchain_vtables{ 0 };
inline DWORD dw_old = 0x0;