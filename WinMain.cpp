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

	// ���̴ϼȶ����� �׽�Ʈ 21.11.15
	// 
	// CoInitialize��?
	// https://docs.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-coinitialize
	//
	// ComPtr�� ����ϱ� ���ؼ� COM ���̺귯���� �ʱ�ȭ
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