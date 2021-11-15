#include "Engine/_Exclude.h"

using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// CoInitialize 21.11.15
	// 
#pragma region Initialize

	// 코이니셜라이즈 테스트 21.11.15
	// 
	// CoInitialize란?
	// https://docs.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-coinitialize
	//
	// ComPtr을 사용하기 위해서 COM 라이브러리를 초기화
	if (FAILED(CoInitialize(nullptr)))
	{
		ErrorLogger::Log(GetLastError(), "Failed to call ConInitialize.");
		return -1;
	}

	// 윈도우 클래스 생성, 프로그램이 종료될 때 윈도우 클래스를 해제함. 21.11.15
	RegistWindowClassExW T(HandleMessageSetup, hInstance, L"Basic");

#pragma endregion

	// 엔진을 생성, 에디터 생성, 루프콜 실행 21.11.15
#pragma region CreateEditor

	unique_ptr<Engine> engine = make_unique<Engine>(hInstance);
	EDITOR::Shared::get_shared()->engine = engine.get();

	EDITOR::construct_main_scene(engine.get());
	
	EDITOR::create_editor_window(
		engine.get(),
		EDITOR::CMS::get_instance()->main_scene
	);

	engine->LoopCall();
#pragma endregion

	return 0;
}