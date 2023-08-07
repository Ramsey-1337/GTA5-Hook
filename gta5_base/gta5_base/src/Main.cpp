#include <Includes.h>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndHandling(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(wnd_proc, hWnd, uMsg, wParam, lParam);
}

bool init = true;
bool open = false;

HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pdevice)))
        {
            pdevice->GetImmediateContext(&pcontext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            hwnd = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pdevice->CreateRenderTargetView(pBackBuffer, NULL, &rendertarget);
            pBackBuffer->Release();
            ////////////////////////////////////////////////////////////////////

            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14);

            ////////////////////////////////////////////////////////////////////
            wnd_proc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndHandling);
            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplDX11_Init(pdevice, pcontext);

            init = 0;
        }
        else
            return opresent(pSwapChain, SyncInterval, Flags);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //Open key for the Menu
    if (GetAsyncKeyState(VK_INSERT) & 1)
    {
        m_gui->m_open = !m_gui->m_open;
    }
 


    //Blocking Inputs & Drawing the Cursor
    ImGui::GetIO().WantCaptureKeyboard = m_gui->m_open;
    ImGui::GetIO().WantCaptureMouse = m_gui->m_open;
    ImGui::GetIO().MouseDrawCursor = m_gui->m_open;

    //Render Menu
    m_gui->render_menu();
 
    ImGui::Render();

    pcontext->OMSetRenderTargets(1, &rendertarget, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return opresent(pSwapChain, SyncInterval, Flags);
}



void Hook_Thread()
{
    swapchain_vtables = *(DWORD_PTR**)(g_pattern->g_swapchain);
    swapchain_vtables = (DWORD_PTR*)(swapchain_vtables[0]);
    swapchain_backup = swapchain_vtables[8];
    VirtualProtect((LPVOID)swapchain_vtables, sizeof(DWORD_PTR), (DWORD)PAGE_EXECUTE_READWRITE, (PDWORD)&dw_old);
    opresent = reinterpret_cast<OriginalFnPresent>(swapchain_vtables[8]);
    swapchain_vtables[8] = (uintptr_t)&Present;
    VirtualProtect((LPVOID)swapchain_vtables, sizeof(DWORD_PTR), (DWORD)dw_old, (PDWORD)&dw_old);
}

BOOL DllMain(HMODULE hmod, DWORD dwreason, LPVOID lpreversed)
{
    dwreason;
    lpreversed;
    if (dwreason == 1)
    {
        g_pattern->Initialize();
        _beginthreadex(0, 0, (_beginthreadex_proc_type)Hook_Thread, 0, 0, 0);
    }
    return 1;
}