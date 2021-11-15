#pragma once

#include "../_Include.h"

/// <summary>
/// �����츦 �����ϰ� ����ü���� ����� ���� ������Ʈ
/// 21.11.15
/// </summary>
class Display final : public Component
{
#pragma region ConDestructor

public:
    Display();
    Display(
        SafePtr<Display> parentDisplay, LPCWSTR wTitle, LPCWSTR wClass,
        UINT windowWidth, UINT windowHeight,
        bool mainDisplay = false);
    ~Display();

#pragma endregion

    // ���÷����� ���� ������ ������ �ν��Ͻ��� �����ϴ� ����
    // 21.11.15
#pragma region Val_Display

public:
    std::wstring wTitle, wClass;
    UINT windowWidth, windowHeight;
    std::unique_ptr<WindowInputContainer> renderWindow;

    bool mainDisplay;
    SafePtr<Display> parentDisplay;
#pragma endregion

public:
    Delegate::delegate<UINT, UINT> delegate_resize_window;

    // ���÷��� ���� �Լ�
    // 21.11.15
#pragma region Func_DisplayFunction

public:
    /// <summary>
    /// ���ε� �޼���� ������ ����� ȣ�� ��
    /// �����츦 �����ϰ� ������Ʈ�� ��Ȱ��ȭ ��Ŵ
    /// 21.11.15
    /// </summary>
    void window_close_message();

    /// <summary>
    /// ���ε� �޼���� ������ ������ ���� �� ȣ���
    /// �������� ������ ����
    /// </summary>
    /// <param name="new_width"></param>
    /// <param name="new_height"></param>
    void resize_window(UINT new_width, UINT new_height);

#pragma endregion


#pragma region Component

public:
    virtual void awake() override;
    virtual void sleep() override;
    virtual void update() override;
    virtual void draw_detail_view() override;

#pragma endregion

};
REGIST_COMPONENT(Display);