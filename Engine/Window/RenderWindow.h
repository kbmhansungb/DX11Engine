#pragma once
#include "ErrorLogger.h"
#include "Tool/SafePtr.h"
#include "Tool/Delegate.h"

class RenderWindow : public UseSafePtr
{
private:
    HWND handle;
public:
    RenderWindow(
        HINSTANCE hInstance,
        HWND parentHandle,
        LPCWSTR windowTitle,
        LPCWSTR wClass,
        int width,
        int height,
        DWORD style = WS_OVERLAPPEDWINDOW)//WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER)
    {
        RECT wr;
        wr.left = 30;
        wr.top = 30;
        wr.right = wr.left + width;
        wr.bottom = wr.top + height;
        AdjustWindowRect(&wr, style, false);

        handle = CreateWindow(
            wClass,
            windowTitle,
            style,
            wr.left, // pos x
            wr.top, // pos y
            wr.right - wr.left,
            wr.bottom - wr.top,
            parentHandle,
            nullptr,
            hInstance,
            this);

        ShowWindow(this->handle, SW_SHOW);
        SetForegroundWindow(this->handle);
        SetFocus(this->handle);
    }
    virtual ~RenderWindow()
    {
        if (this->handle != nullptr)
        {
            DestroyWindow(this->handle);
        }
    }
    bool ProcessMessages()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_NULL)
        {
            if (!IsWindow(this->handle))
            {
                this->handle = nullptr;
                return false;
            }
        }
        return true;
    }

	HWND GetHWND()
	{
		return this->handle;
	}
private:
};

