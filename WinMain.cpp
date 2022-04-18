#include "Engine/_Exclude.h"

using namespace std;

int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow)
{
#pragma region Initialize

	// coinitialize
	// https://docs.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-coinitialize
	if (FAILED(CoInitialize(nullptr)))
	{
		ErrorLogger::Log(GetLastError(), "Failed to call ConInitialize.");
		return -1;
	}

	// ������ Ŭ���� ����, ���α׷��� ����� �� ������ Ŭ������ ������. 21.11.15
	RegistWindowClassExW T(HandleMessageSetup, hInstance, L"Basic");

#pragma endregion

	// ������ ����, ������ ����, ������ ���� 21.11.15
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