#pragma once
#include "RenderWindow.h"
#include "Input/Input.h"
#include "Tool/Delegate.h"
#include "../ImGui_Sort/_Exclude.h"

class WindowInputContainer : public RenderWindow
{
public:
    WindowInputContainer(
        HINSTANCE hInstance,
        HWND parentHandle,
        LPCWSTR windowTitle,
        LPCWSTR wClass,
        int width, int height,
        DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER | WS_SIZEBOX);
    virtual ~WindowInputContainer();
    void callback();
    void record(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    Input input;

    void destroy_window();
    Delegate::delegate<unsigned char> Delegate_keyborad_cha;
    Delegate::delegate<KeyboardEvent> Delegate_keyboard_event;
    Delegate::delegate<MouseEvent> Delegate_mouse_event;
    Delegate::delegate<UINT, UINT> Delegate_window_size_changed;
    Delegate::delegate<> Delegate_window_close;
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowInputContainer* const pWindow = reinterpret_cast<WindowInputContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
    {
        //pWindow->record(hwnd, uMsg, wParam, lParam);
        return true;
    }
    //ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_CLOSE:
        pWindow->Delegate_window_close.Invoke();
        //DestroyWindow(hwnd);
        return 0;
    default:
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
}

//static LRESULT CALLBACK ViewportProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
//    {
//        return 0;
//    }
//    return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}

static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowInputContainer* pWindow = reinterpret_cast<WindowInputContainer*>(pCreate->lpCreateParams);
        if (pWindow == nullptr)
        {
            ErrorLogger::Log("Critical Error: Pointer to window contatiner is null during WM_NCCREATE.");
            exit(-1);
        }
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}