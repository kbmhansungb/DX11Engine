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
    // �θ� ���÷��̰� �ִ� ��� �θ� ���÷����� ����� ������ 21.11.15
    HWND parentHandle = nullptr;
    if (parentDisplay.is_vaild()) parentHandle = parentDisplay.get()->renderWindow->GetHWND();

    // �����츦 ������ 21.11.15
    renderWindow = make_unique<WindowInputContainer>(
        get_owner()->this_scene->this_engine->hInstance,
        parentHandle,
        wTitle.c_str(),
        wClass.c_str(),
        windowWidth,
        windowHeight);

    // �������� ��������Ʈ�� ���ε� 21.11.15
    {
        // �������� ������ ���� ��
        renderWindow->Delegate_window_size_changed
            .AddInvoker(this, &Display::resize_window);

        // �����찡 ����� ��
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
    // ���� �����찡 ������ ���� ��� ���÷��� ������Ʈ�� ��Ȱ�� ���·� ���� 21.11.15
    if (renderWindow.get() == nullptr)
    {
        this->active = false;
        return;
    }

    // ���������찡 ���� �޼����� ��ȯ�� ��� ���÷��̸� ������ 21.11.15
    if (renderWindow->ProcessMessages() == false)
    {
        window_close_message();
        return;
    }

    // ������Ʈ�� ������ ��ȿ�� ��� ����̽� �Է��� ó���� 21.11.15
    this->renderWindow->callback();
}

void Display::window_close_message()
{
    // ���� ���÷����� ��� ������ �������� �����ϰ�
    // �ƴ� ��� �����츦 ������ 
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
    // ���÷����� ũ�� ���� 21.11.15
    windowWidth = new_width;
    windowHeight = new_height;

    // ���÷��� ũ�� ���� ��������Ʈ ���� 21.11.15
    delegate_resize_window.Invoke(new_width, new_height);
}

void Display::draw_detail_view()
{
    Component::draw_detail_view();

    if (ImGui::CollapsingHeader("Display"))
    {
        // Val_Display �� ���� 21.11.15

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
