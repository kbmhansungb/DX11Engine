#include "Display.h"

using namespace std;

Display::Display() :
    parentDisplay(SafePtr<Display>()), wTitle(L""), wClass(L""),
    windowWidth(800), windowHeight(600),
    mainDisplay(false)
{}

Display::Display(
    SafePtr<Display> parentDisplay, LPCWSTR wTitle, LPCWSTR wClass, 
    UINT windowWidth, UINT windowHeight, 
    bool mainDisplay) :
    parentDisplay(parentDisplay), wTitle(wTitle), wClass(wClass),
    windowWidth(windowWidth), windowHeight(windowHeight),
    mainDisplay(mainDisplay)
{
}

Display::~Display() {}

void Display::awake()
{
    // 부모 디스플레이가 있는 경우 부모 디스플레이의 헨들을 가져옴 21.11.15
    HWND parentHandle = nullptr;
    if (parentDisplay.is_vaild()) parentHandle = parentDisplay.get()->renderWindow->GetHWND();

    // 윈도우를 생성함 21.11.15
    renderWindow = make_unique<WindowInputContainer>(
        get_owner()->this_scene->this_engine->hInstance,
        parentHandle,
        wTitle.c_str(),
        wClass.c_str(),
        windowWidth,
        windowHeight);

    // 윈도우의 델리게이트를 바인드 21.11.15
    {
        // 윈도우의 사이즈 변경 시
        renderWindow->Delegate_window_size_changed
            .AddInvoker(this, &Display::resize_window);

        // 윈도우가 종료될 시
        renderWindow->Delegate_window_close
            .AddInvoker(this, &Display::window_close_message);
    }
}

void Display::sleep()
{
    renderWindow.reset();
}

void Display::update()
{
    // 렌더 윈도우가 비정상 적인 경우 디스플레이 컴포넌트를 비활성 상태로 만듬 21.11.15
    if (renderWindow.get() == nullptr)
    {
        this->active = false;
        return;
    }

    // 렌더윈도우가 종료 메세지를 반환한 경우 디스플레이를 종료함 21.11.15
    if (renderWindow->ProcessMessages() == false)
    {
        window_close_message();
        return;
    }

    // 컴포넌트가 여전히 유효한 경우 디바이스 입력을 처리함 21.11.15
    this->renderWindow->callback();
}

void Display::window_close_message()
{
    // 메인 디스플레이인 경우 엔진의 루프문을 종료하고
    // 아닐 경우 윈도우를 제거함 
    // 21.11.16
    if (mainDisplay)
    {
        owner->this_scene->this_engine->engine_activation = false;
    }
    else
    {
        this->active = false;
    }
}

void Display::resize_window(UINT new_width, UINT new_height)
{
    // 디스플레이의 크기 변경 21.11.15
    windowWidth = new_width;
    windowHeight = new_height;

    // 디스플레이 크기 변경 델리게이트 전파 21.11.15
    delegate_resize_window.Invoke(new_width, new_height);
}

void Display::draw_detail_view()
{
    Component::draw_detail_view();

    if (ImGui::CollapsingHeader("Display"))
    {
        // Val_Display 값 변경 21.11.15

        ImGui::base_field(parentDisplay, "Parent display");
        
        ImGui::wstring_field(wTitle, "Title", 100);
        
        ImGui::wstring_field(wClass, "Class", 100);
        
        int Width = windowWidth;
        ImGui::INT_field(&Width, "Width");
        windowWidth = max(0, Width);

        int Height = windowHeight;
        ImGui::INT_field(&Height, "Height");
        windowHeight = max(0, Height);

        string main_display_name = "Main display##" + StringHelper::ptr_to_string(&mainDisplay);
        ImGui::Checkbox(main_display_name.c_str(), &mainDisplay);
    }
}
