#pragma once

#include "../_Include.h"

class Camera;
class RenderTarget;

/// <summary>
/// 게임의 렌더를 관리하는 컴포넌트
/// 21.11.15
/// </summary>
class SubGraphics : public Component
{

#pragma region ConDestructor

public:
	SubGraphics() {	}
	SubGraphics(SafePtr<Camera> Camera_ptr, SafePtr<RenderTarget> Render_target, float resolutionWidth, float resolutionHeight);
	~SubGraphics();

#pragma endregion

	// 게임 렌더를 위해 필요한 변수
	// 21.11.15
#pragma region Val_SubGraphics

public:
	ID3D11Device* Device = NULL;
	ID3D11DeviceContext* Device_context = NULL;

#pragma message (__FILE__ "(" _CRT_STRINGIZE(__LINE__) ")" ": warning: viewport를 Camera에서 설정하도록 하면 더 직관적인지에 대해서 고민해봐야함.")
	// viewport를 렌더타겟에서 설정하도록 하면
	// update_viewport_size, update_viewport_size의 함수를
	// 병합하여 보다 단순화 시킬 수 있지 않을까 생각함
	// 
	// 실행을 위해서는 검증이 필요함
	// 
	// 라이트 지오메트리 버퍼와 칼라 지오메트리 버퍼의 크기를 다르게 하기 위해서
	// 필요하다고 생각함.
	// 21.11.15
	CD3D11_VIEWPORT viewport;
	SafePtr<Camera> Camera_ptr;
	SafePtr<RenderTarget> Render_target;

	std::list<SafePtr<Scene>> Drawing_scenes;

	/// <summary>
	/// draw 필요한 오프셋 변수, 필요할 때 오버라이드 하여 사용함.
	/// 21.11.15
	/// </summary>
	UINT Offset = 0;

#pragma endregion

	// SubGraphics의 메서드
	// 21.11.15
#pragma region Func_SubGraphics

public:
	/// <summary>
	/// SubGraphics가 가지고 있는 정보를 설정하고
	/// 드로잉 씬 리스트에 따라 draw_all_renderer를 호출함.
	/// 21.11.15
	/// </summary>
	void only_draw();

	/// <summary>
	/// viewport desc의 값을 업데이트 하는 메서드
	/// 21.11.15
	/// </summary>
	/// <param name="topLeftX"></param>
	/// <param name="topLeftY"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="minDepth"></param>
	/// <param name="maxDepth"></param>
	void update_viewport_size(
		FLOAT topLeftX,
		FLOAT topLeftY,
		FLOAT width,
		FLOAT height,
		FLOAT minDepth = D3D11_MIN_DEPTH,
		FLOAT maxDepth = D3D11_MAX_DEPTH
	);

	/// <summary>
	/// update_viewport_size를 width와 height만으로 업데이트 하는 메서드
	/// 21.11.15
	/// </summary>
	/// <param name="window_width"></param>
	/// <param name="window_height"></param>
	void update_viewport_size(
		UINT window_width,
		UINT window_height
	);

#pragma endregion

#pragma region Component

public:
	virtual void awake() override;
	virtual void sleep() override;
	virtual void draw_detail_view() override;

#pragma endregion

};