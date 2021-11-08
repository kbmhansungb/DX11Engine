#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// window class template
// 
// ���� ������� ����������� �޶�� �Ѵ�. ���̾� �α׷� ������ �Ѵ�.
// ��ǲ�� �ϳ��� �־�� �Ѵ�. ��. 
// �ϴ� API ���� ��ü�� �߸��Ǿ� ����.
// �˰ڽ��ϴ�.
// ��. ��.
// �ϴ� �׷��� �ٲٰ� Ȯ���� ���ڽ��ϴ�.

//CreateDialog
// ���ʿ� ��Ŀ���� ���¹��
// ��޸��� ���.
//DialogBox
// ���ʿ��� ��Ŀ���� �Ȱ��� ���
// ��� ���

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