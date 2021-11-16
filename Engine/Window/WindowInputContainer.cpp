#include "WindowInputContainer.h"
#include <memory>

using namespace std;

WindowInputContainer::WindowInputContainer(HINSTANCE hInstance, HWND parentHandle, LPCWSTR windowTitle, LPCWSTR wClass, int width, int height, DWORD style) // 여러 문제가 있지만.. 뭐.. 암튼.
																																			   //DWORD style = WS_SIZEBOX)
																																			   //DWORD style = WS_POPUP | WS_BORDER)
	: RenderWindow(hInstance, parentHandle, windowTitle, wClass, width, height, style)
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; // mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = nullptr;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false)
		{
			ErrorLogger::Log(GetLastError(), "Failed to register raw input deivecs.");
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

WindowInputContainer::~WindowInputContainer()
{
	this->destroy_window();
}

void WindowInputContainer::callback()
{
	while (input.keyboard.CharBufferIsEmpty() == false)
	{
		unsigned char cha = input.keyboard.ReadChar();
		this->Delegate_keyborad_cha.Invoke(cha);
	}
	while (input.keyboard.KeyBufferIsEmpty() == false)
	{
		KeyboardEvent ke = input.keyboard.ReadKey();
		this->Delegate_keyboard_event.Invoke(ke);
	}
	while (input.mouse.EventBufferIsEmpty() == false)
	{
		MouseEvent me = input.mouse.ReadEvent();
		this->Delegate_mouse_event.Invoke(me);
	}
}

void WindowInputContainer::record(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//case WM_SIZE:
	//{
	//	UINT width = LOWORD(lParam);
	//	UINT height = HIWORD(lParam);
	//	this->Delegate_window_size_changed.Invoke(hwnd, width, height);
	//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	//}
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (input.keyboard.IsKeysAutoRepeat())
		{
			input.keyboard.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				input.keyboard.OnKeyPressed(keycode);
			}
		}
		break;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		input.keyboard.OnKeyReleased(keycode);
		break;
	}
	case WM_CHAR:
	{
		unsigned char cha = static_cast<unsigned char>(wParam);
		if (input.keyboard.IsCharsAutoRepeat())
		{
			input.keyboard.OnChar(cha);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				input.keyboard.OnChar(cha);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnLeftPressed(x, y);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnLeftReleased(x, y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnMiddlePressed(x, y);
		break;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnMiddleReleased(x, y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnRightPressed(x, y);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnRightReleased(x, y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			input.mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			input.mouse.OnWheelDown(x, y);
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		input.mouse.OnMouseMove(x, y);
		break;
	}
	case WM_INPUT:
	{
		UINT dataSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			//shared_ptr;
			//weak_ptr;			
			unique_ptr<BYTE[]> rawdata = make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					input.mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		break;
	}
	}
}
//static 변수 설정.
//Input WindowInputContainer::input;
//
LRESULT WindowInputContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//auto p = this;
	// 메세지 처리
	switch (uMsg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		this->Delegate_window_size_changed.Invoke(width, height);

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (input.keyboard.IsKeysAutoRepeat())
		{
			input.keyboard.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				input.keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		input.keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char cha = static_cast<unsigned char>(wParam);
		if (input.keyboard.IsCharsAutoRepeat())
		{
			input.keyboard.OnChar(cha);
		}
		else
		{			
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				input.keyboard.OnChar(cha);
			}
		}
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		input.mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			input.mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			input.mouse.OnWheelDown(x, y);
		}
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		input.mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_INPUT:
	{
		UINT dataSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0)
		{
			//shared_ptr;
			//weak_ptr;			
			unique_ptr<BYTE[]> rawdata = make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					input.mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	case WM_CLOSE:
		Delegate_window_close.Invoke();
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}    
}

void WindowInputContainer::destroy_window()
{
	if (this->GetHWND() != nullptr)
		DestroyWindow(this->GetHWND());
}
