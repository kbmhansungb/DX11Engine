#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// window class template
// 
// 메인 윈도우랑 서브윈도우는 달라야 한다. 다이얼 로그로 만들어야 한다.
// 인풋은 하나만 있어야 한다. 음. 
// 일단 API 구조 자체가 잘못되어 있음.
// 알겠습니다.
// 네. 네.
// 일단 그렇게 바꾸고 확인해 보겠습니다.

//CreateDialog
// 뒤쪽에 포커스가 가는방식
// 모달리스 방식.
//DialogBox
// 뒤쪽에는 포커스가 안가는 방식
// 모달 방식

class RegistWindowClassExW
{
private:
    RegistWindowClassExW() = delete;
public:
    HINSTANCE hInstance;
    LPCWSTR lpszClassName;

    ATOM result;
    RegistWindowClassExW(
        UINT cbSize,
        UINT style,
        WNDPROC lpfnWndProc,
        int cbClsExtra,
        int cbWndExtra,
        HINSTANCE hInstance,
        HICON hIcon,
        HICON hIconsm,
        HCURSOR hCursor,
        HBRUSH hbrBackground,
        LPCWSTR lpszMenuName,
        LPCWSTR lpszClassName
    ) :
        hInstance(hInstance),
        lpszClassName(lpszClassName)
    {
        WNDCLASSEX wc;
        wc.cbSize = cbSize;
        wc.style = style;
        wc.lpfnWndProc = lpfnWndProc;
        wc.cbClsExtra = cbClsExtra;
        wc.cbWndExtra = cbWndExtra;
        wc.hInstance = hInstance;
        wc.hIcon = hIcon;
        wc.hIconSm = hIconsm;
        wc.hCursor = hCursor;
        wc.hbrBackground = hbrBackground;
        wc.lpszMenuName = lpszMenuName;
        wc.lpszClassName = lpszClassName;

        result = RegisterClassExW(&wc);
    }
    RegistWindowClassExW(
        WNDPROC lpfnWndProc,
        HINSTANCE hInstance,
        LPCWSTR lpszClassName
    ) :
        hInstance(hInstance),
        lpszClassName(lpszClassName)
    {
        WNDCLASSEX wc
        {

        };
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_CLASSDC;
        wc.lpfnWndProc = lpfnWndProc;
        wc.cbClsExtra = 0L;
        wc.cbWndExtra = 0L;
        wc.hInstance = hInstance;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.lpszClassName = lpszClassName;
        wc.hIconSm = NULL;

        result = RegisterClassExW(&wc);
    }
    ~RegistWindowClassExW()
    {
        UnregisterClass(lpszClassName, hInstance);
    }
};