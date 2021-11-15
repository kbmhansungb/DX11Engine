#pragma once

#include "../_Include.h"

/// <summary>
/// 윈도우를 생성하고 스왑체인을 만들기 위한 컴포넌트
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

    // 디스플레이의 생성 정보와 생성된 인스턴스를 보관하는 변수
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

    // 디스플레이 관련 함수
    // 21.11.15
#pragma region Func_DisplayFunction

public:
    /// <summary>
    /// 바인드 메서드로 윈도우 종료시 호출 됨
    /// 윈도우를 종료하고 컴포넌트를 비활성화 시킴
    /// 21.11.15
    /// </summary>
    void window_close_message();

    /// <summary>
    /// 바인드 메서드로 윈도우 사이즈 변경 시 호출됨
    /// 윈도우의 사이즈 변경
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