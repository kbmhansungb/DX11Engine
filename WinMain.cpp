#include "Engine/_Exclude.h"

using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
#pragma region 안정성

	if (FAILED(CoInitialize(nullptr)))
	{
		ErrorLogger::Log(GetLastError(), "Failed to call ConInitialize.");
		return -1;
	}
	RegistWindowClassExW T(HandleMessageSetup, hInstance, L"Basic");

#pragma endregion
#pragma region 초기화

#pragma endregion
#pragma region 엔진

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