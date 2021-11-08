#pragma once

#include "../_Include.h"

class Display final : public Component
{
public:
    SafePtr<Display> parentDisplay;
    LPCWSTR wTitle, wClass;
    UINT windowWidth, windowHeight;
    bool mainDisplay;

public:
    std::unique_ptr<WindowInputContainer> renderWindow;
    Display();
	Display(
        SafePtr<Display> parentDisplay, LPCWSTR wTitle, LPCWSTR wClass,
        UINT windowWidth, UINT windowHeight,
        bool mainDisplay = false);
    virtual ~Display();
    virtual std::type_index get_class_type();
    virtual void awake() override;
    virtual void sleep() override;

    virtual void update() override;

    void window_close_message();
    void resize_window(UINT new_width, UINT new_height);
    Delegate::delegate<UINT, UINT> delegate_resize_window;
};
REGIST_COMPONENT(Display);